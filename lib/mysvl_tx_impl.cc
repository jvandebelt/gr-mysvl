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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <boost/integer/common_factor_rt.hpp>
#include "mysvl_tx_impl.h"
#include <cstdio>

 //TODO CHANGE PACKET LENGTH + PADDING TO BE PER OUTPUT. CURRENTLY GLOBAL

namespace gr {
  namespace mysvl {

    mysvl_tx::sptr
    mysvl_tx::make(size_t itemsize, unsigned int blocksize, const char *map_filename,
                   const char *fft_filename, std::vector<unsigned int> packet_size, unsigned int padding_size)
    {
      return gnuradio::get_initial_sptr
        (new mysvl_tx_impl(itemsize, blocksize, map_filename, fft_filename, packet_size, padding_size));
    }

    /*
     * The private constructor
     */
    mysvl_tx_impl::mysvl_tx_impl(size_t itemsize, unsigned int blocksize, const char *map_filename,
                                 const char *fft_filename, std::vector<unsigned int> packet_size, unsigned int padding_size)
      : gr::block("mysvl_tx",
             gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize),
             gr::io_signature::make(1, io_signature::IO_INFINITE, itemsize)),
             d_itemsize(itemsize), d_blocksize(blocksize), d_current_input(0),
        d_current_output(0),
        d_hypervisor(map_filename, fft_filename, itemsize),
        d_packet_size(packet_size),
        d_padding_size(padding_size)
    {
    	// check blocksize
      std::vector<fft_parameters> d_fft_list = d_hypervisor.get_fft_list();
      int smallest_fft_size                  = d_fft_list[0].fft_size;

      set_tag_propagation_policy(TPP_DONT);

      d_fft_list_in.clear();
      d_fft_list_out.clear();
      /*d_data= {{0.125000,0.000000}, {0.522104,-0.148216}, {-0.495528,0.114832}, {-0.267916,0.091700},
      			{0.236544,-0.138456}, {-0.098500,0.473800}, {0.476480,-0.225344}, {-0.187516,0.035372},
      			{0.051776,-0.353552}, {-0.104936,0.059916},  {0.228684,0.117504}, {-0.530912,0.560756},
      			{0.359128,0.015872}, {-0.132852,0.632840}, {-0.105164,-0.368872}, {0.368272,-0.032412},
      			{0.125000,0.750000}, {0.463968,0.457792}, {0.151476,-0.430948}, {0.685052,0.238524},
      			{0.494428,0.119428}, {-0.557540,-0.050056}, {0.416348,0.017368}, {0.104256,-0.568836},
      			{-0.301776,-0.353552}, {0.079812,0.451516}, {0.439152,0.528072}, {0.642060,0.178484},
      			{-0.090096,0.465096}, {-0.446492,0.305776}, {-0.111440,-0.093688}, {-0.538848,-0.320228},
      			{0.125000,0.000000}, {-0.538848,0.320228}, {-0.111440,0.093688}, {-0.446492,-0.305776},
      			{-0.090096,-0.465096}, {0.642060,-0.178484}, {0.439152,-0.528072}, {0.079812,-0.451516},
      			{-0.301776,0.353552}, {0.104256,0.568836}, {0.416348,-0.017368}, {-0.557540,0.050056},
      			{0.494428,-0.119428}, {0.685052,-0.238524}, {0.151476,0.430948}, {0.463968,-0.457792},
      			{0.125000,-0.750000}, {0.368272,0.032412}, {-0.105164,0.368872}, {-0.132852,-0.632840},
      			{0.359128,-0.015872}, {-0.530912,-0.560756}, {0.228684,-0.117504}, {-0.104936,-0.059916},
      			{0.051776,0.353552}, {-0.187516,-0.035372}, {0.476480,0.225344}, {-0.098500,-0.473800},
      			{0.236544,0.138456}, {-0.267916,-0.091700}, {-0.495528,-0.114832}, {0.522104,0.148216}};
      			*/

      	d_data={{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0},
      			{0, 0}, {0,0}, {0,0}, {0,0}, {0, 0}, {0,0}, {0,0}, {0,0}};
        // d_buffer_input=0;
        // d_buffer_remaining=0;

        for (int i = 0; i < d_fft_list.size(); i++) {
                                                     if (d_fft_list[i].fft_size <
            smallest_fft_size) smallest_fft_size = d_fft_list[i].fft_size;

        if (d_fft_list[i].input) d_fft_list_in.push_back(d_fft_list[i]);
        else
          d_fft_list_out.push_back(d_fft_list[i]);
        }

        if(d_packet_size.size()!=d_fft_list_out.size())
          throw std::invalid_argument("Enter the correct number of packet sizes (one per output)");


        for(int i; i < d_fft_list_out.size(); i++) {
          if(d_packet_size[i]%d_fft_list_out[i].fft_size!=0)
            throw std::invalid_argument("Packet Length should be a multiple of FFT size");
        }

      set_output_multiple(d_hypervisor.get_fft_span());

      // d_hypervisor.print_spectrum_map();
    }

