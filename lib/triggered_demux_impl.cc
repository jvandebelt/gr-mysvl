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
    triggered_demux::make(size_t itemsize, size_t trigger_size, const std::vector<int> &lengths, int trigger_delay, bool add_tags)
    {
      return gnuradio::get_initial_sptr
        (new triggered_demux_impl(itemsize, trigger_size, lengths, trigger_delay, add_tags));
    }
    
    /*
     * The private constructor
     */
    triggered_demux_impl::triggered_demux_impl(size_t itemsize, size_t trigger_size, const std::vector<int> &lengths, int trigger_delay, bool add_tags)
      : gr::block("triggered_demux",
              gr::io_signature::make2(1, 2, itemsize, trigger_size),
              gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize)),
              d_itemsize(itemsize),
              d_trigger_size(trigger_size),
	          d_stream(0),
	          d_trigger_delay(trigger_delay),
	          d_lengths(lengths),
	          d_add_tags(add_tags),
	          d_next_packet_length(0),
	          d_last_sample(0.0),
	          //d_history(0),
	          d_min_output_items(0),
	          d_reset(false)
    {   
        while (d_lengths[d_stream] == 0) {
            d_stream++;
            if (d_stream == d_lengths.size()) {
                throw std::invalid_argument("At least one size must be non-zero.");
            }
        }
        set_tag_propagation_policy(TPP_DONT);
       
        for(int i=0; i< d_lengths.size();i++) {
            d_min_output_items += d_lengths[i];
            }
        set_output_multiple(d_min_output_items);
        //set_max_output_buffer(d_min_output_items*16);
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
                  
      int items_in = 0; // Items in
      int items_out = 0; // Total items out
      gr_vector_int output_index(d_lengths.size(), 0); // Items written
      std::vector<gr::tag_t> stream_t;  
      d_next_packet_length=d_lengths[d_stream];
      //ninput_items[0] += d_history;
      //ninput_items[1] += d_history;
      
      //printf("Input items[0]: %d\n", ninput_items[0]);
      //printf("Input items[1]: %d\n", ninput_items[1]); 
      //printf("Output items: %d\n", noutput_items); 
      
      /*
      
      */
      
          
      while (noutput_items-items_out >=d_lengths[d_stream] && ninput_items[0]-items_in >=d_lengths[d_stream] && ninput_items[1]-items_in >=d_lengths[d_stream]) {
            gr::thread::scoped_lock guard(d_setlock);
            
            if(d_delay_left > 0 && d_delay_left <= d_lengths[d_stream]) {
                d_next_packet_length=d_delay_left;
                d_delay_left=0; 
            }
            else if(d_delay_left > 0) {
                d_next_packet_length= d_lengths[d_stream];
                d_delay_left -= d_lengths[d_stream];      
            }
            
            d_next_packet_length = find_trigger_offset(items_in, items_in+d_lengths[d_stream], trigger) - items_in;  
            //printf("Offset found at %d\n", items_in + d_next_packet_length);        
            if(d_reset) {
                if(d_next_packet_length + d_trigger_delay <= d_lengths[d_stream])                
                   d_next_packet_length +=d_trigger_delay;
                else{
                    d_delay_left=d_next_packet_length+d_trigger_delay-d_lengths[d_stream];
                    d_next_packet_length = d_lengths[d_stream];}
            }
                        
            //printf("Next packet length %d\n", d_next_packet_length);
            
            //if(d_delay_left)
                //printf("Delay left: %d\n", d_delay_left);  
            
            if(d_next_packet_length == d_lengths[d_stream]) {                      
                out = (char *) output_items[d_stream] + output_index[d_stream]*d_itemsize;  
                              
                memcpy(out, &in[items_in*d_itemsize], d_next_packet_length*d_itemsize);
                
                if(d_add_tags){
                  add_item_tag(d_stream, nitems_written(d_stream) +output_index[d_stream], pmt::intern("trigger"), pmt::from_long(d_next_packet_length));       
                }
            
            // repeat existing tags
                get_tags_in_window(stream_t, 0,items_in, items_in + d_next_packet_length);
                BOOST_FOREACH(gr::tag_t t, stream_t){
                  t.offset = t.offset - nitems_read(0) - items_in + nitems_written(d_stream) + output_index[d_stream];
                  add_item_tag(d_stream, t);
                }
                
                output_index[d_stream] += d_next_packet_length;
                items_out += d_next_packet_length;                
                items_in += d_next_packet_length;
                        
                do { // Skip all those outputs with zero length
                    d_stream = (d_stream+1) % d_lengths.size();
                  } while (d_lengths[d_stream] == 0);
                  
                if(d_reset && d_delay_left ==0) {
                    d_reset = false;
                    d_stream = 0;
                }
            }
            else {
                items_in += d_next_packet_length;
            }
        } 
            
            // Save any remaining samples
            
      set_history(std::min(ninput_items[0], ninput_items[1])-items_in);
      //d_history = std::min(ninput_items[0], ninput_items[1])-items_in;
      
      //printf("In: %d, Out: %d\n", items_in, items_out);   
    
      for(int i = 0; i < output_index.size(); i++) {
	    produce((int) i, output_index[i]);
      }
           
      consume(0, items_in);
      consume(1, items_in);
      
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

