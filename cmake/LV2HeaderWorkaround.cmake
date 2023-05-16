# LV2HeaderWorkaround.cmake
#
# Copyright (C) 2023 AnClark Liu. All rights reserved.
#
# Workaround for LV2 headers on systems without LV2 SDK installed.
# They are unnecessary for DPF, but the Triceratops upstream still requires them
# (mainly in synth.h)
#
# SPDX-License-Identifier: ISC

set(DUMMY_LV2_HEADERS_PATH ${PROJECT_BINARY_DIR}/dpf/lv2-headers)

file(MAKE_DIRECTORY ${DUMMY_LV2_HEADERS_PATH}/lv2/lv2plug.in/ns/ext/atom/)
file(COPY_FILE dpf/distrho/src/lv2/atom-forge.h ${DUMMY_LV2_HEADERS_PATH}/lv2/lv2plug.in/ns/ext/atom/forge.h)
file(COPY_FILE dpf/distrho/src/lv2/atom-util.h ${DUMMY_LV2_HEADERS_PATH}/lv2/lv2plug.in/ns/ext/atom/atom-util.h)
file(COPY_FILE dpf/distrho/src/lv2/atom.h ${DUMMY_LV2_HEADERS_PATH}/lv2/lv2plug.in/ns/ext/atom/atom.h)
file(COPY_FILE dpf/distrho/src/lv2/urid.h ${DUMMY_LV2_HEADERS_PATH}/lv2/lv2plug.in/ns/ext/atom/urid.h)

include_directories(${DUMMY_LV2_HEADERS_PATH})
