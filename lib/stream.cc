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

#include "stream.h"
#include <math.h>
#include <string.h>
#include <volk/volk.h>
#include <cstdio>


namespace gr {
  namespace mysvl {


    /*
     * The private constructor
     */
	stream::stream() {
		d_itemsize = 0;
	};

    stream::stream(int fft_size, bool forward,
			const std::vector<float> &window, int itemsize,
			int nthreads) : d_fft_size(fft_size), d_forward(forward) {
		d_itemsize = itemsize;
		d_fft = new fft::fft_complex(d_fft_size, d_forward, 1);
		if(!set_window(window))
        	throw std::runtime_error("error: window not the same length as fft_size\n");
	}

    /*
     * Our virtual destructor.
     */
    stream::~stream() {
		delete d_fft;		
    }

	void stream::set_nthreads(int n) {

		d_fft->set_nthreads(n);
	}

	int stream::nthreads() const {

		return d_fft->nthreads();
	}

	bool stream::set_window(const std::vector<float> &window) {

		if(window.size()==0 || window.size()==d_fft_size) {
			d_window=window;
			return true;
		}
		else
			return false;
	}

	void stream::print_fft_size() {
		
		printf("FFT Size: %d", d_fft_size);
	}

	size_t stream::get_fft_size() {
		
		return d_fft_size;
	}
	
	int
	stream::work(std::vector<gr_complex> &input_items,
               std::vector<gr_complex> &output_items)
	{
		// Each element in the vector void star is a complex sample.

		unsigned int input_data_size = d_itemsize;
		unsigned int output_data_size = d_itemsize;
		
		bool shift = true;
		int count = 0;

		// copy input into optimally aligned buffer
			if(d_window.size()) {
				gr_complex *dst = d_fft->get_inbuf();
				if(!d_forward && shift) {
					unsigned int offset = (!d_forward)?(d_fft_size/2):0;
					int fft_m_offset = d_fft_size - offset;
					volk_32fc_32f_multiply_32fc(&dst[fft_m_offset], &input_items[0], &d_window[0], offset);
					volk_32fc_32f_multiply_32fc(&dst[0], &input_items[offset], &d_window[offset], d_fft_size-offset);
				}
				else {
					volk_32fc_32f_multiply_32fc(&dst[0], &input_items[0], &d_window[0], d_fft_size);
				}
			}
			else {
				if(!d_forward && shift) {  // apply an ifft shift on the data
					gr_complex *dst = d_fft->get_inbuf();
					unsigned int len = (unsigned int)(floor(d_fft_size/2.0)); // half length of complex array
					memcpy(&dst[0], &input_items[len], sizeof(gr_complex)*(d_fft_size - len));
					memcpy(&dst[d_fft_size - len], &input_items[0], sizeof(gr_complex)*len);
				}
				else {
					memcpy(d_fft->get_inbuf(), &input_items[0], input_data_size*d_fft_size);
				}
			}
			// compute the fft
			d_fft->execute();
			
			// copy result to our output
			if(d_forward && shift) {  // apply an fft shift on the data
				unsigned int len = (unsigned int)(ceil(d_fft_size/2.0));
				memcpy(&output_items[0], &d_fft->get_outbuf()[len], sizeof(gr_complex)*(d_fft_size - len));
				memcpy(&output_items[d_fft_size - len], &d_fft->get_outbuf()[0], sizeof(gr_complex)*len);

			}
			else {
				memcpy (&output_items[0], d_fft->get_outbuf(), output_data_size*d_fft_size);
			}

	}


	
  } /* namespace mysvl */
} /* namespace gr */

