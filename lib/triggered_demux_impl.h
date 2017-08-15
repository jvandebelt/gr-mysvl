/* -*- c++ -*- */
/* 
 * Copyright 2017 Jonathan van de Belt.
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

#ifndef INCLUDED_MYSVL_TRIGGERED_DEMUX_IMPL_H
#define INCLUDED_MYSVL_TRIGGERED_DEMUX_IMPL_H

#include <mysvl/triggered_demux.h>

namespace gr {
  namespace mysvl {

    class triggered_demux_impl : public triggered_demux
    {
     private:
      size_t d_itemsize;
      size_t d_trigger_size; 
      unsigned int d_stream;    // index of currently selected stream
      int d_residual;           // number if items left to put into current stream
      gr_vector_int d_lengths;  // number if items to pack per stream
      int d_trigger_delay;      // add a delay between the trigger point and the sync point
      int d_offset;
      bool d_reset;
      int d_iteration;
      int d_delay_left;
      float d_last_sample;
      
      int find_trigger_offset(int start, int end, const float *trigger_signal);
      
     public:
      triggered_demux_impl(size_t itemsize, size_t trigger_size, const std::vector<int> &lengths, int trigger_delay);
      ~triggered_demux_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_MYSVL_TRIGGERED_DEMUX_IMPL_H */

