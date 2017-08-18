/* -*- c++ -*- */
/* 
 * Copyright 2016 Jonathan van de Belt.
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
#include <boost/math/common_factor_rt.hpp>
#include "svl_impl.h"
#include <cstdio>

namespace gr {
  namespace mysvl {

    svl::sptr
    svl::make(size_t itemsize, unsigned int blocksize, const char *map_filename,
			 const char *fft_filename)
    { 
		return gnuradio::get_initial_sptr
			(new svl_impl(itemsize, blocksize, map_filename, fft_filename));
    }

    /*
     * The private constructor
     */
    svl_impl::svl_impl(size_t itemsize, unsigned int blocksize, const char *map_filename, 
				const char *fft_filename )
      : gr::block("svl",
			gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize),
			gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize)),
			d_itemsize(itemsize), d_blocksize(blocksize), d_current_input(0), d_current_output(0),
			d_hypervisor(map_filename, fft_filename, itemsize)
    {
		// check blocksize
		std::vector<fft_parameters> d_fft_list = d_hypervisor.get_fft_list();
		int smallest_fft_size=d_fft_list[0].fft_size;

		d_fft_list_in.clear();
		d_fft_list_out.clear();
		d_buffer_items=0;
		//d_buffer_input=0;
		//d_buffer_remaining=0;

		for(int i=0; i<d_fft_list.size(); i++){
			if(d_fft_list[i].fft_size < smallest_fft_size)
				smallest_fft_size=d_fft_list[i].fft_size;
			if(d_fft_list[i].input)
				d_fft_list_in.push_back(d_fft_list[i]);
			else
				d_fft_list_out.push_back(d_fft_list[i]);
		}

        // Remove power of two contraint
		//if(smallest_fft_size < d_blocksize || smallest_fft_size%d_blocksize != 0)
        	//throw std::runtime_error("error: blocksize must be an equal or smaller power of two than smallest fft size\n");

		set_output_multiple(d_hypervisor.get_fft_span());		
		//d_hypervisor.print_spectrum_map();
	}

    /*
     * Our virtual destructor.
     */
    svl_impl::~svl_impl()
    {
    }

	bool
	svl_impl::check_topology(int ninputs, int noutputs)
	{
		d_ninputs = ninputs;
		d_noutputs = noutputs;
		int total_input_size =0;
		int total_output_size=0;
            
        for(int i=0; i<  d_fft_list_in.size(); i++)
            total_input_size += d_fft_list_in[i].fft_size;
        for(int i=0; i<  d_fft_list_out.size(); i++)
            total_output_size += d_fft_list_out[i].fft_size;
    
		//set_relative_rate((double)total_input_size/(double)total_output_size);
		//printf("Relative rate: %f", (double)total_input_size/(double)total_output_size);
		//d_factor = boost::math::lcm(d_ninputs, d_noutputs); //not needed?
		d_hypervisor.create_streams(ninputs, noutputs);
		//d_hypervisor.do_fft_test();
		set_output_multiple(d_hypervisor.get_fft_span());
		//printf("Hypervisor span: %d \n", d_hypervisor.get_fft_span());  
		//if(!d_hypervisor.check_spectrum_map(ninputs, noutputs))
        	//throw std::runtime_error("error: inconsistency between configuration and spectrum_map\n");
		return true;
	}

    void
    svl_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        //GR_LOG_INFO(d_logger, "MySVL");
        //printf("Output items %d \n", noutput_items);
      	for(unsigned int i = 0; i < ninput_items_required.size(); ++i){
		 	ninput_items_required[i] = (int) (noutput_items / d_hypervisor.get_fft_span() *d_fft_list_in[i].fft_size + .5);
		 	//printf("Number of items required for input %d is %d \n", i, ninput_items_required[i]);
		 	}
    }

    int
    svl_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {

		// Each element in the vector void star is a stream of complex samples.
	
		//printf("------------------------- \n");
		//printf("Number of Output Items: %d\n", noutput_items);
		
		//for(int i = 0; i< ninput_items.size(); i++) {
		//	printf("Number of Input Items %d: %d\n", i, ninput_items[i]);
		//}
			
		const char **in = (const char**) &input_items[0]; // in points to the address of the vector of void pointers.
		char **out = (char**) &output_items[0];

		//int count = 0, totalcount = noutput_items/d_hypervisor.get_fft_span();
		//printf("Totalcount: %d \n", totalcount);
		unsigned int acc = 0;
		unsigned int skip = 0;
		bool initialized = false;
		int count = 0, totalcount = noutput_items/d_hypervisor.get_fft_span();
		
		//std::vector<tag_t> tags;
        //get_tags_in_range(tags, 0, 0, noutput_items,
        //                pmt::intern("trigger"));
              
        /*                
        if(tags.size() > 0) {
            if(tags[0].offset > 0) {
               if(tags[0].offset + d_buffer_items == d_hypervisor.get_fft_span()) {
               
                   d_hypervisor.store_input_stream(0, tags[0].offset, (gr_complex*) in[0], d_itemsize);
				    in[0] += d_itemsize*tags[0].offset;
				    consume(0, tags[0].offset);
				    
				    d_hypervisor.work();
			
			        for(unsigned int i=0; i<d_noutputs; i++){
				        d_hypervisor.get_output_stream(i, d_fft_list_out[i].fft_size, (gr_complex*) out[i]);
				        out[i] += d_itemsize*d_fft_list_out[i].fft_size;
				        //printf("Output %d, FFT Size: %d", i, d_fft_list_out[i].fft_size);
				        produce(i, d_fft_list_out[i].fft_size);
			        }
			        
			        d_buffer_items=0;
			        count++;
               }
               else{ 
                in[0] += d_itemsize*tags[0].offset; // drop first samples
                consume(0, d_itemsize*tags[0].offset);
                noutput_items -= tags[0].offset;            
                }
             }
        }
        */
                
		while(count < totalcount) {
			
			//printf("Count: %d \n", count);

			for(unsigned int i=0; i<d_ninputs; i++){
				d_hypervisor.store_input_stream(i, d_fft_list_in[i].fft_size, (gr_complex*) in[i], d_itemsize);
				in[i] += d_itemsize*d_fft_list_in[i].fft_size;
				//printf("Input %d, FFT Size: %d", i, d_fft_list_in[i].fft_size);
				consume(i, d_fft_list_in[i].fft_size);
			}
			
			d_hypervisor.work();
			
			for(unsigned int i=0; i<d_noutputs; i++){
				d_hypervisor.get_output_stream(i, d_fft_list_out[i].fft_size, (gr_complex*) out[i]);
				out[i] += d_itemsize*d_fft_list_out[i].fft_size;
				//printf("Output %d, FFT Size: %d", i, d_fft_list_out[i].fft_size);
				produce(i, d_fft_list_out[i].fft_size);
			}

			count++;

			//memcpy(out[d_current_output] + skip * d_size_bytes, in[d_current_input], d_size_bytes);
			//in[d_current_input] += d_size_bytes;
			//produce(d_current_output, d_blocksize);
			//d_current_input = (d_current_input + 1) % d_ninputs;		
			//d_current_output = (d_current_output + 1) % d_noutputs;

			//d_hypervisor.store_input_stream(d_current_input, d_fft_list_in[d_current_input].fft_size, (gr_complex*) in[d_current_input], d_itemsize);
			
			//memcpy(out[d_current_output], in[d_current_input], d_size_bytes);
			//in[d_current_input] += d_itemsize*d_fft_list_in[d_current_input].fft_size;
			//d_current_input = (d_current_input + 1) % d_ninputs;


			
			// in[d_current_input] += d_size_bytes;
			// accumulate times through the loop; increment skip after a
			// full pass over the input streams.
			// This is separate than d_current_output since we could be in
			// the middle of a loop when we exit.
			//acc++;

			//if(acc == d_ninputs){
			//	acc = 0;
			
				//d_hypervisor.work();
			//	initialized = true;
					
			//}
			
			// Keep track of our loop counter
			
			//	d_hypervisor.get_output_stream(d_current_output, d_fft_list_out[d_current_output].fft_size, (gr_complex*) out[d_current_output]);
			//	produce(d_current_output, d_fft_list_out[d_current_output].fft_size);
			//	out[d_current_output] += d_itemsize*d_fft_list_out[d_current_output].fft_size;
			//	d_current_output = (d_current_output + 1) % d_noutputs;
			//	count += d_fft_list_out[d_current_output].fft_size;
			//}
			
			
		}     
		
		return WORK_CALLED_PRODUCE;
    }

  } /* namespace mysvl */
} /* namespace gr */

