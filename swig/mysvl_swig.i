/* -*- c++ -*- */

#define MYSVL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "mysvl_swig_doc.i"

%{
#include "mysvl/weave.h"
#include "mysvl/svl.h"
#include "spectrum_map.h"
#include "read_fft_params.h"
#include "hypervisor.h"
#include "stream.h"
#include <gnuradio/fft/fft.h>
%}

%include "mysvl/weave.h"
GR_SWIG_BLOCK_MAGIC2(mysvl, weave);
%include "mysvl/svl.h"
GR_SWIG_BLOCK_MAGIC2(mysvl, svl);