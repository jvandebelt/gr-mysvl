/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

	public:
		hypervisor(const char *filename);
		~hypervisor();

    };

  } // namespace mysvl
} // namespace gr

#endif /* INCLUDED_HYPERVISOR_H */

