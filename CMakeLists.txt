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

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(dpf)
set(DPF_WIDGETS_PATH ext/dpf_widgets)

option(TR_USE_EXTERNAL_CATEGORY_FILE "Use external preset category file (upstream behavior), otherwise use built-in one" OFF)
option(TR_BUILTIN_FACTORY_PRESETS "Embed factory presets into plugin" ON)

configure_file(config.h.in ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR})

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

# Workaround for `uint` not being recognized on Win32
if(WIN32)
    target_compile_definitions(${PROJECT_NAME}-core PRIVATE uint=unsigned\ int)
endif()

# Workaround for LV2 headers on systems without LV2 SDK installed
include(LV2HeaderWorkaround)

dpf_add_plugin(${PROJECT_NAME}
    TARGETS vst2 vst3 lv2 clap
    FILES_DSP
        plugin/triceratops.cpp
        plugin/engine.cpp
    FILES_COMMON
        plugin/params.cpp
        plugin/presets.cpp
    FILES_UI
        ${DPF_WIDGETS_PATH}/opengl/DearImGui.cpp
        plugin/triceratops_ui.cpp
        ui/EditorUI.cpp
        ui/Menus.cpp
        ui/Utils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "plugin/"
    "ui/"
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-core)

# Introduce DPF's Dear ImGui headers
target_include_directories (${PROJECT_NAME} PUBLIC ${DPF_WIDGETS_PATH}/generic)
target_include_directories (${PROJECT_NAME} PUBLIC ${DPF_WIDGETS_PATH}/opengl)
target_include_directories(${PROJECT_NAME} PUBLIC ${DPF_WIDGETS_PATH}/opengl/DearImGui)

# Built-in factory presets support
include(FactoryPresets)

