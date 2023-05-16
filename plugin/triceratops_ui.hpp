/****************************************************************************
   triceratops_ui.hpp - Triceratops DPF plugin editor interface

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

#ifndef TRICERATOPS_UI_HPP
#define TRICERATOPS_UI_HPP

#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"

#include "params.hpp"
#include "presets.hpp"

// Forward declarations
class EditorUI;

// --------------------------------------------------------------------------------------------------------------------

START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

class TriceratopsUI : public UI {

    float fParamValues[TRICERATOPS_PARAM_COUNT];
    float fParamMinValues[TRICERATOPS_PARAM_COUNT], fParamMaxValues[TRICERATOPS_PARAM_COUNT];
    float fParamDefaultValues[TRICERATOPS_PARAM_COUNT];

    ResizeHandle fResizeHandle;

    ScopedPointer<Presets> fPresets;

    ScopedPointer<EditorUI> fEditorUI;
    friend EditorUI; // Allow EditorUI to access private members (param properties)

    // ----------------------------------------------------------------------------------------------------------------

public:
    TriceratopsUI();
    ~TriceratopsUI();

    // ----------------------------------------------------------------------------------------------------------------
    // Utility APIs

    void panic();

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // DSP/Plugin Callbacks

    void parameterChanged(uint32_t index, float value) override;

    // ----------------------------------------------------------------------------------------------------------------
    // Widget Callbacks

    void onImGuiDisplay() override;

private:
    // ----------------------------------------------------------------------------------------------------------------
    // Internal Procedures

    void _initParameterProperties();
    void _applyPreset(PresetsObject preset);

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriceratopsUI)
};

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // TRICERATOPS_UI_HPP
