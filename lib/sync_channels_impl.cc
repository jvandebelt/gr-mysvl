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
#include "sync_channels_impl.h"
#include <cstdio>

namespace gr {
  namespace mysvl {

    sync_channels::sptr
    sync_channels::make(unsigned int packet_length)
    {
      return gnuradio::get_initial_sptr
        (new sync_channels_impl(packet_length));
    }

    /*
     * The private constructor
     */
    sync_channels_impl::sync_channels_impl(unsigned int packet_length)
      : gr::block("sync_channels",
              	gr::io_signature::make(1, io_signature::IO_INFINITE, sizeof(gr_complex)),
              	gr::io_signature::make(1, io_signature::IO_INFINITE, sizeof(gr_complex))),
				d_packet_length(packet_length)
    {	
		set_tag_propagation_policy(TPP_DONT);
		d_itemsize=sizeof(gr_complex);
		set_output_multiple(d_packet_length);
	}

    /*
     * Our virtual destructor.
     */
    sync_channels_impl::~sync_channels_impl()
    {
    }
	
	bool
    sync_channels_impl::check_topology(int ninputs, int noutputs)
    {
		d_ninputs  = ninputs;
     	d_noutputs = noutputs;
	  	if(ninputs != noutputs)
			return false;

      	set_output_multiple(d_packet_length);

      	return true;
    }	

	bool sync_channels_impl::drop_samples(int offsets[], int drop_offsets[]){
	
		//std::cout<< "in drop samples" << std::endl;

		bool offsets_exist=false;
		// first check whether we have offsets or are alligned
		for(int i=0; i< d_ninputs; i++){
			if((offsets[i]%d_packet_length)!=0){
				offsets_exist=true;
				//printf("input %d offset of %d \n",i, (offsets[i]%d_packet_length));
				}				
		}

		//std::cout<< "offsets_exist" <<offsets_exist <<std::endl;
		if(!offsets_exist){
			return false;
		}
		
		//not alligned: need to drop from all
		for(int i=0; i< d_ninputs; i++){
			if((offsets[i]%d_packet_length) == 0)
				drop_offsets[i]=d_packet_length;
			else
				drop_offsets[i]=0;
		}
		return true;
	}

	bool sync_channels_impl::check_size(int items_in[], int items_out, gr_vector_int &ninput_items, int noutput_items){
		for(int i=0; i< d_ninputs; i++){
			if(ninput_items[i]-items_in[i] < d_packet_length)
				return false;
			}
		if(noutput_items-items_out < d_packet_length)
			return false;
		else
			return true;	
	}

    void
    sync_channels_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      	for (unsigned int i = 0; i < ninput_items_required.size(); ++i) 
		{
        	ninput_items_required[i] =noutput_items;
      	}
    }

    int
    sync_channels_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
		const char **in = (const  char **) &input_items[0];
      	char **out = (char **) &output_items[0];
		
		int items_in[d_ninputs]; // Items in
      	int items_out =0; // Total items out
		
		
	
	  	//int count = 0, totalcount = (int) noutput_items/d_packet_length;

		gr_vector_int output_index(d_ninputs, 0); // Items written
      	std::vector<gr::tag_t> tags;
		std::vector<gr::tag_t> propagate_tags;
		int offsets[d_ninputs];
		int drop_offsets[d_ninputs];

				
		//drop_samples(offsets, drop_offsets);
		/*
		if(drop_samples(offsets, drop_offsets)){
			for(int i =0; i<d_ninputs; i++)
			{
				consume(i, drop_offsets[i]);
			  	set_history(ninput_items[i] - drop_offsets[i]);
			}
			return 0;
		}
		*/

		for(int i = 0; i <d_ninputs; i++) {
				items_in[i] =0;
			}	

	  	while(check_size(items_in, items_out, ninput_items, noutput_items)) {
			//gr::thread::scoped_lock guard(d_setlock);			
		
			for(int i = 0; i <d_ninputs; i++) {
				offsets[i] =0;
				drop_offsets[i]=0;
			}	

			for(int i = 0; i <d_ninputs; i++) {
				get_tags_in_window(tags, i, items_in[i], items_in[i]+d_packet_length, pmt::intern("drop"));
				if (tags.size() > 0){
					offsets[i]=pmt::to_long(tags[0].value);
					//printf("Input %d offset of %d \n",i, offsets[i]);
				}
				tags.clear();
		  	}

			/*
			if(drop_samples(offsets, drop_offsets)){
				for(int i = 0; i <d_ninputs; i++) {
					//printf("Input %d: need to drop %d samples \n", i, drop_offsets[i]);
					consume(i, drop_offsets[i]);
					items_in[i]+=drop_offsets[i];
					}
				set_history(ninput_items[0]-items_in[0]);
				return 0;
			}
			*/	
			//else{
				for(int i = 0; i <d_ninputs; i++) {
					memcpy(out[i]+items_in[i]*d_itemsize, in[i], d_itemsize*d_packet_length);
					in[i] += d_packet_length*d_itemsize;
							
					get_tags_in_window(propagate_tags, i,items_in[i], items_in[i] + d_packet_length);
		            BOOST_FOREACH(gr::tag_t t, propagate_tags){;
		              add_item_tag(i, t);
		            }
					items_in[i]+=d_packet_length;
				}
				items_out+=d_packet_length;
			//}
			//count ++;
		}
		
		//int d_history=ninput_items[0];
		//for(int i = 0; i < d_ninputs; i++) {
		//	if(d_history>ninput_items[i]-items_in[i])
		//		d_history=ninput_items[i]-items_in[i];
		//}

     	//set_history(d_history);
          
		  for(int i = 0; i < d_ninputs; i++) {
			consume((int) i, items_in[i]);
			produce((int) i, items_out);
		  }
           
      		
		return WORK_CALLED_PRODUCE;
	}

  } /* namespace mysvl */
} /* namespace gr */

