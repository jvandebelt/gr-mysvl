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

#include <gnuradio/thread/thread.h>
#include "spectrum_map.h"
#include <gnuradio/io_signature.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <stdio.h>

namespace gr {
  namespace mysvl {

	spectrum_map::spectrum_map() {
		d_fp = 0;
		d_new_fp=0;
		d_spectrum_span=0;
		d_updated = false;
    }

	spectrum_map::spectrum_map(const char *filename) {
		d_fp = 0;
		d_new_fp=0;
		d_spectrum_span=0;
		d_updated = false;
		open(filename);
		do_update();
		read_file();
		d_filename=std::string(filename);
    }

    spectrum_map::~spectrum_map() {
      if(d_fp)
        fclose ((FILE*)d_fp);
      if(d_new_fp)
        fclose ((FILE*)d_new_fp);
    }
	
	void spectrum_map::update_filename(const char *filename) {
		d_fp = 0;
		d_new_fp=0;
		d_spectrum_span=0;
		d_updated = false;
		open(filename);
		do_update();
		read_file();
		d_filename=std::string(filename);		
	}
	
	std::string spectrum_map::get_filename() {
        return d_filename;
    }

	void
    spectrum_map::open(const char *filename) {
		// obtain exclusive access for duration of this function
		gr::thread::scoped_lock lock(fp_mutex);

		int fd;

		// we use "open" to use to the O_LARGEFILE flag
		if((fd = ::open(filename, O_RDONLY)) < 0) {
			perror(filename);
			throw std::runtime_error("can't open file");
		}

		if(d_new_fp) {
		fclose(d_new_fp);
		d_new_fp = 0;
		}

		if((d_new_fp = fdopen (fd, "rb")) == NULL) {
			perror(filename);
			::close(fd);	// don't leak file descriptor if fdopen fails
			throw std::runtime_error("can't open file");
		}

		d_updated = true;
	}
	
	void
    spectrum_map::close() {
		// obtain exclusive access for duration of this function
		gr::thread::scoped_lock lock(fp_mutex);

		if(d_new_fp != NULL) {
		fclose(d_new_fp);
		d_new_fp = NULL;
		}
		d_updated = true;
	}

	void
    spectrum_map::do_update() {
		if(d_updated) {
			gr::thread::scoped_lock lock(fp_mutex); // hold while in scope

			if(d_fp)
			fclose(d_fp);

			d_fp = d_new_fp;    // install new file pointer
			d_new_fp = 0;
			d_updated = false;
		}
	}

	void
	spectrum_map::read_file() {
		do_update();       // update d_fp is reqd
		if(d_fp == NULL)
		throw std::runtime_error("work with file not open");

		gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this function
	
		int input_num =0;
		int output_num =0;
		
		/*while(fgets(line, 20, (FILE*)d_fp)!= NULL) {
			d_spectrum_map_in.push_back(atoi(line));
			d_spectrum_span++;
			//printf("%i",d_spectrum_map[d_spectrum_span-1]);
		}
*/      
        d_spectrum_map_in.clear();
        d_spectrum_map_out.clear();
        
		while(fscanf((FILE*)d_fp,"%d%d", &input_num, &output_num) != EOF) {
				d_spectrum_map_in.push_back(input_num);
				d_spectrum_map_out.push_back(output_num);

				d_spectrum_span++;

		}
		if(ferror((FILE*)d_fp))
			throw std::invalid_argument("error reading spectrum map file");
		/*
		if(d_spectrum_span%2 != 0) {
			//GR_LOG_ERROR(d_logger, "error with spectrum map file \n");
			perror("d_spectrum_span%2 != 0");
			throw std::invalid_argument("error with spectrum map file: spectrum span not divisible by two");
		}
		*/
	}
	
	int spectrum_map::get_size() const {
		return d_spectrum_span;
	}

	gr_vector_int spectrum_map::get_spectrum_map_in() const {
		return d_spectrum_map_in;
	}

	gr_vector_int spectrum_map::get_spectrum_map_out() const {
		return d_spectrum_map_out;
	}
	









  } /* namespace mysvl */
} /* namespace gr */

