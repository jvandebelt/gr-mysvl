#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Tue Mar 12 18:10:57 2019
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
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
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
        self.samp_rate = samp_rate = 32000
        self.new_time_preamble = new_time_preamble = [52.000000 +0.000000j, -5.919279 -8.536566j, -4.260753 -12.607212j,8.939880 -6.125765j,0.884604 -4.096587j,-6.474256 -3.434596j,-4.174710 -0.578714j,3.791506 +1.235053j,0.279833 -0.109122j,5.530533-0.567199j,-1.929208 -7.317350j,-1.378117 +3.190803j,2.805871 -7.702256j,-5.711313 -1.701914j,-2.966432 +1.741682j,-0.247675 +4.490168j,-3.292893 -1.707107j,2.883898 +2.676192j,5.619135 +0.751750j,-4.115069 +2.814674j,-2.589373 -7.294397j,4.612185 +5.259877j,1.197091 +3.742967j,-0.979906 -3.882719j,-1.890878 +1.962808j,4.160219 -3.353154j,-0.540289 +6.005091j,-2.112620 -3.003023j,-3.042150 +2.611726j,1.802848 +4.135673j,5.298560 +4.620461j,0.703246 +3.450126j,5.000000 -1.000000j,5.157142 -0.379740j,3.726040 -3.647292j,-2.997688 +3.323755j,4.889909 -1.616118j,4.836721 +0.468047j,0.992839 -6.866692j,2.704855 -0.610792j,-6.351763 -2.279833j,-1.939613 +0.731610j,7.529818 +3.181252j,-4.310881 -0.784855j,3.354740 -3.048814j,-3.174640 +4.788060j,1.959678 +1.522812j,7.693754 -1.672790j,-4.707107 +0.292893j,5.049252 +3.458181j,1.552404 -4.513244j,3.323670 -1.959693j,-3.571238 +2.200182j,3.211846 +2.977140j,2.428551 -5.636953j,4.723748 +2.769317j,-0.037192 -4.351763j,-4.334488 -2.957252j,2.495908 +3.419032j,3.958268 +0.210689j,-2.732363 -2.667578j,4.275458 -3.511993j,-2.928632 +2.726462j,-1.663484 -3.392583j,2.000000 +0.000000j,-1.663484 +3.392583j,-2.928632 -2.726462j,4.275458 +3.511993j,-2.732363 +2.667578j,3.958268 -0.210689j,2.495908 -3.419032j,-4.334488 +2.957252j,-0.037192 +4.351763j,4.723748 -2.769317j,2.428551 +5.636953j,3.211846 -2.977140j,-3.571238 -2.200182j,3.323670 +1.959693j,1.552404 +4.513244j,5.049252 -3.458181j,-4.707107 -0.292893j,7.693754 +1.672790j,1.959678 -1.522812j,-3.174640 -4.788060j,3.354740 +3.048814j,-4.310881 +0.784855j,7.529818 -3.181252j,-1.939613 -0.731610j,-6.351763 +2.279833j,2.704855 +0.610792j,0.992839 +6.866692j,4.836721 -0.468047j,4.889909 +1.616118j,-2.997688 -3.323755j,3.726040 +3.647292j,5.157142 +0.379740j,5.000000 +1.000000j,0.703246 -3.450126j,5.298560 -4.620461j,1.802848-4.135673j,-3.042150 -2.611726j,-2.112620 +3.003023j,-0.540289 -6.005091j,4.160219 +3.353154j,-1.890878 -1.962808j,-0.979906 +3.882719j,1.197091 -3.742967j,4.612185 -5.259877j,-2.589373 +7.294397j,-4.115069 -2.814674j,5.619135 -0.751750j,2.883898 -2.676192j,-3.292893 +1.707107j,-0.247675 -4.490168j,-2.966432 -1.741682j,-5.711313 +1.701914j,2.805871 +7.702256j,-1.378117 -3.190803j,-1.929208 +7.317350j,5.530533 +0.567199j,0.279833 +0.109122j,3.791506 -1.235053j,-4.174710 +0.578714j,-6.474256 +3.434596j,0.884604 +4.096587j,8.939880 +6.125765j,-4.260753 +12.607212j,-5.919279 +8.536566j]

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_time_sink_x_0 = qtgui.time_sink_c(
        	128, #size
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
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.fft_vxx_0_0 = fft.fft_vcc(128, True, (), True, 1)
        self.fft_vxx_0 = fft.fft_vcc(128, True, (), True, 1)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 128)
        self.blocks_vector_source_x_0 = blocks.vector_source_c(new_time_preamble, True, 1, [])
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 128)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.0078125, ))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.qtgui_time_sink_x_0, 0))    
        self.connect((self.blocks_stream_to_vector_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_stream_to_vector_0, 0))    
        self.connect((self.blocks_vector_to_stream_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.fft_vxx_0, 0), (self.fft_vxx_0_0, 0))    
        self.connect((self.fft_vxx_0_0, 0), (self.blocks_vector_to_stream_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_time_preamble(self):
        return self.time_preamble

    def set_time_preamble(self, time_preamble):
        self.time_preamble = time_preamble

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_new_time_preamble(self):
        return self.new_time_preamble

    def set_new_time_preamble(self, new_time_preamble):
        self.new_time_preamble = new_time_preamble
        self.blocks_vector_source_x_0.set_data(self.new_time_preamble, [])


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
