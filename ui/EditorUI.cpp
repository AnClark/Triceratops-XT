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

#include "EditorUI.hpp"
#include "presets.hpp"

#include <string>
#include <time.h>
#include <vector>

EditorUI::EditorUI(PluginUI* uiInstance)
    : m_ui_instance(uiInstance)
{
}

void EditorUI::mainWindow()
{
    const float width  = m_ui_instance->getWidth();
    const float height = m_ui_instance->getHeight();
    const float margin = 20.0f * m_ui_instance->getScaleFactor();

    ImGui::SetNextWindowPos(ImVec2(margin, margin));
    ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

    if (ImGui::Begin("Triceratops-XT", nullptr, ImGuiWindowFlags_NoResize)) {
        static char aboutText[256] = "This is the demo UI for Triceratops-XT. Work in progress.\n";
        ImGui::InputTextMultiline("About", aboutText, sizeof(aboutText));

        // Test: Preset loader
        {
            createPresetMenu();

            if (ImGui::Button("Presets")) {
                ImGui::OpenPopup("Preset Menu");
            }
        }
    }

    ImGui::End();
}
