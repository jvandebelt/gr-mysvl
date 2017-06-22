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

#ifndef INCLUDED_HYPERVISOR_H
#define INCLUDED_HYPERVISOR_H

#include <mysvl/svl.h>
#include "spectrum_map.h"
#include "read_fft_params.h"
#include "stream.h"

namespace gr {
  namespace mysvl {

    class hypervisor
    {
	private:
/*
		size_t d_itemsize;
		unsigned int d_blocksize;
		unsigned int d_ninputs;
		unsigned int d_noutputs;
		unsigned int d_current_input;
		unsigned int d_current_output;
		unsigned int d_size_bytes;*/
		
		int d_itemsize;
		int d_fft_span;
		spectrum_map d_map;
		fft_params d_fft_params;
		stream_vector d_streams_in;
		stream_vector d_streams_out;
		// Vector of vector of complex samples
		std::vector<std::vector<gr_complex> > d_items_in;
		std::vector<std::vector<gr_complex> > d_items_out;
		std::vector<std::vector<gr_complex> > d_frequency_domain_items;
		
		//stream d_test_stream;

	public:
		hypervisor();
		hypervisor(const char *map_filename, const char *fft_filename, int itemsize);
		~hypervisor();
		void print_spectrum_map();
		std::vector<fft_parameters> get_fft_list();
		int get_fft_span();
		void print_complex_samples(int input);
		bool check_spectrum_map(int ninputs, int noutputs);
		void create_streams(int ninputs, int noutputs);
		void work();
		void do_fft_test();
		void store_input_stream(int input, unsigned int ninput_items, const gr_complex* in, size_t itemsize);
		void get_output_stream(int output, unsigned int noutput_items,  gr_complex* out);

    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_HYPERVISOR_H */

