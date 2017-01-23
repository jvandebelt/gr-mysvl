#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import mysvl_swig as mysvl

class qa_weave (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        src_data_0 = (1, 2, 3, 4)
        src_data_1 = (5, 6, 7, 8)
        expected_result = (1, 5, 2, 6, 3, 7, 4, 8)
        src0 = blocks.vector_source_f(src_data_0)
        src1 = blocks.vector_source_f(src_data_1)
        op = mysvl.weave(gr.sizeof_float, 1)
        dst = blocks.vector_sink_f()
        self.tb.connect(src0, (op, 0))
        self.tb.connect(src1, (op, 1))
        self.tb.connect(op, dst)
        self.tb.run()
        result_data = dst.data()
        #print(dst.data())
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

    def test_002_t (self):
        src_data_0 = (1, 2, 3, 4)
        src_data_1 = (5, 6, 7, 8)
        expected_result = (1, 2, 5, 6, 3, 4, 7, 8)
        src0 = blocks.vector_source_f(src_data_0)
        src1 = blocks.vector_source_f(src_data_1)
        op = mysvl.weave(gr.sizeof_float, 2)
        dst = blocks.vector_sink_f()
        self.tb.connect(src0, (op, 0))
        self.tb.connect(src1, (op, 1))
        self.tb.connect(op, dst)
        self.tb.run()
        result_data = dst.data()
        #print(dst.data())
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)



if __name__ == '__main__':
    gr_unittest.run(qa_weave, "qa_weave.xml")
