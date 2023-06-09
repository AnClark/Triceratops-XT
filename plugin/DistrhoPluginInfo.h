/****************************************************************************
   DistrhoPluginInfo.h - Plugin meta definitions

   Copyright (C) 2023, AnClark Liu. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 3
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND   "Nick Bailey, AnClark Liu"
#define DISTRHO_PLUGIN_NAME    "Triceratops"
#define DISTRHO_PLUGIN_URI     "https://github.com/anclark/triceratops-xt"
#define DISTRHO_PLUGIN_CLAP_ID "thunderox.triceratops"

#define DISTRHO_PLUGIN_HAS_UI       1
#define DISTRHO_UI_USE_NANOVG       0
#define DISTRHO_PLUGIN_HAS_EMBED_UI 1
#define DISTRHO_PLUGIN_IS_RT_SAFE   1
#define DISTRHO_PLUGIN_IS_SYNTH     1
#define DISTRHO_PLUGIN_NUM_INPUTS   2
#define DISTRHO_PLUGIN_NUM_OUTPUTS  2
#define DISTRHO_UI_USER_RESIZABLE   0

#define DISTRHO_UI_DEFAULT_WIDTH  700
#define DISTRHO_UI_DEFAULT_HEIGHT 390

// Enable Dear ImGui support
// See dpf/distrho/DistrhoInfo.hpp for more details.
#define DISTRHO_UI_USE_CUSTOM          1
#define DISTRHO_UI_CUSTOM_INCLUDE_PATH "DearImGui.hpp"
#define DISTRHO_UI_CUSTOM_WIDGET_TYPE  DGL_NAMESPACE::ImGuiTopLevelWidget

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED

// end of DistrhoPluginInfo.h