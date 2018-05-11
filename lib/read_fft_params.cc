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

#include "read_fft_params.h"
#include <gnuradio/thread/thread.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/fft/window.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <stdio.h>

bool power_of_two(int x)
{
  bool check = false;

  if (!(x == 0) && !(x & (x - 1))) check = true;
  return check;
}

namespace gr {
  namespace mysvl {
    fft_params::fft_params()
    {
      d_fp      = 0;
      d_new_fp  = 0;
      d_updated = false;
    }

    fft_params::fft_params(const char *filename)
    {
      d_fp      = 0;
      d_new_fp  = 0;
      d_updated = false;
      open(filename);
      do_update();
      read_file();
    }

    fft_params::~fft_params()
    {
      if (d_fp) fclose((FILE *)d_fp);

      if (d_new_fp) fclose((FILE *)d_new_fp);
    }

    void fft_params::update_filename(const char *filename)
    {
      d_fp      = 0;
      d_new_fp  = 0;
      d_updated = false;
      open(filename);
      do_update();
      read_file();
    }

    void
    fft_params::open(const char *filename)
    {
      // obtain exclusive access for duration of this function
      gr::thread::scoped_lock lock(fp_mutex);

      int fd;

      // we use "open" to use to the O_LARGEFILE flag
      if ((fd = ::open(filename, O_RDONLY)) < 0)
      {
        perror(filename);
        throw std::runtime_error("can't open file");
      }

      if (d_new_fp)
      {
        fclose(d_new_fp);
        d_new_fp = 0;
      }

      if ((d_new_fp = fdopen(fd, "rb")) == NULL)
      {
        perror(filename);
        ::close(fd); // don't leak file descriptor if fdopen fails
        throw std::runtime_error("can't open file");
      }

      d_updated = true;
    }

    void
    fft_params::close()
    {
      // obtain exclusive access for duration of this function
      gr::thread::scoped_lock lock(fp_mutex);

      if (d_new_fp != NULL)
      {
        fclose(d_new_fp);
        d_new_fp = NULL;
      }
      d_updated = true;
    }

    void
    fft_params::do_update()
    {
      if (d_updated)
      {
        gr::thread::scoped_lock lock(fp_mutex); // hold while in scope

        if (d_fp) fclose(d_fp);

        d_fp      = d_new_fp; // install new file pointer
        d_new_fp  = 0;
        d_updated = false;
      }
    }

    void
    fft_params::read_file()
    {
      do_update(); // update d_fp is reqd

      if (d_fp == NULL) throw std::runtime_error("work with file not open");

      gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this
                                              // function

      int num = 0;

      fft_list.clear();

      bool input;
      int  index;
      int  fft_size;
      char temp_string[25];

      while (fscanf((FILE *)d_fp, "%d%d%d%s", &num, &index, &fft_size,
                    temp_string) == 4) {
        if ((num > 1) || (num < 0))
        {
          // GR_LOG_ERROR(d_logger, "error with spectrum map file \n");
          perror("fft file error: input not a bool");
          throw std::invalid_argument("error with fft file: input not a bool");
        }
        else input = (bool)num;

        // Remove power of 2 contraint
        // if(!power_of_two(fft_size)) {
        // GR_LOG_ERROR(d_logger, "error with spectrum map file \n");
        // perror("fft file error: fft_size not a power of 2");
        // throw std::invalid_argument("error with fft file: fft_size not a
        // power of 2");
        // }

        fft_parameters temp { input, index, fft_size, convert_window(fft_size,
                                                                     std::string(
                                                                       temp_string)) };
        fft_list.push_back(temp);
      }

      if (ferror((FILE *)d_fp)) throw std::invalid_argument(
                "error reading fft file");
    }

    std::vector<float> fft_params::convert_window(int         fft_size,
                                                  std::string window_type)
    {
      if (window_type == "rectangular") return fft::window::rectangular(fft_size);

      if (window_type ==  "hamming") return fft::window::hamming(fft_size);

      if (window_type ==  "hann") return fft::window::hann(fft_size);

      if (window_type ==  "blackman") return fft::window::blackman(fft_size);

      if (window_type ==  "blackmanharris") return fft::window::blackman_harris(
          fft_size);

      if (window_type == "kaiser") return fft::window::kaiser(fft_size, 0);

      if (window_type == "bartlett") return fft::window::bartlett(fft_size);

      if (window_type ==  "flattop") return fft::window::flattop(fft_size);
      else throw std::out_of_range("fft file: window type out of range");
    }

    std::vector<fft_parameters> fft_params::get_list() const
    {
      return fft_list;
    }
  } /* namespace mysvl */
}   /* namespace gr */
