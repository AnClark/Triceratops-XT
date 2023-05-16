/****************************************************************************
   presets.hpp - Triceratops preset manager

   Copyright (C) 2015-2022, Nick Bailey (ThunderOx Software),
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

#ifndef TRICERATOPS_PRESETS_HPP
#define TRICERATOPS_PRESETS_HPP

#include "params.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "config.h"

// -------------------------------------------------------------------------
// Embed factory preset definitions
// -------------------------------------------------------------------------

typedef struct
{
    std::string name;
    std::string file_path;
    bool        read_only;

    const unsigned int* file_array;
    int                 file_array_size;
} EmbedPresetInfo;

#ifdef TR_BUILTIN_FACTORY_PRESETS
static std::vector<EmbedPresetInfo> embed_factory_presets;
void                                InitializeEmbedFactoryPresets(std::vector<EmbedPresetInfo>& factory_presets_list);
#endif

// -------------------------------------------------------------------------
// Triceratop preset definitions
// -------------------------------------------------------------------------

/**
 * Preset type.
 */
enum PresetType {
    PRESET_TYPE_NULL,     // Null
    PRESET_TYPE_CATEGORY, // Category item
    PRESET_TYPE_PRESET    // Preset item
};

/**
 * Preset data object.
 *
 * Both preset and category data are stored in the same struct.
 */
typedef struct
{
    PresetType             type;             // applies to both presets & categories
    int                    category;         // applies to both presets & categories (On category it means category ID)
    string                 name;             // applies to both presets & categories
    string                 dir;              // applies only to presets
    bool                   unfold;           // applies only to categories
    bool                   is_built_in;      // applies only to presets
    const EmbedPresetInfo* preset_file_data; // applies only to presets
} PresetsObject;

class Presets {
public:
    // -------------------------------------------------------------
    // Initializer

#ifdef DISTRHO_PLUGIN_TARGET_LV2
    Presets(const char* init_bundle_path);
#else
    Presets();
#endif

    // -------------------------------------------------------------
    // Preset accessors

    vector<PresetsObject>& fetchTotalPresetList();
    vector<PresetsObject>  getCategories();
    vector<PresetsObject>  getPresets(const int category);

    void loadPreset(const PresetsObject& target_preset, float param_values[TRICERATOPS_PARAM_COUNT]);

    // -------------------------------------------------------------
    // Configuration

#ifdef DISTRHO_PLUGIN_TARGET_LV2
    void setBundlePath(const char* new_bundle_path)
    {
        this->bundle_path = string(new_bundle_path);
    }
#endif
    void setUserPresetScanPath(const char* scan_path)
    {
        this->user_scan_path = string(scan_path);
    }

private:
    // -------------------------------------------------------------
    // Core resources

    vector<PresetsObject> preset_list;

    int number_of_categories;

    string bundle_path;
    string user_scan_path;

    // -------------------------------------------------------------
    // Initializer functions (internal)

    bool _readCategoryFile();
    void _readPresetFiles();

    // -------------------------------------------------------------
    // Utils

    int   _getParamSymbolIndex(const char* symbol);
    float _sanitizeParam(ParamId param_id, float value);

    /**
     * Sort two preset items by name.
     * This function is used together with std::sort().
     */
    static bool _sortByName(const PresetsObject& lhs, const PresetsObject& rhs) { return lhs.name < rhs.name; }
};

#endif // TRICERATOPS_PRESETS_HPP
