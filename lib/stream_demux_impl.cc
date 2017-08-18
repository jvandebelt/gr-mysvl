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
#include "stream_demux_impl.h"
#include <cstdio>

namespace gr {
  namespace mysvl {

    stream_demux::sptr
    stream_demux::make(size_t itemsize, const std::vector<int> &lengths, bool add_tags)
    {
      return gnuradio::get_initial_sptr
        (new stream_demux_impl(itemsize, lengths, add_tags));
    }

    /*
     * The private constructor
     */
    stream_demux_impl::stream_demux_impl(size_t itemsize, const std::vector<int> &lengths, bool add_tags)
      : gr::block("stream_demux",
              gr::io_signature::make(1, 1, itemsize),
              gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize)),
            d_itemsize(itemsize),
	        d_stream(0),
	        d_add_tags(add_tags),
	        d_lengths(lengths)
    {    
        while (d_lengths[d_stream] == 0) {
            d_stream++;
            if (d_stream == d_lengths.size()) {
                throw std::invalid_argument("At least one size must be non-zero.");
            }
        }
        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    stream_demux_impl::~stream_demux_impl()
    {
    }

    void
    stream_demux_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    stream_demux_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      char *out;
      
      int items_in = 0; // Items in
      int items_out = 0; // Total items out
      gr_vector_int output_index(d_lengths.size(), 0); // Items written
      std::vector<gr::tag_t> stream_t;  
      
      while (ninput_items[0]-items_in >d_lengths[d_stream] && noutput_items-items_out >d_lengths[d_stream]) {
            gr::thread::scoped_lock guard(d_setlock);
            
            out = (char *) output_items[d_stream] + output_index[d_stream]*d_itemsize;  
                          
            memcpy(out, &in[items_in*d_itemsize], d_lengths[d_stream]*d_itemsize);
            
            if(d_add_tags){
              add_item_tag(d_stream, nitems_written(d_stream) +output_index[d_stream], pmt::intern("trigger"), pmt::from_long(d_lengths[d_stream]));       
            }
            
            // propagate existing tags
            get_tags_in_window(stream_t, 0,items_in, items_in + d_lengths[d_stream]);
            BOOST_FOREACH(gr::tag_t t, stream_t){
              t.offset = t.offset - nitems_read(0) - items_in + nitems_written(d_stream) + output_index[d_stream];
              add_item_tag(d_stream, t);
            }
            
            output_index[d_stream] += d_lengths[d_stream];
            items_out += d_lengths[d_stream];                
            items_in += d_lengths[d_stream];
            
            
            do { // Skip all those outputs with zero length
                d_stream = (d_stream+1) % d_lengths.size();
              } while (d_lengths[d_stream] == 0);
        } 
            
            // Save any remaining samples
            set_history(ninput_items[0]-items_in);
    
      for(int i = 0; i < output_index.size(); i++) {
	    produce((int) i, output_index[i]);
      }
           
      consume (0, items_in);

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }

  } /* namespace mysvl */
} /* namespace gr */
