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
		
		for(int i=0; i< d_ninputs;i++) {
            d_dropped.push_back(false);
            }

      	return true;
    }	

	/*
	bool sync_channels_impl::drop_samples(int drop[], int drop_offsets[]){
	
		//std::cout<< "in drop samples" << std::endl;

		bool offsets_exist=false;
		bool drop_all_same=true;
		int d_highest_drop=0;

		// first check whether we have offsets or are alligned
		// also check if all have dropped the same amount
		for(int i=0; i< d_ninputs; i++){
			if((drop[i])!=0){
				offsets_exist=true;
				//printf("input %d drop of %d \n",i, (drop[i]%d_packet_length));
				}
			if((drop[i])!= drop[0]){
				drop_all_same=false;
				//printf("input %d drop of %d \n",i, (drop[i]%d_packet_length));
				}
			//keep track of highest drop - for next part
			if(drop[i]>d_highest_drop)
				d_highest_drop=drop[i];			
		}	
		
		//std::cout<< "offsets_exist" <<offsets_exist <<std::endl;
		if(!offsets_exist){
			return false;
		}
	
		if(drop_all_same){
			return false;
		}	

		//not alligned: need to drop from all except the stream(s) with highest drop
		
		for(int i=0; i< d_ninputs; i++){
			drop_offsets[i]=d_highest_drop-drop[i];
		}
		return true;
	}

	*/

	int factorial(int n)
		{
		  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
		}
	
	bool sync_channels_impl::drop_samples(int trigger_values[], int packets_to_drop[]){
		
		bool values_exist=true;
		bool same_value = true;
		int number_of_comparisons = factorial(d_ninputs)/2;
		int d_closest_values[number_of_comparisons];
		int largest_difference=0;
		int first_stream=0;
		int d_index=0;

		//printf("in drop_samples. size of d_closest is %d\n", number_of_comparisons);

		// if no value, don't drop packets
		for(int i=0; i< d_ninputs; i++){
			if(trigger_values[i] <1 || trigger_values[i]>64){
				values_exist=false;
				//printf("input %d drop of %d \n",i, (drop[i]%d_packet_length));
				}
			if(trigger_values[i]!= trigger_values[0]){
				same_value=false;
				}
			for(int j=i; j<d_ninputs;j++){
				if(j!=i){
					d_closest_values[d_index]=trigger_values[i]-trigger_values[j];
					//printf("difference of input %d value and input %d value is %d\n",i, j, d_closest_values[d_index]);
					//printf("i is %d value and j is %d\n",i, j);
					if(abs(d_closest_values[d_index])>=largest_difference){
						largest_difference=abs(d_closest_values[d_index]);
						if(d_closest_values[d_index]>=0)
							first_stream=i;
						else
							first_stream=j;
					}
					d_index++;
				}
			}
		}
			

		if(!values_exist){		// outside range
			return false;
		}	
		
		if(same_value){			// all the same
			return false;
		}
			
				//printf("input %d drop of %d \n",i, (drop[i]%d_packet_length));
				
			//keep track of highest drop - for next part
			

		//std::cout<< "offsets_exist" <<offsets_exist <<std::endl;
		
		//printf("First stream is %d\n", first_stream);

		for(int i=0; i< d_ninputs; i++){
			packets_to_drop[i]=abs(trigger_values[first_stream]-trigger_values[i]);
		}
			

		//not alligned: need to drop from all except the stream(s) with highest drop
		
		//for(int i=0; i< d_ninputs; i++){
			//drop_offsets[i]=d_highest_value-trigger_values[i];
		//}
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
		int trigger_values[d_ninputs];
		int packets_to_drop[d_ninputs];
		bool d_stop=false;

				
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
				trigger_values[i] =0;
				packets_to_drop[i]=0;
			}	

			for(int i = 0; i <d_ninputs; i++) {
				get_tags_in_window(tags, i, items_in[i], items_in[i]+d_packet_length, pmt::intern("trigger"));
				if (tags.size() > 0){
					trigger_values[i]=pmt::to_long(tags[0].value);
					//printf("Input %d drop %d packet\n",i, trigger_values[i]);
					}
				tags.clear();
		  	}

			if(drop_samples(trigger_values, packets_to_drop)){
				for(int i = 0; i <d_ninputs; i++) {
					//printf("Input %d: need to drop %d packets \n", i, packets_to_drop[i]);
					//consume(i, d_packet_length*drop_offsets[i]);
					if(d_packet_length*packets_to_drop[i]<=ninput_items[i]-items_in[i] || d_stop)
						items_in[i]+=d_packet_length*packets_to_drop[i];
					else{
						items_in[i]=ninput_items[i];
						d_stop=true;
					}
					if(d_stop){
						for(int i = 0; i < d_ninputs; i++) {
							consume(i, items_in[i]);
							produce(i, items_out);
		  				}
						if(items_out > 0)
							return WORK_CALLED_PRODUCE;
						else
							return 0;
					}
				}
			}	
			else{
				for(int i = 0; i <d_ninputs; i++) {
					memcpy(out[i]+items_in[i]*d_itemsize, in[i], d_itemsize*d_packet_length);
					in[i] += d_packet_length*d_itemsize;
							
					get_tags_in_window(propagate_tags, i,items_in[i], items_in[i] + d_packet_length), pmt::intern("trigger");
		            BOOST_FOREACH(gr::tag_t t, propagate_tags){;
						t.offset = t.offset - nitems_read(i) -items_in[i] + nitems_written(i) +items_out; //fix new offset
		            	add_item_tag(i, t);
		            }
					items_in[i]+=d_packet_length;
				}
				items_out+=d_packet_length;
			}
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

