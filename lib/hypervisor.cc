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

#include "hypervisor.h"
#include <gnuradio/io_signature.h>
#include <cstdio>

namespace gr {
  namespace mysvl {


    /*
     * The private constructor
     */
    hypervisor::hypervisor(const char *map_filename, const char *fft_filename, int itemsize) : d_map(map_filename), d_itemsize(itemsize), d_fft_params(fft_filename) {

		std::vector<fft_parameters>  fft_list = d_fft_params.get_list();

		int input_fft_size=0;
		int output_fft_size=0;

		for(unsigned int i=0; i<fft_list.size(); i++) {
			if(fft_list[i].input)
				input_fft_size+=fft_list[i].fft_size;
			else
				output_fft_size+=fft_list[i].fft_size;
		}
		if(input_fft_size!=output_fft_size)
			throw std::invalid_argument("error: inputs and outputs don't match fft file");
		d_fft_span=input_fft_size; 
	}

// , d_test_stream(64, true, window, itemsize)
	hypervisor::hypervisor() : d_map() {//, d_test_stream()
		
	}

    /*
     * Our virtual destructor.
     */
    hypervisor::~hypervisor() {
		
    }

	void hypervisor::create_streams(int ninputs, int noutputs) {
		
		//const std::vector<float> d_window(0);
		
		std::vector<fft_parameters>  fft_list = d_fft_params.get_list();
		
		int fft_inputs = 0;
		int fft_outputs = 0;

		d_streams_in.clear();
		d_streams_out.clear();
		d_items_in.clear();	
		d_items_out.clear();

		for(unsigned int i=0; i<fft_list.size(); i++) {

			if(fft_list[i].input){ 
				d_streams_in.emplace_back(new stream(fft_list[i].fft_size, true, fft_list[i].window, d_itemsize));
				d_items_in.resize(d_items_in.size() + 1);
				d_items_in[fft_inputs].resize(fft_list[i].fft_size);
				fft_inputs++;
			}
			else {
				d_streams_out.emplace_back(new stream(fft_list[i].fft_size, false, fft_list[i].window, d_itemsize));
				d_items_out.resize(d_items_out.size() + 1);
				d_items_out[fft_outputs].resize(fft_list[i].fft_size);
				fft_outputs++;
			}
		}

		// check if we have fft_params for all outputs and inputs
		if(!(fft_inputs==ninputs && fft_outputs==noutputs)){
			throw std::invalid_argument("error: inputs and outputs don't match fft file"); 
		}
		
		if(!check_spectrum_map(ninputs, noutputs))
        	throw std::runtime_error("error: inconsistency between configuration and spectrum_map\n");
		//printf("Size of d_streams_in: %d \n", d_streams_in.size());

		//for(int i=0; i<noutputs; i++) {
		//	d_streams_out.emplace_back(new stream(64, false, d_window, d_itemsize)); //still need to set the fft size correctly manually
		//	d_items_out.resize(d_items_out.size() + 1);
		//}
	}
	
	std::vector<fft_parameters> hypervisor::get_fft_list(){
		return d_fft_params.get_list();
	}

	int hypervisor::get_fft_span(){
		return d_fft_span;
	}
	
	void hypervisor::store_input_stream(int input, unsigned int ninput_items, const gr_complex* in, size_t itemsize) {
		
		if(d_items_in[input].size() <= d_items_in[input].size() + ninput_items)
			d_items_in[input].resize(d_items_in[input].size() + ninput_items);

		d_items_in[input].insert(d_items_in[input].end(), &in[0], &in[ninput_items*itemsize]);
		
		std::copy(&in[0], &in[ninput_items*itemsize], d_items_in[input].begin());
		//d_items_out[input].resize(ninput_items); // this line is making it work somehow
		//printf("Size of d_items[in]: %d \n", d_items_in[input].size());
		
	}

	void hypervisor::get_output_stream(int output, unsigned int noutput_items, gr_complex* out) {
	
		std::copy(d_items_out[output].end()-noutput_items, d_items_out[output].end(), out);
		//d_items_out[output].erase(d_items_out[output].end()-noutput_items, d_items_out[output].end()); 
	
	}

	void hypervisor::work() {
		
		gr_vector_int input_map = d_map.get_spectrum_map_in();
		gr_vector_int output_map= d_map.get_spectrum_map_out();

		// convert each input stream to frequency domain
		for(unsigned int input=0; input<d_streams_in.size(); input++) {

            float fft_size = (float) d_streams_in[input]->get_fft_size();
            
			d_streams_in[input]->work(d_items_in[input], d_items_in[input]);
		
			// normalise fft - only need to do this once
			for(unsigned int i=0; i<d_items_in[input].size(); i++) {
				d_items_in[input][i].real(d_items_in[input][i].real()/fft_size);
				d_items_in[input][i].imag(d_items_in[input][i].imag()/fft_size);
			}	
		}
		
		// do spectrum mapping

		gr_vector_int track_input_locations;
		gr_vector_int track_output_locations;

		for(unsigned int input=0; input<d_streams_in.size(); input++)
			track_input_locations.push_back(0);

		for(unsigned int output=0; output<d_streams_out.size(); output++)
			track_output_locations.push_back(0);
		
		for(unsigned int i=0; i<input_map.size(); i++) {
			d_items_out[output_map[i]-1][track_output_locations[output_map[i]-1]]=d_items_in[input_map[i]-1][track_input_locations[input_map[i]-1]];
			track_output_locations[output_map[i]-1]++;
			track_input_locations[input_map[i]-1]++;
		}

		//
		// temp placeholder - only works if ninputs == noutputs!
		//for(int output=0; output<d_items_out.size(); output++) {
		//	for(int j=0; j<d_items_out[output].size(); j++)
		//		d_items_out[output][j] = d_items_in[output][j];
		//}		
	

		// convert each output stream to time domain
		for(unsigned int output=0; output<d_streams_out.size(); output++) {
			d_streams_out[output]->work(d_items_out[output], d_items_out[output]);
		}
		for(unsigned int input=0; input<d_items_in.size(); input++) {
		    d_items_in[input].clear();
		}
		
	}

