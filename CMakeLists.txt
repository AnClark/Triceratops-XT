#  CMakeLists.txt - Build system for Triceratops
#
#  Copyright (C) 2023 AnClark Liu. All rights reserved.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

cmake_minimum_required(VERSION 3.20)

project(Triceratops
    VERSION 0.0.1
    LANGUAGES C CXX
)

add_subdirectory(dpf)

include_directories(
    src/
    src/dsp/
)

add_library(${PROJECT_NAME}-core STATIC
    src/dsp/DLineN.cpp
    src/dsp/Filter.cpp
    src/dsp/JCRev.cpp
    src/dsp/Object.cpp
    src/dsp/Reverb.cpp
    src/dsp/eq.cpp
    src/fastmaths.cpp
    src/filter.cpp
    src/lfo.cpp
    src/nix-echo.cpp
    src/synth.cpp
)

dpf_add_plugin(${PROJECT_NAME}
    TARGETS vst2 vst3 lv2 clap
    FILES_DSP
        plugin/triceratops.cpp
        plugin/engine.cpp
    FILES_COMMON
        plugin/params.cpp
        plugin/presets.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "plugin/")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-core)

# Built-in factory presets support
include(FactoryPresets)

