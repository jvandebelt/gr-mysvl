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

#ifndef INCLUDED_STREAM_H
#define INCLUDED_STREAM_H

#include <mysvl/svl.h>
#include <gnuradio/fft/fft.h>

namespace gr {
  namespace mysvl {

    class stream 
    {
	private:

		fft::fft_complex 		*d_fft;
		size_t 					d_fft_size;
		std::vector<float>    	d_window;
		bool 					d_forward;
		float 					d_central_frequency;
		float 					d_bandwidth;	
		int 					d_itemsize;

	public:
		stream(int fft_size, bool forward,
			const std::vector<float> &window, int itemsize,
			int nthreads=1);
		stream();
		~stream();
		
		size_t get_fft_size();
		void print_fft_size();
		void set_nthreads(int n);
		int nthreads() const;
		bool set_window(const std::vector<float> &window);
		int work(std::vector<gr_complex> &input_items,
				std::vector<gr_complex> &output_items); 
    };

	typedef boost::shared_ptr<stream> stream_pointer;
	typedef std::vector<stream_pointer> stream_vector;

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_STREAM_H */

