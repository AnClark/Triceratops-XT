#!/usr/bin/env python
import os
import sys 
import shutil
from waflib import Logs
from waflib.extras import autowaf as autowaf

# Variables for 'waf dist'
APPNAME = 'triceratops.lv2'
VERSION = '0.5'

# Mandatory variables
top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_c')
    opt.load('compiler_cxx')
    autowaf.set_options(opt)

def configure(conf):	
    conf.load('compiler_c')
    conf.load('compiler_cxx')
    autowaf.configure(conf)
    autowaf.display_header('triceratops Configuration')
   
    if conf.env['MSVC_COMPILER']:
        conf.env.append_unique('CXXFLAGS', ['-TP', '-MD', '-g'])
    else:
        conf.env.append_unique('CXXFLAGS', ['-O2','-funroll-loops','-std=c++0x','-g'])

    if sys.maxsize >= 9223372036854775807:
        print("detected 64 bit architecture, enabling -fPIC")
        conf.env.append_unique('CXXFLAGS', ['-fPIC','-fpermissive','-finline-functions'])

    if not autowaf.is_child():
        autowaf.check_pkg(conf, 'lv2', uselib_store='LV2CORE')
        autowaf.check_pkg(conf, 'gtkmm-2.4', uselib_store='GTKMM')
        autowaf.check_pkg(conf, 'cairomm-1.0', uselib_store='CAIROMM')

    # Set env['pluginlib_PATTERN']
    pat = conf.env['cshlib_PATTERN']
    if pat.startswith('lib'):
        pat = pat[3:]
    conf.env['pluginlib_PATTERN'] = pat
    conf.env['pluginlib_EXT'] = pat[pat.rfind('.'):]

    conf.env.LV2DIR = "/usr/lib/lv2"

    autowaf.display_msg(conf, "LV2 bundle directory", conf.env['LV2DIR'])
    print('')

def build(bld):
    bundle = APPNAME

    # Build manifest.ttl by substitution (for portable lib extension)
    bld(features     = 'subst',
        source       = 'manifest.ttl.in',
        target       = '%s/%s' % (bundle, 'manifest.ttl'),
        install_path = '${LV2DIR}/%s' % bundle,
        LIB_EXT      = bld.env['pluginlib_EXT'])

    # Copy other data files to build bundle (build/triceratops.lv2)
    for i in [ 'triceratops.ttl' ]:
        bld(features     = 'subst',
            source       = i,
            target       = '%s/%s' % (bundle, i),
            install_path = '${LV2DIR}/%s' % bundle,
            LIB_EXT      = bld.env['pluginlib_EXT'])

    # Create a build environment that builds module-style library names
    # e.g. triceratops.so instead of libtriceratops.so
    # Note for C++ you must set cxxshlib_PATTERN instead
    penv                   = bld.env.derive()
    penv['cshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']

    includes = None
    if autowaf.is_child:
        includes = '../..'

    # Build plugin library
    obj = bld(features     = 'cxx cshlib ',
              env          = penv,
              source       = 'triceratops.cpp filter.cpp lfo.cpp fastmaths.cpp synth.cpp nix-echo.cpp dsp/eq.cpp dsp/Object.cpp dsp/Reverb.cpp dsp/DLineN.cpp dsp/JCRev.cpp dsp/Filter.cpp',
              name         = 'triceratops',
              target       = '%s/triceratops' % bundle,
              install_path = '${LV2DIR}/%s' % bundle,
              uselib       = 'LV2CORE',
              includes     = includes)
    
    # Build UI library
    obj = bld(features     = 'cxx cshlib',
              env          = penv,
              source       = 'triceratops_gui.cpp fader_widget.cpp wave_widget.cpp filter_widget.cpp knob_widget.cpp volume_widget.cpp toggle_widget.cpp spacer_widget.cpp presets.cpp widget_button.cpp dco_gui.cpp lfo_gui.cpp adsr_gui.cpp adsr_lfo_gui.cpp amp_gui.cpp echo_gui.cpp reverb_gui.cpp modifier_gui.cpp unison_gui.cpp logo_gui.cpp ',
              name         = 'triceratops_gui',
              target       = '%s/triceratops_gui' % bundle,
              install_path = '${LV2DIR}/%s' % bundle,
              uselib       = 'LV2CORE GTKMM',
              includes     = includes)

    bld.install_files('${LV2DIR}/triceratops.lv2/', 'logo.png')
    bld.install_files('${LV2DIR}/triceratops.lv2/', 'triceratops.conf')
    bld.install_files('${LV2DIR}/triceratops.lv2/', 'triceratops_categories.txt')
    bld.install_files('${LV2DIR}/triceratops-presets.lv2', bld.path.ant_glob('presets.lv2/*.*'))




