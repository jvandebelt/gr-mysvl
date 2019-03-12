#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Tue Mar 12 13:00:27 2019
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import channels
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from grc_gnuradio import blks2 as grc_blks2
from optparse import OptionParser
import mysvl
import relative_paths  # embedded python module
import sip
import sys
import time


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.freq = freq = 2375e6
        self.udp_len = udp_len = 1472*16
        self.time_preamble = time_preamble =  [0.125000+0.000000j, 0.522104-0.148216j, -0.495528+0.114832j, -0.267916+0.091700j, 0.236544-0.138456j, -0.098500+0.473800j, 0.476480-0.225344j, -0.187516+0.035372j, 0.051776-0.353552j, -0.104936+0.059916j,  0.228684+0.117504j, -0.530912+0.560756j, 0.359128+0.015872j, -0.132852+0.632840j, -0.105164-0.368872j, 0.368272-0.032412j, 0.125000+0.750000j, 0.463968+0.457792j, 0.151476-0.430948j, 0.685052+0.238524j, 0.494428+0.119428j, -0.557540-0.050056j, 0.416348+0.017368j, 0.104256-0.568836j, -0.301776-0.353552j, 0.079812+0.451516j, 0.439152+0.528072j, 0.642060+0.178484j, -0.090096+0.465096j, -0.446492+0.305776j, -0.111440-0.093688j, -0.538848-0.320228j, 0.125000+0.000000j, -0.538848+0.320228j, -0.111440+0.093688j, -0.446492-0.305776j, -0.090096-0.465096j, 0.642060-0.178484j, 0.439152-0.528072j, 0.079812-0.451516j, -0.301776+0.353552j, 0.104256+0.568836j, 0.416348-0.017368j, -0.557540+0.050056j, 0.494428-0.119428j, 0.685052-0.238524j, 0.151476+0.430948j, 0.463968-0.457792j, 0.125000-0.750000j, 0.368272+0.032412j, -0.105164+0.368872j, -0.132852-0.632840j, 0.359128-0.015872j, -0.530912-0.560756j, 0.228684-0.117504j, -0.104936-0.059916j, 0.051776+0.353552j, -0.187516-0.035372j, 0.476480+0.225344j, -0.098500-0.473800j, 0.236544+0.138456j, -0.267916-0.091700j, -0.495528-0.114832j, 0.522104+0.148216j]
        self.time_offset2 = time_offset2 = 1
        self.time_offset = time_offset = 1
        self.samp_rate = samp_rate = 500000
        self.preamble_len = preamble_len = 64
        self.packet_len = packet_len = 1024*4
        self.number_len = number_len = 6*4*8
        self.noise2 = noise2 = 0
        self.noise = noise = 0
        self.length = length = 96
        self.gain2 = gain2 = 0.6
        self.gain = gain = 0.6
        self.freq_offset2 = freq_offset2 = 0
        self.freq_offset = freq_offset = 0
        self.freq2 = freq2 = freq+5e6

        ##################################################
        # Blocks
        ##################################################
        self._time_offset2_range = Range(0.95, 1.05, 0.000001, 1, 200)
        self._time_offset2_win = RangeWidget(self._time_offset2_range, self.set_time_offset2, "Time offset", "counter_slider", float)
        self.top_layout.addWidget(self._time_offset2_win)
        self._time_offset_range = Range(0.95, 1.05, 0.000001, 1, 200)
        self._time_offset_win = RangeWidget(self._time_offset_range, self.set_time_offset, "Time offset", "counter_slider", float)
        self.top_layout.addWidget(self._time_offset_win)
        self._noise2_range = Range(0, 0.2, 0.0001, 0, 200)
        self._noise2_win = RangeWidget(self._noise2_range, self.set_noise2, "Noise", "counter_slider", float)
        self.top_layout.addWidget(self._noise2_win)
        self._noise_range = Range(0, 0.2, 0.0001, 0, 200)
        self._noise_win = RangeWidget(self._noise_range, self.set_noise, "Noise", "counter_slider", float)
        self.top_layout.addWidget(self._noise_win)
        self._freq_offset2_range = Range(0, 0.05, 0.0001, 0, 200)
        self._freq_offset2_win = RangeWidget(self._freq_offset2_range, self.set_freq_offset2, "Freq offset", "counter_slider", float)
        self.top_layout.addWidget(self._freq_offset2_win)
        self._freq_offset_range = Range(0, 0.05, 0.0001, 0, 200)
        self._freq_offset_win = RangeWidget(self._freq_offset_range, self.set_freq_offset, "Freq offset", "counter_slider", float)
        self.top_layout.addWidget(self._freq_offset_win)
        self.uhd_usrp_sink_0_0 = uhd.usrp_sink(
        	",".join(("", "serial=30C6272")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0_0.set_center_freq(freq, 0)
        self.uhd_usrp_sink_0_0.set_normalized_gain(gain, 0)
        self.uhd_usrp_sink_0_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "serial=30C628B")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(freq2, 0)
        self.uhd_usrp_sink_0.set_normalized_gain(gain2, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.qtgui_time_sink_x_0_0_0 = qtgui.time_sink_c(
        	packet_len, #size
        	samp_rate, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0_0_0.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0_0_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0_0_0.disable_legend()
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        
        for i in xrange(2*1):
            if len(labels[i]) == 0:
                if(i % 2 == 0):
                    self.qtgui_time_sink_x_0_0_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_0_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_0_0_win, 2,0,1,1)
        self.qtgui_time_sink_x_0_0 = qtgui.time_sink_c(
        	packet_len, #size
        	samp_rate, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0_0.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0_0.disable_legend()
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        
        for i in xrange(2*1):
            if len(labels[i]) == 0:
                if(i % 2 == 0):
                    self.qtgui_time_sink_x_0_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_0_win, 1,0,1,1)
        self.mysvl_svl_1 = mysvl.svl(gr.sizeof_gr_complex*1, 1, "./inputs/spectrum_maps/one_many_tx.txt", "./inputs/parameters/one_many_tx.txt")
        self.digital_psk_mod_0_0 = digital.psk.psk_mod(
          constellation_points=2,
          mod_code="gray",
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          verbose=False,
          log=False,
          )
        self.digital_psk_mod_0 = digital.psk.psk_mod(
          constellation_points=2,
          mod_code="gray",
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          verbose=False,
          log=False,
          )
        self.digital_ofdm_tx_0 = digital.ofdm_tx(
        	  fft_len=64, cp_len=16,
        	  packet_length_tag_key="length",
        	  bps_header=1,
        	  bps_payload=2,
        	  rolloff=0,
        	  debug_log=False,
        	  scramble_bits=False
        	 )
        self.channels_channel_model_0_0 = channels.channel_model(
        	noise_voltage=noise2,
        	frequency_offset=freq_offset2,
        	epsilon=time_offset2,
        	taps=(1+1j, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.channels_channel_model_0 = channels.channel_model(
        	noise_voltage=noise,
        	frequency_offset=freq_offset,
        	epsilon=time_offset,
        	taps=(1+1j, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_vector_source_x_1_0_0 = blocks.vector_source_b(range(1,65), True, 1, [])
        self.blocks_vector_source_x_1_0 = blocks.vector_source_b(range(1,65), True, 1, [])
        self.blocks_vector_source_x_0_0 = blocks.vector_source_c(time_preamble, True, 1, [])
        self.blocks_vector_source_x_0 = blocks.vector_source_c(time_preamble, True, 1, [])
        self.blocks_udp_sink_0_0 = blocks.udp_sink(gr.sizeof_gr_complex*1, "127.0.0.2", 4000, udp_len, True)
        self.blocks_udp_sink_0 = blocks.udp_sink(gr.sizeof_gr_complex*1, "127.0.0.1", 4000, udp_len, True)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, length, "length")
        self.blocks_stream_mux_0_0 = blocks.stream_mux(gr.sizeof_gr_complex*1, (preamble_len, number_len,packet_len))
        self.blocks_stream_mux_0 = blocks.stream_mux(gr.sizeof_gr_complex*1, (preamble_len, number_len,packet_len))
        self.blocks_multiply_const_vxx_0_0_0_0 = blocks.multiply_const_vcc((0.5, ))
        self.blocks_multiply_const_vxx_0_0_0 = blocks.multiply_const_vcc((0.5, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.05, ))
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_char*1, "./inputs/Memory_and_Forgetting.mp3", True)
        self.blks2_packet_encoder_0_0 = grc_blks2.packet_mod_b(grc_blks2.packet_encoder(
        		samples_per_symbol=2,
        		bits_per_symbol=1,
        		preamble="",
        		access_code="",
        		pad_for_usrp=True,
        	),
        	payload_length=1,
        )
        self.blks2_packet_encoder_0 = grc_blks2.packet_mod_b(grc_blks2.packet_encoder(
        		samples_per_symbol=2,
        		bits_per_symbol=1,
        		preamble="",
        		access_code="",
        		pad_for_usrp=True,
        	),
        	payload_length=1,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blks2_packet_encoder_0, 0), (self.digital_psk_mod_0, 0))    
        self.connect((self.blks2_packet_encoder_0_0, 0), (self.digital_psk_mod_0_0, 0))    
        self.connect((self.blocks_file_source_0_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.mysvl_svl_1, 0))    
        self.connect((self.blocks_multiply_const_vxx_0_0_0, 0), (self.blocks_stream_mux_0_0, 1))    
        self.connect((self.blocks_multiply_const_vxx_0_0_0_0, 0), (self.blocks_stream_mux_0, 1))    
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_throttle_0_0, 0))    
        self.connect((self.blocks_stream_mux_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.blocks_stream_mux_0_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.blocks_stream_mux_0_0, 0), (self.uhd_usrp_sink_0_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.digital_ofdm_tx_0, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.channels_channel_model_0, 0))    
        self.connect((self.blocks_throttle_0_0, 0), (self.channels_channel_model_0_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_stream_mux_0, 0))    
        self.connect((self.blocks_vector_source_x_0_0, 0), (self.blocks_stream_mux_0_0, 0))    
        self.connect((self.blocks_vector_source_x_1_0, 0), (self.blks2_packet_encoder_0, 0))    
        self.connect((self.blocks_vector_source_x_1_0_0, 0), (self.blks2_packet_encoder_0_0, 0))    
        self.connect((self.channels_channel_model_0, 0), (self.blocks_udp_sink_0, 0))    
        self.connect((self.channels_channel_model_0, 0), (self.qtgui_time_sink_x_0_0, 0))    
        self.connect((self.channels_channel_model_0_0, 0), (self.blocks_udp_sink_0_0, 0))    
        self.connect((self.channels_channel_model_0_0, 0), (self.qtgui_time_sink_x_0_0_0, 0))    
        self.connect((self.digital_ofdm_tx_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.digital_psk_mod_0, 0), (self.blocks_multiply_const_vxx_0_0_0, 0))    
        self.connect((self.digital_psk_mod_0_0, 0), (self.blocks_multiply_const_vxx_0_0_0_0, 0))    
        self.connect((self.mysvl_svl_1, 1), (self.blocks_stream_mux_0, 2))    
        self.connect((self.mysvl_svl_1, 0), (self.blocks_stream_mux_0_0, 2))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.set_freq2(self.freq+5e6)
        self.uhd_usrp_sink_0_0.set_center_freq(self.freq, 0)

    def get_udp_len(self):
        return self.udp_len

    def set_udp_len(self, udp_len):
        self.udp_len = udp_len

    def get_time_preamble(self):
        return self.time_preamble

    def set_time_preamble(self, time_preamble):
        self.time_preamble = time_preamble
        self.blocks_vector_source_x_0.set_data(self.time_preamble, [])
        self.blocks_vector_source_x_0_0.set_data(self.time_preamble, [])

    def get_time_offset2(self):
        return self.time_offset2

    def set_time_offset2(self, time_offset2):
        self.time_offset2 = time_offset2
        self.channels_channel_model_0_0.set_timing_offset(self.time_offset2)

    def get_time_offset(self):
        return self.time_offset

    def set_time_offset(self, time_offset):
        self.time_offset = time_offset
        self.channels_channel_model_0.set_timing_offset(self.time_offset)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)
        self.qtgui_time_sink_x_0_0.set_samp_rate(self.samp_rate)
        self.qtgui_time_sink_x_0_0_0.set_samp_rate(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.uhd_usrp_sink_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

    def get_preamble_len(self):
        return self.preamble_len

    def set_preamble_len(self, preamble_len):
        self.preamble_len = preamble_len

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len

    def get_number_len(self):
        return self.number_len

    def set_number_len(self, number_len):
        self.number_len = number_len

    def get_noise2(self):
        return self.noise2

    def set_noise2(self, noise2):
        self.noise2 = noise2
        self.channels_channel_model_0_0.set_noise_voltage(self.noise2)

    def get_noise(self):
        return self.noise

    def set_noise(self, noise):
        self.noise = noise
        self.channels_channel_model_0.set_noise_voltage(self.noise)

    def get_length(self):
        return self.length

    def set_length(self, length):
        self.length = length
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.length)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.length)

    def get_gain2(self):
        return self.gain2

    def set_gain2(self, gain2):
        self.gain2 = gain2
        self.uhd_usrp_sink_0.set_normalized_gain(self.gain2, 0)
        	

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain
        self.uhd_usrp_sink_0_0.set_normalized_gain(self.gain, 0)
        	

    def get_freq_offset2(self):
        return self.freq_offset2

    def set_freq_offset2(self, freq_offset2):
        self.freq_offset2 = freq_offset2
        self.channels_channel_model_0_0.set_frequency_offset(self.freq_offset2)

    def get_freq_offset(self):
        return self.freq_offset

    def set_freq_offset(self, freq_offset):
        self.freq_offset = freq_offset
        self.channels_channel_model_0.set_frequency_offset(self.freq_offset)

    def get_freq2(self):
        return self.freq2

    def set_freq2(self, freq2):
        self.freq2 = freq2
        self.uhd_usrp_sink_0.set_center_freq(self.freq2, 0)


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
