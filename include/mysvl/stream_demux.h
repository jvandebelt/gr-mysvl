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


#ifndef INCLUDED_MYSVL_STREAM_DEMUX_H
#define INCLUDED_MYSVL_STREAM_DEMUX_H

#include <mysvl/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mysvl {

    /*!
     * \brief <+description of block+>
     * \ingroup mysvl
     *
     */
    class MYSVL_API stream_demux : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<stream_demux> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mysvl::stream_demux.
       *
       * To avoid accidental use of raw pointers, mysvl::stream_demux's
       * constructor is in a private implementation
       * class. mysvl::stream_demux::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, const std::vector<int> &lengths, bool add_tags);
    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_MYSVL_STREAM_DEMUX_H */

