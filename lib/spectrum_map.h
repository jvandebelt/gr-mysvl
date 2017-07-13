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

#ifndef INCLUDED_SPECTRUM_MAP_H
#define INCLUDED_SPECTRUM_MAP_H

#include <mysvl/svl.h>
#include <boost/thread/mutex.hpp>
#include <string>

namespace gr {
  namespace mysvl {

    class spectrum_map
    {
	private:
		int d_spectrum_span;
		gr_vector_int d_spectrum_map_in;
		gr_vector_int d_spectrum_map_out;
		
		// From file_source_impl.h
		FILE *d_fp;
		FILE *d_new_fp;
		bool d_updated;
		boost::mutex fp_mutex;
		std::string d_filename;

		void do_update();
		

	public:
		spectrum_map();
		spectrum_map(const char *filename);
		~spectrum_map();

		void update_filename(const char* filename);
		std::string get_filename();

		void open(const char *filename);
		void close();
		void read_file();
		int get_size() const;
		gr_vector_int get_spectrum_map_in() const;
		gr_vector_int get_spectrum_map_out() const;

    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_SPECTRUM_MAP_H */

