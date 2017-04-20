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
		d_size_bytes = d_itemsize * d_blocksize;
		set_output_multiple(blocksize);		
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

		set_relative_rate((double)ninputs/(double)noutputs);
		d_factor = boost::math::lcm(d_ninputs, d_noutputs);
		d_hypervisor.create_streams(ninputs, noutputs);
		//d_hypervisor.do_fft_test();
		set_output_multiple(d_blocksize*d_factor);
		if(!d_hypervisor.check_spectrum_map(ninputs, noutputs))
        	throw std::runtime_error("error: inconsistency between configuration and spectrum_map\n");
		return true;
	}

    void
    svl_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      for(unsigned int i = 0; i < ninput_items_required.size(); ++i) {
		 ninput_items_required[i] = (int) ((noutput_items * d_noutputs
		/ ninput_items_required.size()) + .5);
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

		int count = 0, totalcount = noutput_items * d_noutputs;
		unsigned int acc = 0;
		unsigned int skip = 0;
		bool initialized = false;
		
		while(count < totalcount) {

			//memcpy(out[d_current_output] + skip * d_size_bytes, in[d_current_input], d_size_bytes);
			//in[d_current_input] += d_size_bytes;
			//produce(d_current_output, d_blocksize);
			//d_current_input = (d_current_input + 1) % d_ninputs;		
			//d_current_output = (d_current_output + 1) % d_noutputs;

			d_hypervisor.store_input_stream(d_current_input, d_blocksize, (gr_complex*) in[d_current_input], d_itemsize);
			
			//memcpy(out[d_current_output], in[d_current_input], d_size_bytes);
			in[d_current_input] += d_size_bytes;
			d_current_input = (d_current_input + 1) % d_ninputs;

			d_hypervisor.get_output_stream(d_current_output, d_blocksize, (gr_complex*) out[d_current_output]);
			produce(d_current_output, d_blocksize);
			out[d_current_output] += d_size_bytes;
			d_current_output = (d_current_output + 1) % d_noutputs;
			
			// in[d_current_input] += d_size_bytes;
			// accumulate times through the loop; increment skip after a
			// full pass over the input streams.
			// This is separate than d_current_output since we could be in
			// the middle of a loop when we exit.
			acc++;

			if(acc == d_ninputs){
				acc = 0;
			
				d_hypervisor.work();
					
			}

			// Keep track of our loop counter
			count += d_blocksize*d_factor/d_noutputs;

		}

		consume_each((int)((totalcount/d_ninputs) + .5));
		return WORK_CALLED_PRODUCE;
    }

  } /* namespace mysvl */
} /* namespace gr */

