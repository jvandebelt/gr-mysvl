#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sun Mar 10 22:57:08 2019
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
from gnuradio import analog
from gnuradio import blocks
from gnuradio import channels
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import mysvl
import relative_paths  # embedded python module
import sip
import sys


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
        self.time_preamble = time_preamble =  [0.125000+0.000000j, 0.522104-0.148216j, -0.495528+0.114832j, -0.267916+0.091700j, 0.236544-0.138456j, -0.098500+0.473800j, 0.476480-0.225344j, -0.187516+0.035372j, 0.051776-0.353552j, -0.104936+0.059916j,  0.228684+0.117504j, -0.530912+0.560756j, 0.359128+0.015872j, -0.132852+0.632840j, -0.105164-0.368872j, 0.368272-0.032412j, 0.125000+0.750000j, 0.463968+0.457792j, 0.151476-0.430948j, 0.685052+0.238524j, 0.494428+0.119428j, -0.557540-0.050056j, 0.416348+0.017368j, 0.104256-0.568836j, -0.301776-0.353552j, 0.079812+0.451516j, 0.439152+0.528072j, 0.642060+0.178484j, -0.090096+0.465096j, -0.446492+0.305776j, -0.111440-0.093688j, -0.538848-0.320228j, 0.125000+0.000000j, -0.538848+0.320228j, -0.111440+0.093688j, -0.446492-0.305776j, -0.090096-0.465096j, 0.642060-0.178484j, 0.439152-0.528072j, 0.079812-0.451516j, -0.301776+0.353552j, 0.104256+0.568836j, 0.416348-0.017368j, -0.557540+0.050056j, 0.494428-0.119428j, 0.685052-0.238524j, 0.151476+0.430948j, 0.463968-0.457792j, 0.125000-0.750000j, 0.368272+0.032412j, -0.105164+0.368872j, -0.132852-0.632840j, 0.359128-0.015872j, -0.530912-0.560756j, 0.228684-0.117504j, -0.104936-0.059916j, 0.051776+0.353552j, -0.187516-0.035372j, 0.476480+0.225344j, -0.098500-0.473800j, 0.236544+0.138456j, -0.267916-0.091700j, -0.495528-0.114832j, 0.522104+0.148216j]
        self.time_offset = time_offset = 1
        self.threshold = threshold = 11
        self.samp_rate = samp_rate = 2000
        self.preamble_len = preamble_len = 64
        self.packet_len = packet_len = 512
        self.noise = noise = 0
        self.length = length = 96
        self.freq_offset = freq_offset = 0

        ##################################################
        # Blocks
        ##################################################
        self._time_offset_range = Range(0.95, 1.05, 0.000001, 1, 200)
        self._time_offset_win = RangeWidget(self._time_offset_range, self.set_time_offset, "Time offset", "counter_slider", float)
        self.top_grid_layout.addWidget(self._time_offset_win, 0,0,1,1)
        self._threshold_range = Range(0, 25, 0.25, 11, 200)
        self._threshold_win = RangeWidget(self._threshold_range, self.set_threshold, "Threshold", "counter_slider", float)
        self.top_layout.addWidget(self._threshold_win)
        self._noise_range = Range(0, 0.2, 0.0001, 0, 200)
        self._noise_win = RangeWidget(self._noise_range, self.set_noise, "Noise", "counter_slider", float)
        self.top_layout.addWidget(self._noise_win)
        self._freq_offset_range = Range(0, 0.05, 0.0001, 0, 200)
        self._freq_offset_win = RangeWidget(self._freq_offset_range, self.set_freq_offset, "Freq offset", "counter_slider", float)
        self.top_layout.addWidget(self._freq_offset_win)
        self.qtgui_time_sink_x_1 = qtgui.time_sink_f(
        	preamble_len+packet_len, #size
        	samp_rate, #samp_rate
        	"Synchronization", #name
        	2 #number of inputs
        )
        self.qtgui_time_sink_x_1.set_update_time(0.10)
        self.qtgui_time_sink_x_1.set_y_axis(-0.2, 2)
        
        self.qtgui_time_sink_x_1.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_1.enable_tags(-1, False)
        self.qtgui_time_sink_x_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_1.enable_autoscale(False)
        self.qtgui_time_sink_x_1.enable_grid(False)
        self.qtgui_time_sink_x_1.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_1.disable_legend()
        
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
        
        for i in xrange(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_1.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_1.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_1.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_1.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_1.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_1.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_1_win = sip.wrapinstance(self.qtgui_time_sink_x_1.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_1_win, 1,0,1,2)
        self.qtgui_time_sink_x_0_1_0 = qtgui.time_sink_c(
        	packet_len*4, #size
        	samp_rate, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0_1_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_1_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0_1_0.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0_1_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0_1_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_1_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_1_0.enable_grid(False)
        self.qtgui_time_sink_x_0_1_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0_1_0.disable_legend()
        
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
                    self.qtgui_time_sink_x_0_1_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_1_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_1_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_1_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_1_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_1_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_1_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_1_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_1_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_1_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_1_0_win, 4,0,1,1)
        self.qtgui_time_sink_x_0_1 = qtgui.time_sink_c(
        	packet_len*4, #size
        	samp_rate, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0_1.set_update_time(0.10)
        self.qtgui_time_sink_x_0_1.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0_1.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0_1.enable_tags(-1, True)
        self.qtgui_time_sink_x_0_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_1.enable_autoscale(False)
        self.qtgui_time_sink_x_0_1.enable_grid(False)
        self.qtgui_time_sink_x_0_1.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0_1.disable_legend()
        
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
                    self.qtgui_time_sink_x_0_1.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_1.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_1.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_1.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_1.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_1.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_1.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_1.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_1_win = sip.wrapinstance(self.qtgui_time_sink_x_0_1.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_1_win, 3,0,1,1)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_c(
        	preamble_len*4, #size
        	samp_rate, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0.set_y_label("Amplitude", "")
        
        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0.disable_legend()
        
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
                    self.qtgui_time_sink_x_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win, 2,0,1,1)
        self.mysvl_triggered_demux_0 = mysvl.triggered_demux(gr.sizeof_gr_complex*1, gr.sizeof_float, (preamble_len,packet_len), 1, True)
        self.mysvl_drop_packet_0 = mysvl.drop_packet(packet_len)
        self.digital_corr_est_cc_0 = digital.corr_est_cc((time_preamble), 1, 0, 0.99)
        self.channels_channel_model_0 = channels.channel_model(
        	noise_voltage=noise,
        	frequency_offset=freq_offset,
        	epsilon=time_offset,
        	taps=(1+1j, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_vector_source_x_0_0 = blocks.vector_source_c(time_preamble, True, 1, [])
        self.blocks_udp_source_0 = blocks.udp_source(gr.sizeof_gr_complex*1, "127.0.0.1", 4000, 1472*32, True)
        (self.blocks_udp_source_0).set_min_output_buffer(1000000)
        self.blocks_udp_sink_0 = blocks.udp_sink(gr.sizeof_gr_complex*1, "127.0.0.1", 4000, 1472*16, True)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_threshold_ff_0 = blocks.threshold_ff(threshold, threshold, 0)
        self.blocks_tag_gate_1 = blocks.tag_gate(gr.sizeof_gr_complex * 1, False)
        self.blocks_stream_mux_0_0 = blocks.stream_mux(gr.sizeof_gr_complex*1, (preamble_len, packet_len))
        self.blocks_complex_to_float_0 = blocks.complex_to_float(1)
        self.blocks_abs_xx_1 = blocks.abs_ff(1)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 100, 1, 0)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, threshold)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0, 0), (self.qtgui_time_sink_x_1, 1))    
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.blocks_abs_xx_1, 0), (self.blocks_threshold_ff_0, 0))    
        self.connect((self.blocks_abs_xx_1, 0), (self.qtgui_time_sink_x_1, 0))    
        self.connect((self.blocks_complex_to_float_0, 0), (self.blocks_abs_xx_1, 0))    
        self.connect((self.blocks_stream_mux_0_0, 0), (self.channels_channel_model_0, 0))    
        self.connect((self.blocks_tag_gate_1, 0), (self.mysvl_triggered_demux_0, 0))    
        self.connect((self.blocks_threshold_ff_0, 0), (self.mysvl_triggered_demux_0, 1))    
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_mux_0_0, 1))    
        self.connect((self.blocks_udp_source_0, 0), (self.digital_corr_est_cc_0, 0))    
        self.connect((self.blocks_vector_source_x_0_0, 0), (self.blocks_stream_mux_0_0, 0))    
        self.connect((self.channels_channel_model_0, 0), (self.blocks_udp_sink_0, 0))    
        self.connect((self.digital_corr_est_cc_0, 1), (self.blocks_complex_to_float_0, 0))    
        self.connect((self.digital_corr_est_cc_0, 0), (self.blocks_tag_gate_1, 0))    
        self.connect((self.mysvl_drop_packet_0, 0), (self.qtgui_time_sink_x_0_1_0, 0))    
        self.connect((self.mysvl_triggered_demux_0, 1), (self.mysvl_drop_packet_0, 0))    
        self.connect((self.mysvl_triggered_demux_0, 0), (self.qtgui_time_sink_x_0, 0))    
        self.connect((self.mysvl_triggered_demux_0, 1), (self.qtgui_time_sink_x_0_1, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_time_preamble(self):
        return self.time_preamble

    def set_time_preamble(self, time_preamble):
        self.time_preamble = time_preamble
        self.blocks_vector_source_x_0_0.set_data(self.time_preamble, [])

    def get_time_offset(self):
        return self.time_offset

    def set_time_offset(self, time_offset):
        self.time_offset = time_offset
        self.channels_channel_model_0.set_timing_offset(self.time_offset)

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold
        self.analog_const_source_x_0.set_offset(self.threshold)
        self.blocks_threshold_ff_0.set_hi(self.threshold)
        self.blocks_threshold_ff_0.set_lo(self.threshold)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)
        self.qtgui_time_sink_x_0_1.set_samp_rate(self.samp_rate)
        self.qtgui_time_sink_x_0_1_0.set_samp_rate(self.samp_rate)
        self.qtgui_time_sink_x_1.set_samp_rate(self.samp_rate)

    def get_preamble_len(self):
        return self.preamble_len

    def set_preamble_len(self, preamble_len):
        self.preamble_len = preamble_len

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len

    def get_noise(self):
        return self.noise

    def set_noise(self, noise):
        self.noise = noise
        self.channels_channel_model_0.set_noise_voltage(self.noise)

    def get_length(self):
        return self.length

    def set_length(self, length):
        self.length = length

    def get_freq_offset(self):
        return self.freq_offset

    def set_freq_offset(self, freq_offset):
        self.freq_offset = freq_offset
        self.channels_channel_model_0.set_frequency_offset(self.freq_offset)


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
