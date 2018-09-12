/* -*- c++ -*- */
/*
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty MERCHANTABILITY
 * of or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_MYSVL_MYSVL_TX_IMPL_H
#define INCLUDED_MYSVL_MYSVL_TX_IMPL_H

#include <mysvl/mysvl_tx.h>
#include "hypervisor.h"

namespace gr {
  namespace mysvl {

    class mysvl_tx_impl : public mysvl_tx
    {
     private:
     	size_t d_itemsize;
      unsigned int d_blocksize;
      unsigned int d_ninputs;
      unsigned int d_noutputs;
      unsigned int d_current_input;
      unsigned int d_current_output;
      unsigned int d_size_bytes;
      unsigned int d_padding_size;
      std::vector<unsigned int> d_packet_size;
      std::vector<gr_complex> d_data;
      std::vector<unsigned int> d_residual;
      hypervisor d_hypervisor;
      std::vector<fft_parameters> d_fft_list_in;
      std::vector<fft_parameters> d_fft_list_out;

     public:
      mysvl_tx_impl(size_t itemsize, unsigned int blocksize, const char *map_filename, const char *fft_filename, std::vector<unsigned int> packet_size, unsigned int padding_size);
      ~mysvl_tx_impl();

      bool check_topology(int ninputs, int noutputs);

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_MYSVL_MYSVL_TX_IMPL_H */

