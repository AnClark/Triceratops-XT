/****************************************************************************
   Menus.hpp - Definitions of Triceratops UI menus

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

#include <string>
#include <vector>

using namespace std;

void EditorUI::createPresetMenu()
{
    stringstream label_builder;

    if (ImGui::BeginPopupContextItem("Preset Menu")) {

        vector<PresetsObject> category_list = m_ui_instance->fPresets->getCategories();
        vector<PresetsObject> preset_list_by_category;

        for (auto category_item = category_list.begin(); category_item != category_list.end(); category_item++) {

            if (ImGui::BeginMenu(_generateImGuiLabel(*category_item).c_str())) {

                preset_list_by_category = m_ui_instance->fPresets->getPresets(category_item->category);

                if (preset_list_by_category.size() <= 0) {
                    ImGui::TextDisabled("--- Empty ---");
                } else {
                    for (auto preset_item = preset_list_by_category.begin(); preset_item != preset_list_by_category.end(); preset_item++) {
                        ImGui::SetNextItemWidth(100.0f);
                        if (ImGui::Selectable(_generateImGuiLabel(*preset_item).c_str())) {
                            m_ui_instance->_applyPreset(*preset_item);
                        }
                    }
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndPopup();
    }
}
