#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Example1
# Generated: Thu Mar  7 16:52:28 2019
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
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import blks2 as grc_blks2
from optparse import OptionParser
import mysvl
import relative_paths  # embedded python module
import sip
import sys


class example1(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Example1")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Example1")
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

        self.settings = Qt.QSettings("GNU Radio", "example1")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 10000000
        self.length = length = 96

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	1024, #size
        	samp_rate, #samp_rate
        	"Demodulated GMSK  Data", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0.disable_legend()
        
        labels = ["Bytes", "", "", "", "",
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
        
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win, 2,0,1,2)
        self.qtgui_freq_sink_x_0_0_0_1 = qtgui.freq_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"Virtual Spectrum 2", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0_0_0_1.set_update_time(0.10)
        self.qtgui_freq_sink_x_0_0_0_1.set_y_axis(-80, -20)
        self.qtgui_freq_sink_x_0_0_0_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0_0_0_1.enable_autoscale(False)
        self.qtgui_freq_sink_x_0_0_0_1.enable_grid(False)
        self.qtgui_freq_sink_x_0_0_0_1.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0_0_0_1.enable_control_panel(False)
        
        if not True:
          self.qtgui_freq_sink_x_0_0_0_1.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0_0_0_1.set_plot_pos_half(not True)
        
        labels = ["GMSK Signal", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0_0_0_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0_0_0_1.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0_0_0_1.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0_0_0_1.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0_0_0_1.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_0_0_1_win = sip.wrapinstance(self.qtgui_freq_sink_x_0_0_0_1.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_0_0_1_win, 0,1,1,1)
        self.qtgui_freq_sink_x_0_0_0_0 = qtgui.freq_sink_c(
        	2048, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"Real Spectrum", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0_0_0_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0_0_0_0.set_y_axis(-80, -20)
        self.qtgui_freq_sink_x_0_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0_0_0_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0_0_0_0.enable_grid(False)
        self.qtgui_freq_sink_x_0_0_0_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0_0_0_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_freq_sink_x_0_0_0_0.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0_0_0_0.set_plot_pos_half(not True)
        
        labels = ["MySVL Signal", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0_0_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0_0_0_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0_0_0_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0_0_0_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0_0_0_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_0_0_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0_0_0_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_0_0_0_win, 1,0,1,2)
        self.qtgui_freq_sink_x_0_0_0 = qtgui.freq_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"Virtual Spectrum 1", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0_0_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0_0_0.set_y_axis(-80, -20)
        self.qtgui_freq_sink_x_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0_0_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0_0_0.enable_grid(False)
        self.qtgui_freq_sink_x_0_0_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0_0_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_freq_sink_x_0_0_0.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0_0_0.set_plot_pos_half(not True)
        
        labels = ["OFDM Signal", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0_0_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0_0_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0_0_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0_0_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_0_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0_0_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_0_0_win, 0,0,1,1)
        self.mysvl_svl_1 = mysvl.svl(gr.sizeof_gr_complex*1, 1, "./inputs/spectrum_maps/example1a_tx.txt", "./inputs/parameters/example1a-c_tx.txt")
        self.mysvl_svl_0_0 = mysvl.svl(gr.sizeof_gr_complex*1, 1, "./inputs/spectrum_maps/example1a_rx.txt", "./inputs/parameters/example1a-c_rx.txt")
        self.digital_ofdm_tx_0 = digital.ofdm_tx(
        	  fft_len=64, cp_len=16,
        	  packet_length_tag_key="length",
        	  bps_header=1,
        	  bps_payload=2,
        	  rolloff=0,
        	  debug_log=False,
        	  scramble_bits=False
        	 )
        self.digital_ofdm_rx_0 = digital.ofdm_rx(
        	  fft_len=64, cp_len=16,
        	  frame_length_tag_key='frame_'+"length",
        	  packet_length_tag_key="length",
        	  bps_header=1,
        	  bps_payload=2,
        	  debug_log=False,
        	  scramble_bits=False
        	 )
        self.digital_gmsk_mod_0 = digital.gmsk_mod(
        	samples_per_symbol=2,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.digital_gmsk_demod_0 = digital.gmsk_demod(
        	samples_per_symbol=2,
        	gain_mu=0.175,
        	mu=0.5,
        	omega_relative_limit=0.005,
        	freq_error=0.0,
        	verbose=False,
        	log=False,
        )
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, length, "length")
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vcc((2.0/4, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.05, ))
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_char*1, "./inputs/Memory_and_Forgetting.mp3", True)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, "./inputs/Memory_and_Forgetting.mp3", True)
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_char*1, "./outputs/output_audio1.mp3", False)
        self.blocks_file_sink_0_0.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, "./outputs/output_audio2.mp3", False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_char_to_float_0 = blocks.char_to_float(1, 1)
        self.blks2_packet_encoder_1 = grc_blks2.packet_mod_b(grc_blks2.packet_encoder(
        		samples_per_symbol=2,
        		bits_per_symbol=1,
        		preamble="",
        		access_code="",
        		pad_for_usrp=False,
        	),
        	payload_length=0,
        )
        self.blks2_packet_decoder_0 = grc_blks2.packet_demod_b(grc_blks2.packet_decoder(
        		access_code="",
        		threshold=-1,
        		callback=lambda ok, payload: self.blks2_packet_decoder_0.recv_pkt(ok, payload),
        	),
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blks2_packet_decoder_0, 0), (self.blocks_char_to_float_0, 0))    
        self.connect((self.blks2_packet_decoder_0, 0), (self.blocks_file_sink_0_0, 0))    
        self.connect((self.blks2_packet_encoder_1, 0), (self.digital_gmsk_mod_0, 0))    
        self.connect((self.blocks_char_to_float_0, 0), (self.qtgui_time_sink_x_0, 0))    
        self.connect((self.blocks_file_source_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))    
        self.connect((self.blocks_file_source_0_0, 0), (self.blks2_packet_encoder_1, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.mysvl_svl_1, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.qtgui_freq_sink_x_0_0_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.mysvl_svl_1, 1))    
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.qtgui_freq_sink_x_0_0_0_1, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.digital_ofdm_tx_0, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.mysvl_svl_0_0, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.qtgui_freq_sink_x_0_0_0_0, 0))    
        self.connect((self.digital_gmsk_demod_0, 0), (self.blks2_packet_decoder_0, 0))    
        self.connect((self.digital_gmsk_mod_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))    
        self.connect((self.digital_ofdm_rx_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.digital_ofdm_tx_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.mysvl_svl_0_0, 1), (self.digital_gmsk_demod_0, 0))    
        self.connect((self.mysvl_svl_0_0, 0), (self.digital_ofdm_rx_0, 0))    
        self.connect((self.mysvl_svl_1, 0), (self.blocks_throttle_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "example1")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.qtgui_freq_sink_x_0_0_0.set_frequency_range(0, self.samp_rate)
        self.qtgui_freq_sink_x_0_0_0_0.set_frequency_range(0, self.samp_rate)
        self.qtgui_freq_sink_x_0_0_0_1.set_frequency_range(0, self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_length(self):
        return self.length

    def set_length(self, length):
        self.length = length
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.length)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.length)


def main(top_block_cls=example1, options=None):

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
