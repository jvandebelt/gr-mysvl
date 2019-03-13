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
              gr::io_signature::make2(1, 2, sizeof(gr_complex), sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {	d_packet_size=packet_size;
		set_tag_propagation_policy(TPP_DONT);
		set_output_multiple(d_packet_size);
		d_drop=0;
		//d_wait=0;
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
		
		//	ninput_items_required[1] = (noutput_items/d_packet_size)*1*sizeof(float);
		//set_output_multiple(d_packet_size);
		//d_wait++;
	}
		

    int
    drop_packet_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      	const char *in = (const char *) input_items[0];
		const float *header = (const float *) input_items[1];
     	char *out = (char *) output_items[0];
		
		std::vector<gr::tag_t> propagate_tags; 
    	// Do <+signal processing+>
   		// Tell runtime system how many input items we consumed on
     	// each input stream.
		int header_offset=0;
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
			
			// dropping samples correctly
			//add "drop" intruction tags
			
			if ((d_tags[i].offset % d_packet_size) != 0)
		    {
				//d_drop++;
		      	//printf("Unsynchronized tag. Dropping %lu samples\n", d_tags[i].offset%d_packet_size);
				//printf("total samples dropped is %lu\n", nitems_read(0)-nitems_written(0)+d_tags[i].offset% d_packet_size);

				memcpy(out, &in[0], sizeof(gr_complex) * d_packet_size*(d_tags[i].offset/d_packet_size));
			
				get_tags_in_window(propagate_tags, 0,0, d_packet_size*(d_tags[i].offset/d_packet_size), pmt::intern("trigger"));
	           	BOOST_FOREACH(gr::tag_t t, propagate_tags){
						t.offset = t.offset - nitems_read(0) + nitems_written(0); //fix new offset
						//printf("Header value %d\n", header[header_offset]);
						t.value= pmt::from_long((int)(header[header_offset])); //need plus one for the zero, or not?
						header_offset+=sizeof(char);
						add_item_tag(0, t);
	            }

				produce(0, d_packet_size*(d_tags[i].offset / d_packet_size));			  
				
				if((d_tags[i].offset / d_packet_size)*sizeof(char) <=ninput_items[1])
					consume(1, (d_tags[i].offset / d_packet_size)*sizeof(char));
				else
					consume(1, ninput_items[1]);
		      	consume(0, d_tags[i].offset);
		      	set_history(ninput_items[0] - d_tags[i].offset);
				return WORK_CALLED_PRODUCE;			
		    }
		}

		//if(d_drop){
				//add_item_tag(0, nitems_written(0) +1, pmt::intern("drop"), pmt::from_long(d_drop));     
				//d_drop=0;
			//}

		//for(int i=0; i< ninput_items[1];i++) {
		//	printf("Header value %f\n", header[i]);
		//}
	
		memcpy(out, &in[0], sizeof(gr_complex) *d_packet_size*(ninput_items[0]/d_packet_size));
		
		get_tags_in_window(propagate_tags, 0,0, d_packet_size*(ninput_items[0]/d_packet_size), pmt::intern("trigger"));
	            BOOST_FOREACH(gr::tag_t t, propagate_tags){;
					t.offset = t.offset - nitems_read(0) + nitems_written(0); //fix new offset
					//std::cout << "current tag keyis  " << t.key << std::endl;
					t.value= pmt::from_long((int)(header[header_offset])); //need plus one for the zero, or not?
					//std::cout << "current tag value is " << t.value << std::endl;
					header_offset+=sizeof(char);
	            	add_item_tag(0, t);
	            }
		//GR_LOG_INFO(d_logger, "printing header values");

		//for(int i=0; i<(ninput_items[0]/d_packet_size); i++){
		//	printf("Header value %f\n", header[i]);
		//}
		
		consume(0, d_packet_size*(ninput_items[0]/d_packet_size));
		if((sizeof(char)*(ninput_items[0]/d_packet_size)) <=ninput_items[1])
				consume(1, sizeof(char)*(ninput_items[0]/d_packet_size));
			else
				consume(1, ninput_items[1]);

		produce(0, d_packet_size*(ninput_items[0]/d_packet_size));
		set_history(ninput_items[0]%d_packet_size);

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE; 
    }

  } /* namespace mysvl */
} /* namespace gr */

