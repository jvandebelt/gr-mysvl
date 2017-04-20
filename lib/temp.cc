d_hypervisor.store_input_stream(d_current_input, d_blocksize*d_factor/d_ninputs, (gr_complex*) in[d_current_output]);
			d_current_input = (d_current_input + 1) % d_ninputs;
			
			if(initialized) {
				d_hypervisor.get_output_stream(d_current_output, d_blocksize*d_factor/d_noutputs, (gr_complex*) out[d_current_output]);
				produce(d_current_output, d_blocksize*d_factor/d_noutputs);
			
				d_current_output = (d_current_output + 1) % d_noutputs;
			}
				
			// in[d_current_input] += d_size_bytes;
			// accumulate times through the loop; increment skip after a
			// full pass over the input streams.
			// This is separate than d_current_output since we could be in
			// the middle of a loop when we exit.
			acc++;

			if(acc >= d_ninputs){
				for(int i=0; i<d_ninputs; i++)
					in[i] += d_size_bytes*d_factor/d_ninputs;
				acc = 0;
				d_hypervisor.work();

				initialized = true;
					
			}

			// Keep track of our loop counter
			count += d_blocksize*d_factor/d_noutputs;

		}