	void hypervisor::print_complex_samples(int input) {

		// Print out is working.
		for(unsigned int i=0; i<d_items_in[input].size(); i++) {
			printf("Complex number: %f, %f \n", d_items_in[input][i].real(), d_items_in[input][i].imag());
		}
		
	}

	void hypervisor::print_spectrum_map() {
		
		printf("Spectrum maps \n");
		for(unsigned int i=0; i<d_map.get_size(); i++) {
			printf("in: %d ", d_map.get_spectrum_map_in()[i]);
			printf(" out: %d \n", d_map.get_spectrum_map_out()[i]);
		}
	}

	bool hypervisor::check_spectrum_map(int ninputs, int noutputs) {
		
		// find the number of different values and the highest value in spectrum in and out
		// check that they match nin and nout
			
		gr_vector_int input_map = d_map.get_spectrum_map_in();
		gr_vector_int output_map= d_map.get_spectrum_map_out();

		std::vector<fft_parameters>  fft_list = d_fft_params.get_list();
		
		// Check that input span equals output span. Does this have to be true?
		if(input_map.size()!=output_map.size())
			throw std::invalid_argument("spectrum map error: different input and output sizes"); 
		
		// Check that inputs have the same number of samples as fft_size
		for(unsigned int i=0; i < ninputs; i++){
			int occurances = 0;
			for(unsigned int j=0;j<input_map.size(); j++)
				if(input_map[j]==i+1)
					occurances++;
			for(unsigned int k=0; k<fft_list.size(); k++)
				if(fft_list[k].index == i+1 && fft_list[k].input && occurances!= fft_list[k].fft_size)
					throw std::invalid_argument("error with spectrum map " + d_map.get_filename() + ": number of samples for input " + std::to_string(fft_list[k].index)  +" does not match fft file");
		}
	
	    
		// Check that outputs have the same number of samples as fft_size
		for(unsigned int i=0; i < noutputs; i++){
			int occurances = 0;
			for(unsigned int j=0;j<output_map.size(); j++)
				if(output_map[j]==i+1)
					occurances++;
			for(unsigned int k=0; k<fft_list.size(); k++)
				if(fft_list[k].index == i+1 && !fft_list[k].input && occurances!= fft_list[k].fft_size)
					throw std::invalid_argument("error with spectrum map " + d_map.get_filename() + ": number of samples for output " + std::to_string(fft_list[k].index)  +" does not match fft file");
		}

		return true;
	}
	

		// Test to make sure FFT is working
	void hypervisor::do_fft_test() {

		std::vector<std::vector < gr_complex > > inputs;
		std::vector<std::vector < gr_complex > > outputs;
		
		stream_vector in;
		stream_vector out;		

		//stream s(64, true, d_window, d_itemsize);
		//stream s2(64, false, d_window, d_itemsize);
		

		//in.emplace_back(new stream(256, true, d_window, d_itemsize));
		//out.emplace_back(new stream(256, false, d_window, d_itemsize));
		

		inputs.resize(1);
		for(unsigned int i=0; i<64; i++) {
			inputs[0].push_back(gr_complex(10.0, 10.0));
		}
		
		printf("Inputs \n");
		for(unsigned int i=0; i<inputs[0].size(); i++) {
			//const gr_complex *in= (const gr_complex *) &inputs[0][i];
			printf("Complex number: %f, %f \n", inputs[0][i].real(), inputs[0][i].imag());
		}		
		
		outputs.resize(1);
		outputs[0].resize(64);
				
		// Something going wrong with a stream of vectors. Memory issues??
		in[0]->work(inputs[0], outputs[0]);

		for(unsigned int i=0; i<outputs[0].size(); i++) {
			outputs[0][i].real(outputs[0][i].real()/256.0);
			outputs[0][i].imag(outputs[0][i].imag()/256.0);

		}
		
		printf("Intermediate \n");
		for(unsigned int i=0; i<outputs[0].size(); i++) {
			//gr_complex *out= (gr_complex *) &outputs[0][i];
			printf("Complex number: %f, %f \n", outputs[0][i].real(), outputs[0][i].imag());
		}

		inputs[0] = outputs[0];
	
		out[0]->work(inputs[0], outputs[0]);
		
		

		printf("Outputs \n");
		for(unsigned int i=0; i<outputs[0].size(); i++) {
			//gr_complex *out= (gr_complex *) &outputs[0][i];
			printf("Complex number: %f, %f \n", outputs[0][i].real(), outputs[0][i].imag());
		}
		


	}
	
  } /* namespace mysvl */
} /* namespace gr */

