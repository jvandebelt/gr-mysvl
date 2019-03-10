/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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
#include "drop_packet_impl.h"

namespace gr {
  namespace mysvl {

    drop_packet::sptr
    drop_packet::make(int packet_size)
    {
      return gnuradio::get_initial_sptr
        (new drop_packet_impl(packet_size));
    }

    /*
     * The private constructor
     */
    drop_packet_impl::drop_packet_impl(int packet_size)
      : gr::block("drop_packet",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {	d_packet_size=packet_size;
		set_tag_propagation_policy(TPP_DONT);
		set_output_multiple(d_packet_size);
		d_drop=0;
	}

    /*
     * Our virtual destructor.
     */
    drop_packet_impl::~drop_packet_impl()
    {
    }

    void
    drop_packet_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
		set_output_multiple(d_packet_size);
		}

    int
    drop_packet_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      	const char *in = (const char *) input_items[0];
     	char *out = (char *) output_items[0];
		
		std::vector<gr::tag_t> propagate_tags; 
    	// Do <+signal processing+>
   		// Tell runtime system how many input items we consumed on
     	// each input stream.
		
		d_tags.clear();

		//if(ninput_items[0]%d_packet_size){

			//printf("Ninput_items %d\n", ninput_items[0]);
			//set_history(ninput_items[0]);
			//return 0;
		//}

      	get_tags_in_window(d_tags, 0,0, ninput_items[0], pmt::intern("trigger")); //some problem here, fixed using window instead of range (relative vs absolute)
		
		for(int i=0; i<d_tags.size(); i++){
		
			d_tags[i].offset = d_tags[i].offset-nitems_read(0); //convert to relative offset
			//printf("Tag %d value %lu \n", i, pmt::to_long(d_tags[i].value));
			//printf("Tag %d offset %lu \n", i, d_tags[i].offset);
			
			//TODO still some error here when local_offset<packet_size
			// almost working
			//add "drop" intruction tags
			//propagate tags properly?
			
			if ((d_tags[i].offset % d_packet_size) != 0)
		    {
		      	printf("Unsynchronized tag. Dropping %lu samples\n", d_tags[i].offset%d_packet_size);
				printf("total samples dropped is %lu\n", nitems_read(0)-nitems_written(0)+d_tags[i].offset% d_packet_size);

				memcpy(out, &in[0], sizeof(gr_complex) * d_packet_size*(d_tags[i].offset/d_packet_size));
			
				get_tags_in_window(propagate_tags, 0,0, d_packet_size*(d_tags[i].offset/d_packet_size));
	           	BOOST_FOREACH(gr::tag_t t, propagate_tags){
						t.offset = t.offset - nitems_read(0) + nitems_written(0); //fix new offset
						add_item_tag(0, t);
	            }

				produce(0, d_packet_size*(d_tags[i].offset / d_packet_size));			  

		      	consume(0, d_tags[i].offset);
		      	set_history(ninput_items[0] - d_tags[i].offset);
				return WORK_CALLED_PRODUCE;			
		    }
		}

		memcpy(out, &in[0], sizeof(gr_complex) *d_packet_size*(ninput_items[0]/d_packet_size));
		
		get_tags_in_window(propagate_tags, 0,0, d_packet_size*(ninput_items[0]/d_packet_size));
	            BOOST_FOREACH(gr::tag_t t, propagate_tags){;
					t.offset = t.offset - nitems_read(0) + nitems_written(0); //fix new offset
	            	add_item_tag(0, t);
	            }
		
		consume(0, d_packet_size*(ninput_items[0]/d_packet_size));
		produce(0, d_packet_size*(ninput_items[0]/d_packet_size));
		set_history(ninput_items[0]%d_packet_size);

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }

  } /* namespace mysvl */
} /* namespace gr */

