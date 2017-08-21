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


#ifndef INCLUDED_MYSVL_MYSVL_SYNC_H
#define INCLUDED_MYSVL_MYSVL_SYNC_H

#include <mysvl/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace mysvl {

    /*!
     * \brief <+description of block+>
     * \ingroup mysvl
     *
     */
    class MYSVL_API mysvl_sync : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<mysvl_sync> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mysvl::mysvl_sync.
       *
       * To avoid accidental use of raw pointers, mysvl::mysvl_sync's
       * constructor is in a private implementation
       * class. mysvl::mysvl_sync::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, unsigned int blocksize, const char *map_filename, const char *fft_filename, const std::string& lengthtagname, unsigned int packet_size);
    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_MYSVL_MYSVL_SYNC_H */

