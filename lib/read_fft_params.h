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

#ifndef INCLUDED_READ_FFT_PARAMS_H
#define INCLUDED_READ_FFT_PARAMS_H

#include <mysvl/svl.h>
#include <boost/thread/mutex.hpp>
#include <cstring>

struct fft_parameters {
	bool input;
	int index;
	int fft_size;
	std::vector<float> window;
};

namespace gr {
  namespace mysvl {

    class fft_params
    {
	private:
		//gr_vector_int d_spectrum_map_in;
		//gr_vector_int d_spectrum_map_out;
		
		// From file_source_impl.h
		FILE *d_fp;
		FILE *d_new_fp;
		bool d_updated;
		boost::mutex fp_mutex;

		std::vector<fft_parameters> fft_list;

		void do_update();
		

	public:
		fft_params();
		fft_params(const char *filename);
		~fft_params();

		void update_filename(const char* filename);

		void open(const char *filename);
		void close();
		void read_file();
		std::vector<float> convert_window(int fft_size, std::string window_type);
		std::vector<fft_parameters> get_list() const;
		//gr_vector_int get_spectrum_map_out() const;

    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_READ_FFT_PARAMS_H */

