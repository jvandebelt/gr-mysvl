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
#include "weave_impl.h"

namespace gr {
  namespace mysvl {

    weave::sptr
    weave::make(size_t itemsize, unsigned int blocksize)
    {
      return gnuradio::get_initial_sptr
        (new weave_impl(itemsize, blocksize));
    }

    /*
     * The private constructor
     */
    weave_impl::weave_impl(size_t itemsize, unsigned int blocksize)
      : gr::block("weave",
              gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize),
              gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize)), 
					d_itemsize(itemsize), d_blocksize(blocksize), d_current_input(0), d_current_output(0)
    {
		d_size_bytes = d_itemsize * d_blocksize;
		set_output_multiple(blocksize);		
	}

    /*
     * Our virtual destructor.
     */
    weave_impl::~weave_impl()
    {
    }

	bool
	weave_impl::check_topology(int ninputs, int noutputs)
	{
		set_relative_rate((double)ninputs/(double)noutputs);
		d_ninputs = ninputs;
		d_noutputs = noutputs;
		set_output_multiple(d_blocksize * boost::math::lcm(d_ninputs, d_noutputs));
		return true;
	}

    void
    weave_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
		for(unsigned int i = 0; i < ninput_items_required.size(); ++i) {
			 ninput_items_required[i] = (int) ((noutput_items * d_noutputs
			/ ninput_items_required.size()) + .5);
		}
    }		

    int
    weave_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
			
		const char **in = (const char**) &input_items[0];
		char **out = (char**) &output_items[0];

		int count = 0, totalcount = noutput_items * d_noutputs;
		unsigned int skip = 0;
		unsigned int acc = 0;

		while(count < totalcount) {
			memcpy(out[d_current_output] + skip * d_size_bytes, in[d_current_input], d_size_bytes);
			in[d_current_input] += d_size_bytes;
			produce(d_current_output, d_blocksize);
			d_current_input = (d_current_input + 1) % d_ninputs;		
			d_current_output = (d_current_output + 1) % d_noutputs;

			// accumulate times through the loop; increment skip after a
			// full pass over the output streams.
			// This is separate than d_current_output since we could be in
			// the middle of a loop when we exit.
			acc++;

			if(acc >= d_noutputs) {
				skip++;
				acc = 0;
			}

			// Keep track of our loop counter
			count += d_blocksize;

		}

		consume_each((int)((totalcount/d_ninputs) + .5));
		return WORK_CALLED_PRODUCE;
    }

  } /* namespace mysvl */
} /* namespace gr */

