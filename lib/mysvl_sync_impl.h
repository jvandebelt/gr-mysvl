/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_MYSVL_MYSVL_SYNC_IMPL_H
#define INCLUDED_MYSVL_MYSVL_SYNC_IMPL_H

#include <mysvl/mysvl_sync.h>
#include "hypervisor.h"

namespace gr {
  namespace mysvl {

    class mysvl_sync_impl : public mysvl_sync
    {
     private:
		size_t d_itemsize;
		unsigned int d_blocksize;
		unsigned int d_ninputs;
		unsigned int d_noutputs;
		unsigned int d_current_input;
		unsigned int d_current_output;
		unsigned int d_size_bytes;
		hypervisor d_hypervisor;
		std::vector<fft_parameters> d_fft_list_in;
		std::vector<fft_parameters> d_fft_list_out;

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      mysvl_sync_impl(size_t itemsize, unsigned int blocksize, const char *map_filename, const char *fft_filename, const std::string& lengthtagname);
      ~mysvl_sync_impl();
		
		bool check_topology(int ninputs, int noutputs);

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_MYSVL_MYSVL_SYNC_IMPL_H */

