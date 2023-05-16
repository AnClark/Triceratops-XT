/****************************************************************************
   triceratops_ui.cpp - Triceratops DPF plugin editor interface

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

#include "triceratops_ui.hpp"
#include "EditorUI.hpp"

#include <cstdlib>
#include <cstring>

START_NAMESPACE_DISTRHO

/**
   UI class constructor.
   The UI should be initialized to a default state that matches the plugin side.
 */
TriceratopsUI::TriceratopsUI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT)
    , fResizeHandle(this)
//, fEditorUI(nullptr)
{
    setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

    // hide handle if UI is resizable
    if (isResizable())
        fResizeHandle.hide();

    // Initialize parameters
    _initParameterProperties();

    // Initialize presets
#ifdef DISTRHO_PLUGIN_TARGET_LV2
    fPresets = new Presets(getBundlePath());
#else
    fPresets = new Presets();
#endif

    // Create ImGui UI assistant instance
    fEditorUI = new EditorUI(this);
}

TriceratopsUI::~TriceratopsUI() { }

// ----------------------------------------------------------------------------------------------------------------
// DSP/Plugin Callbacks

/**
   A parameter has changed on the plugin side.@n
   This is called by the host to inform the UI about parameter changes.
 */
void TriceratopsUI::parameterChanged(uint32_t index, float value)
{
    fParamValues[index] = value;

    repaint();
}

// ----------------------------------------------------------------------------------------------------------------
// Widget Callbacks

/**
  ImGui specific onDisplay function.
*/
void TriceratopsUI::onImGuiDisplay()
{
    fEditorUI->mainWindow();
}

// ----------------------------------------------------------------------------------------------------------------
// Internal Procedures

/**
  Initialize parameter properties.
  This function should be called only once in the constructor.
*/
void TriceratopsUI::_initParameterProperties()
{
    for (int index = 0; index < TRICERATOPS_PARAM_COUNT; index++) {
        fParamMinValues[index]     = TriceratopsParam::paramMinValue(ParamId(index));
        fParamMaxValues[index]     = TriceratopsParam::paramMaxValue(ParamId(index));
        fParamDefaultValues[index] = TriceratopsParam::paramDefaultValue(ParamId(index));
    }
}

void TriceratopsUI::_applyPreset(PresetsObject preset)
{
    fPresets->loadPreset(preset, fParamValues);

    for (uint32_t i = 0; i < TRICERATOPS_PARAM_COUNT; i++) {
        setParameterValue(i, fParamValues[i]);
    }
}

// ----------------------------------------------------------------------------------------------------------------
// Utility APIs

/**
  All notes off (panic).
*/
void TriceratopsUI::panic()
{
    /**
      There is no API to send MIDI data to DSP side. Instead, DPF provides UI::sendNote().
      But we can still send any MIDI data via this method, simply doing a hack.

      In method UI::sendNote(uint8_t channel, uint8_t note, uint8_t velocity):

      `channel` will be converted into MIDI data 0 by:
        `midi_data[0] = (velocity != 0 ? 0x90 : 0x80) | channel;`

      To send MIDI controller data, we need to transform midi_data[0] into MIDI_STATUS_CONTROLLER (0xB0)
      by simply applying a proper `channel` value. It would be easy to calculate:
            0x80 | channel = 0xB0
        => 0b10000000 | channel = 0b10110000
        => channel = 0b00110000
        => channel = 0x30
    */
    sendNote(0x30, 0x7B, 0); // MIDI_CC_ALL_NOTES_OFF = 0x7B
}

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new TriceratopsUI();
}

END_NAMESPACE_DISTRHO
