/****************************************************************************
   EditorUI.hpp - Triceratops Dear ImGui UI definitions

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

#ifndef EDITOR_UI_HPP
#define EDITOR_UI_HPP

#include "triceratops_ui.hpp"

typedef DISTRHO::TriceratopsUI PluginUI;

class EditorUI {
public:
    EditorUI(PluginUI*);

    void mainWindow(void);

    friend class DISTRHO::TriceratopsUI;

private:
    PluginUI* m_ui_instance;

    // ----------------------------------------------------------------
    // Menus

    void createPresetMenu();

    // ----------------------------------------------------------------
    // Utils

    string _generateImGuiLabel(PresetsObject);
};

#endif // EDITOR_UI_HPP
