/* -*- c++ -*- */
/* 
 * Copyright 2017 Jonathan van de Belt
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "triggered_demux_impl.h"

namespace gr {
  namespace mysvl {

    triggered_demux::sptr
    triggered_demux::make(size_t itemsize, size_t trigger_size, const std::vector<int> &lengths, int trigger_delay)
    {
      return gnuradio::get_initial_sptr
        (new triggered_demux_impl(itemsize, trigger_size, lengths, trigger_delay));
    }
    
    /*
     * The private constructor
     */
    triggered_demux_impl::triggered_demux_impl(size_t itemsize, size_t trigger_size, const std::vector<int> &lengths, int trigger_delay)
      : gr::block("triggered_demux",
              gr::io_signature::make2(1, 2, itemsize, trigger_size),
              gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize)),
              d_itemsize(itemsize),
              d_trigger_size(trigger_size),
	          d_stream(0),
	          d_residual(0),
	          d_trigger_delay(trigger_delay),
	          d_lengths(lengths),
	          d_reset(false),
	          d_offset(0),
	          d_iteration(0),
	          d_delay_left(0),
	          d_last_sample(0.0)
    {   
        while (d_lengths[d_stream] == 0) {
            d_stream++;
            if (d_stream == d_lengths.size()) {
                throw std::invalid_argument("At least one size must be non-zero.");
            }
        }
        d_residual = d_lengths[d_stream];
        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    triggered_demux_impl::~triggered_demux_impl()
    {
    }

    void
    triggered_demux_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {   
      ninput_items_required[0] = noutput_items;
      ninput_items_required[1] = noutput_items;
    }

    int
    triggered_demux_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      const float *trigger = (const float *) input_items[1];
      char *out;
             
      //printf("Iteration: %d\n", d_iteration);
      d_iteration++;
      
      int items_in = 0; // Items in
      gr_vector_int output_index(d_lengths.size(), 0); // Items written
      std::vector<gr::tag_t> stream_t;    
      
      if(d_delay_left > 0 && d_delay_left < noutput_items) {
        d_offset=d_delay_left;
        d_delay_left=0; 
      }
      else if(d_delay_left > 0) {
        d_offset=noutput_items;
        d_delay_left-=noutput_items;      
      }
      
      //printf("output items: %d\n", noutput_items);
      //printf("Delay left %d\n", d_delay_left);
      
      while (items_in < noutput_items) {
        
        //printf("items in: %d\n", items_in);
        
        if(!d_reset){
            d_offset = find_trigger_offset(items_in, noutput_items, trigger);
            if(d_reset) {
                //printf("Trigger found at %d\n", d_offset);
                if(d_offset + d_trigger_delay <= noutput_items)
                    d_offset +=d_trigger_delay;
                else {
                    d_delay_left = d_offset + d_trigger_delay - noutput_items;
                    d_offset=noutput_items;
                    //printf("Delay left %d\n", d_delay_left);               
                }
            }
        }
           
        int space_left_in_buffers = std::min(
              d_offset - items_in, // Space left in output buffer
              ninput_items[0] - items_in  // Space left in input buffer
        );
        int items_to_copy = std::min(
            space_left_in_buffers,
            d_residual
        );
                
        out = (char *) output_items[d_stream] + output_index[d_stream]*d_itemsize;
        memcpy(out, &in[items_in*d_itemsize], items_to_copy*d_itemsize);
        
        get_tags_in_window(stream_t, 0,items_in, items_in + items_to_copy);
        BOOST_FOREACH(gr::tag_t t, stream_t){
          t.offset = t.offset - nitems_read(0) - items_in + nitems_written(d_stream) + output_index[d_stream];
          add_item_tag(d_stream, t);
        }
        
        items_in += items_to_copy;
        output_index[d_stream] += items_to_copy;
        d_residual -= items_to_copy;
        
        if(d_reset && items_in == d_offset && d_delay_left ==0) {
            //printf("Reset\n");
            //printf("items in: %d\n", items_in);
            d_reset = false;
            d_stream = 0;
            d_residual = d_lengths[d_stream];
            
            // Why does reset break out of main while loop??
            // Maybe it doesn't
        }
        
        if (d_residual == 0) {
	      do { // Skip all those outputs with zero length
	        d_stream = (d_stream+1) % d_lengths.size();
	      } while (d_lengths[d_stream] == 0);
              d_residual = d_lengths[d_stream];
              if(d_stream == 1){
	            add_item_tag(1, nitems_written(1) +output_index[1], pmt::intern("trigger"),
                     pmt::from_double(output_index[1]));}
        } 
      }
      //printf("items in: %d\n", items_in);
      
      for (size_t i = 0; i < output_index.size(); i++) {
	    produce((int) i, output_index[i]);
      }
           
      consume_each(items_in);
      
      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }
    
    int triggered_demux_impl::find_trigger_offset(int start, int end, const float *trigger_signal){
    
      // printf("Start is %d, end is %d\n", start, end);
       
       int offset = end;       
       if(trigger_signal) {
           if(d_last_sample ==0 && trigger_signal[start] > 0) {
                d_reset = true;
                offset=start;
                d_last_sample = trigger_signal[start];
                return offset;           
           }
           for(int i = start+1; i<end; i++) {
               if(trigger_signal[i-1] == 0 && trigger_signal[i] > 0){
                    d_reset = true;
                    offset=i;   
                    d_last_sample = trigger_signal[i];              
                    return offset;
               }
           }
       }
    
        d_last_sample = trigger_signal[end-1];
        return offset;
    }

  } /* namespace mysvl */
} /* namespace gr */