    /*
     * Our virtual destructor.
     */
    mysvl_tx_impl::~mysvl_tx_impl()
    {
    }

	bool
    mysvl_tx_impl::check_topology(int ninputs, int noutputs)
    {
      d_ninputs  = ninputs;
      d_noutputs = noutputs;
      d_residual.clear();

      for (int i = 0; i <  noutputs;i++){
            d_residual.push_back(d_packet_size[i]);
           }
      d_hypervisor.create_streams(ninputs, noutputs);

      set_output_multiple(d_hypervisor.get_fft_span());

      return true;
    }

    void
    mysvl_tx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      // GR_LOG_INFO(d_logger, "MySVL");
      // printf("Output items %d \n", noutput_items);
      // printf("Updated output items %d \n", noutput_items);

       int count=noutput_items/d_hypervisor.get_fft_span();
       for(int i=0; i<d_residual.size(); i++) {
         int stream_output_items = count*d_fft_list_out[i].fft_size;
         int number_of_preambles = (d_residual[i]%d_packet_size[i]+stream_output_items)/d_packet_size[i];
           noutput_items-= number_of_preambles*(d_data.size()+d_padding_size*2);
       }


      for (unsigned int i = 0; i < ninput_items_required.size(); ++i) {
        ninput_items_required[i] =
          (int)(noutput_items * d_fft_list_in[i].fft_size /
                d_hypervisor.get_fft_span()  + .5);

        //  printf("Number of items required for input %d is %d \n", i,
        //      ninput_items_required[i]);
      }
    }

    int
    mysvl_tx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char **in = (const char **)&input_items[0]; // in points to the
                                                        // address of the vector
                                                        // of void pointers.
      char **out = (char **)&output_items[0];

      int totalcount=noutput_items/d_hypervisor.get_fft_span();

      for(int i=0; i<d_residual.size(); i++) {
        //printf("d_residual[%d] at start of general work: %d \n", i, d_residual[i]);
        int stream_output_items = totalcount*d_fft_list_out[i].fft_size;
        int number_of_preambles = (d_residual[i]%d_packet_size[i]+stream_output_items)/d_packet_size[i];
        noutput_items-= number_of_preambles*(d_data.size()+d_padding_size*2);
    }

      // int count = 0, totalcount = noutput_items/d_hypervisor.get_fft_span();
      // printf("Totalcount: %d \n", totalcount);
      unsigned int acc = 0;
      unsigned int skip = 0;
      int ntags = 0;
      int count = 0;

      std::vector<tag_t> tags;

      get_tags_in_range(tags, 0, 0, noutput_items, pmt::intern("trigger"));

      if (tags.size() > 0)
      {
        // printf("tag offset/fft span:
        // %d\n",tags[0].offset%d_hypervisor.get_fft_span());
        if (tags[0].offset % d_hypervisor.get_fft_span() != 0)
        {
          printf("Unsynchronized tag. Dropping %lu samples\n", tags[0].offset);
          consume(0, tags[0].offset);
          set_history(ninput_items[0] - tags[0].offset);
          return 0;
        }
      }

      while (count < totalcount) {
        for (unsigned int i = 0; i < d_ninputs; i++) {
          d_hypervisor.store_input_stream(i, d_fft_list_in[i].fft_size,
                                          (gr_complex *)in[i], d_itemsize);
          in[i] += d_itemsize * d_fft_list_in[i].fft_size;

          // printf("Input %d, FFT Size: %d", i, d_fft_list_in[i].fft_size);
          consume(i, d_fft_list_in[i].fft_size);
        }

        d_hypervisor.work();

        for (unsigned int i = 0; i < d_noutputs; i++) {
          //printf("d_residual[%d] in output: %d \n", i, d_residual[i]);

          d_hypervisor.get_output_stream(i, d_fft_list_out[i].fft_size,
                                         (gr_complex *)out[i]);
          out[i] += d_itemsize * d_fft_list_out[i].fft_size;

          // printf("Output %d, FFT Size: %d", i, d_fft_list_out[i].fft_size);
          produce(i, d_fft_list_out[i].fft_size);
          d_residual[i]-= d_fft_list_out[i].fft_size;

          if(d_residual[i]==0) {
            // Add preamble + padding
            memcpy(out[i], (const void*)&d_data[0], d_data.size()*d_itemsize);
            out[i]+= d_itemsize*d_data.size();
            produce(i, d_data.size());
            d_residual[i]=d_packet_size[i];
          }
        }

        count++;
      }

      return WORK_CALLED_PRODUCE;
    }

  } /* namespace mysvl */
} /* namespace gr */

