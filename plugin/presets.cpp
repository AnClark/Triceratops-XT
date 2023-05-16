/****************************************************************************
   presets.cpp - Triceratops preset manager

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

#include "presets.hpp"
#include "params.hpp"
#ifndef TR_USE_EXTERNAL_CATEGORY_FILE
#include "triceratops_categories.h"
#endif

#include "DistrhoUtils.hpp"
#include "src/DistrhoDefines.h"

#include "stb_decompress.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#endif

static void decode_compressed_file_array(const void* compressed_file_data, const int compressed_file_size, std::string& decompressed_file_data)
{
    const unsigned int buf_decompressed_size = triceratops_stb::stb_decompress_length((const unsigned char*)compressed_file_data);
    unsigned char*     buf_decompressed_data = (unsigned char*)malloc(buf_decompressed_size);
    triceratops_stb::stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_file_data, (unsigned int)compressed_file_size);

    std::stringstream output_builder;
    output_builder << buf_decompressed_data;
    decompressed_file_data = output_builder.str();
}

// -------------------------------------------------------------
// Initializer

#ifdef DISTRHO_PLUGIN_TARGET_LV2
Presets::Presets(const char* init_bundle_path)
{
    setBundlePath(init_bundle_path);
}
#else
Presets::Presets()
{
}
#endif

// -------------------------------------------------------------
// Preset accessors

/**
 * Initialize the full preset list (mixture of category and preset items).
 *
 * This function is not intended for directly invoking.
 * In plugin instance, invoke getCategories() and getPresets() instead.
 *
 * @return The reference of preset_list.
 */
vector<PresetsObject>& Presets::fetchTotalPresetList()
{
#ifdef TR_BUILTIN_FACTORY_PRESETS
    if (embed_factory_presets.empty()) {
        InitializeEmbedFactoryPresets(embed_factory_presets);
    }
#endif

    if (preset_list.empty()) {
        _readCategoryFile();
        _readPresetFiles();
    }

    return preset_list;
}

/**
 * Get category list.
 *
 * @return A list of category.
 */
vector<PresetsObject> Presets::getCategories()
{
    fetchTotalPresetList();

    vector<PresetsObject> category_list;

    for (auto preset_item = preset_list.begin(); preset_item != preset_list.end(); preset_item++) {
        if (preset_item->type == PRESET_TYPE_CATEGORY) {
            // Skip empty category
            if (preset_item->name.size() <= 0)
                continue;

            category_list.push_back(*preset_item);
        }
    }

    return category_list;
}

/**
 * Get all presets belonging to given category.
 *
 * @param category ID of category.
 * @return A list of presets.
 */
vector<PresetsObject> Presets::getPresets(const int category)
{
    fetchTotalPresetList();

    vector<PresetsObject> preset_list_by_category;

    for (auto preset_item = preset_list.begin(); preset_item != preset_list.end(); preset_item++) {
        if (preset_item->type == PRESET_TYPE_PRESET && preset_item->category == category) {
            PresetsObject preset = *preset_item;

            preset_list_by_category.push_back(*preset_item);
        }
    }

    return preset_list_by_category;
}

/**
 * Load a preset.
 *
 * @param target_preset
 *        The preset item you want to apply.
 * @param param_values
 *        A param value array storing the current parameters. Param values will be updated in place.
 *        With DPF, you need to call setParameterValue() to apply params.
 * @see TriceratopsUI::_applyPreset()
 */
void Presets::loadPreset(const PresetsObject& target_preset, float param_values[TRICERATOPS_PARAM_COUNT])
{
    // DISTRHO_SAFE_ASSERT(preset_number < preset_list.size())

    ifstream     preset_file;            // Reader for external preset file
    stringstream builtin_preset_content; // Reader for built-in preset
    std::string  decoded_preset_data;    // Stores decoded data of built-in preset

    // const PresetsObject& target_preset = preset_list[preset_number];

    d_stderr("Applying preset: name = %s, built-in = %s", target_preset.name.c_str(), target_preset.is_built_in ? "Yes" : "No");

    if (target_preset.is_built_in) {
        // Only decode and load preset on demand. This should save a lot of memory
        decode_compressed_file_array(target_preset.preset_file_data->file_array, target_preset.preset_file_data->file_array_size, decoded_preset_data);
        // builtin_preset_content << decoded_preset_data;
        builtin_preset_content.str(std::move(decoded_preset_data));

        d_stderr("-- Opening built-in preset: %s", target_preset.name.c_str());
    } else {
        stringstream preset_file_name;
        preset_file_name.str("");

        preset_file_name << target_preset.dir << target_preset.name;

        d_stderr("-- Opening preset: %s", preset_file_name.str().c_str());

        preset_file.open(preset_file_name.str());
    }

    string preset_symbol;
    string line;
    double preset_value;

    while (target_preset.is_built_in ? getline(builtin_preset_content, line) : getline(preset_file, line)) {
        int preset_index = line.rfind("lv2:symbol ");
        if (preset_index > 0)
            preset_symbol = line.substr(preset_index + 12, line.length() - preset_index - 15);

        preset_index = line.rfind("pset:value");
        if (preset_index > 0)
            istringstream(line.substr(preset_index + 11, line.length() + 1)) >> preset_value;

        if (preset_index > 0) {
            int param_index = _getParamSymbolIndex(preset_symbol.c_str());

            if (param_index > -1) {
                float new_value           = preset_value;
                param_values[param_index] = new_value;
            } else
                d_stderr2("Symbol not recognised: %s", preset_symbol.c_str());
        }
    }

    if (!target_preset.is_built_in)
        preset_file.close();
}

// -------------------------------------------------------------
// Initializer functions (internal)

/**
 * Read category list from file (or built-in resource).
 */
bool Presets::_readCategoryFile()
{
    // Cleanup preset list first
    preset_list.clear();

    number_of_categories = 0;

    bool category_file_exists = false;

#ifdef TR_USE_EXTERNAL_CATEGORY_FILE
    stringstream file_name;

#ifdef DISTRHO_PLUGIN_TARGET_LV2
// TODO: On Windows, allow loading preset file from app path
#if !defined(_WIN32) && !defined(__APPLE__)
    if (!category_file_exists) {
        file_name.str("");
        file_name << getenv("HOME") << "/.lv2/"
                  << "/triceratops_categories.txt";
        ifstream check_file(file_name.str());
        if (check_file) {
            d_stderr("loading categories from %s", file_name.str().c_str());
            category_file_exists = true;
        }
    }
#endif // !defined(_WIN32) && !defined(__APPLE__)

    if (!category_file_exists) {
        file_name.str("");
        file_name << bundle_path << "triceratops_categories.txt";
        ifstream check_file(file_name.str());
        if (check_file) {
            d_stderr("loading categories from %s", file_name.str().c_str());
            category_file_exists = true;
        }
    }
#endif // DISTRHO_PLUGIN_TARGET_LV2

#else  // TR_USE_EXTERNAL_CATEGORY_FILE
    string decoded_category_file_data;
    decode_compressed_file_array(Triceratops_Category_File_compressed_data, Triceratops_Category_File_compressed_size, decoded_category_file_data);

    d_stderr("loading categories from built-in resource");
    category_file_exists = true;
#endif // TR_USE_EXTERNAL_CATEGORY_FILE

    if (category_file_exists) {
#ifdef TR_USE_EXTERNAL_CATEGORY_FILE
        ifstream category_file;
        category_file.open(file_name.str());
#else
        stringstream category_file(std::move(decoded_category_file_data));
#endif // TR_USE_EXTERNAL_CATEGORY_FILE

        string line;
        while (getline(category_file, line)) {
            if (line.find("=", 0)) {
                string::size_type i = line.find("=", 0);

                PresetsObject new_preset_object;

                new_preset_object.type = PRESET_TYPE_CATEGORY;
                istringstream(line.substr(i + 1)) >> new_preset_object.category;

                new_preset_object.name   = line.substr(0, i);
                new_preset_object.dir    = "";
                new_preset_object.unfold = false;

                preset_list.push_back(new_preset_object);
                ++number_of_categories;
            }
        }

#ifdef TR_USE_EXTERNAL_CATEGORY_FILE
        category_file.close();
#endif
    } else {
        d_stderr2("No category definitions found.\nCheck your bundle path for triceratops_categories.txt, or reconfigure with TR_USE_EXTERNAL_CATEGORY_FILE=OFF");
    }

    return true;
}

/**
 * Fetch all preset files, then add them to Presets::preset_list.
 */
void Presets::_readPresetFiles()
{
    // Stats
    int count_builtin_presets = 0, count_home_presets = 0, count_bundle_presets = 0;

    // ----------------------------------------------------------
    // LOAD built-in presets

#ifdef TR_BUILTIN_FACTORY_PRESETS
    std::string decoded_preset_data_for_checking;

    if (embed_factory_presets.empty()) {
        d_stderr("No presets found in built-in resources");
    }

    else {
        for (auto preset_item = embed_factory_presets.begin(); preset_item != embed_factory_presets.end(); preset_item++) {
            decode_compressed_file_array(preset_item->file_array, preset_item->file_array_size, decoded_preset_data_for_checking);

            istringstream check_file(decoded_preset_data_for_checking);
            string        line;
            int           preset_category_number = 0;

            while (getline(check_file, line)) {
                int preset_category_finder = line.rfind("preset_category");
                if (preset_category_finder > 0) {
                    getline(check_file, line);
                    istringstream(line.substr(line.rfind("pset:value") + 11)) >> preset_category_number;
                }
            }

            PresetsObject new_preset_object;

            new_preset_object.type             = PRESET_TYPE_PRESET;
            new_preset_object.category         = preset_category_number;
            new_preset_object.name             = string(preset_item->name);
            new_preset_object.dir              = preset_item->file_path;
            new_preset_object.unfold           = false;
            new_preset_object.is_built_in      = true;
            new_preset_object.preset_file_data = &(*preset_item);

            preset_list.push_back(new_preset_object);

            count_builtin_presets++;
        }

        d_stderr("Get %d built-in presets", count_builtin_presets);
    }
#endif // TR_BUILTIN_FACTORY_PRESETS

    string dir;

#ifdef DISTRHO_PLUGIN_TARGET_LV2
#if !defined(_WIN32) && !defined(__APPLE__)
    // ----------------------------------------------------------
    // LOAD $HOME/.lv2 presets (Linux only)

    stringstream home_lv2;
    home_lv2.str("");
    home_lv2 << getenv("HOME") << "/.lv2/";
    dir = home_lv2.str();

    DIR*           dp;
    struct dirent* dirp;

    if ((dp = opendir(dir.c_str())) == NULL) {
        d_stderr("No presets found in %s", dir.c_str());

    }

    else {

        while ((dirp = readdir(dp)) != NULL) {
            if (string(dirp->d_name) != ".directory" && string(dirp->d_name) != "." && string(dirp->d_name) != ".." && string(dirp->d_name) != "manifest.ttl") {
                stringstream file_name;
                file_name.str("");
                file_name << "." << dirp->d_name;

                stringstream preset_file_name_home;
                preset_file_name_home.str("");
                preset_file_name_home << getenv("HOME") << "/.lv2/";
                preset_file_name_home << file_name.str().substr(1);
                preset_file_name_home << "/";
                dir                  = preset_file_name_home.str();
                int file_name_length = file_name.str().length();
                preset_file_name_home << file_name.str().substr(1, file_name_length - 4);
                preset_file_name_home << "ttl";

                bool our_preset = false;

                ifstream check_file(preset_file_name_home.str());
                string   line;
                int      preset_category_number = 0;

                while (getline(check_file, line)) {
                    int triceratops_applies_to =
                        line.rfind("lv2:appliesTo <http://nickbailey.co.nr/triceratops>");

                    if (triceratops_applies_to > 0)
                        our_preset = true;

                    int preset_category_finder = line.rfind("preset_category");
                    if (preset_category_finder > 0) {
                        getline(check_file, line);
                        istringstream(line.substr(line.rfind("pset:value") + 11)) >> preset_category_number;
                    }
                }

                if (our_preset) {
                    preset_file_names.push_back(file_name.str());
                    PresetsObject new_preset_object;

                    new_preset_object.type     = PRESET_TYPE_PRESET;
                    new_preset_object.category = preset_category_number;

                    stringstream file_name_ttl;
                    file_name_ttl.str("");
                    file_name_ttl << file_name.str().substr(1, file_name.str().size() - 4) << "ttl";

                    new_preset_object.name        = file_name_ttl.str();
                    new_preset_object.dir         = dir;
                    new_preset_object.unfold      = false;
                    new_preset_object.is_built_in = false;

                    preset_list.push_back(new_preset_object);

                    count_home_presets++;
                }
            }
        }
    }
    closedir(dp);

    d_stderr("Get %d presets from %s", count_home_presets, dir.c_str());
#endif // !defined(_WIN32) && !defined(__APPLE__)

    // ----------------------------------------------------------
    // LOAD BUNDLE_PATH/../triceratops-presets.lv2/ presets

    stringstream presets_lv2;
    presets_lv2.str("");
    presets_lv2 << bundle_path << "../triceratops-presets.lv2/";
    dir = presets_lv2.str();

#ifdef _WIN32
    // ----------------------------
    // Scan on Windows

    /**
     * Implement preset scanner on Windows
     * Reference: https://www.cnblogs.com/collectionne/p/6815924.html
     */
    intptr_t          handle;
    _finddata_t       findData;
    const std::string dir_path_for_search(dir + "\\*.*");

    handle = _findfirst(dir_path_for_search.c_str(), &findData); // Find the first file
    if (handle == -1) {
        d_stderr("No presets found in %s", dir.c_str());
    } else {

        while (_findnext(handle, &findData) == 0) {
            if (findData.attrib & _A_SUBDIR
                && strcmp(findData.name, ".directory") != 0
                && strcmp(findData.name, ".") != 0
                && strcmp(findData.name, "..") != 0
                && strcmp(findData.name, "manifest.ttl") != 0) { // If sub directory, "."/".." or manifest.ttl detected, ignore
#else
    // ----------------------------
    // Scan on Linux/macOS

    if ((dp = opendir(dir.c_str())) == NULL) {
        d_stderr("No presets found in %s", dir.c_str());
    }

    else {

        while ((dirp = readdir(dp)) != NULL) {
            if (string(dirp->d_name) != ".directory"
                && string(dirp->d_name) != "."
                && string(dirp->d_name) != ".."
                && string(dirp->d_name) != "manifest.ttl") {
#endif // _WIN32
                stringstream preset_file_path;
                preset_file_path.str("");
                preset_file_path << dir;
#ifdef _WIN32
                preset_file_path << string(findData.name);
#else
                preset_file_path << string(dirp->d_name);
#endif

                ifstream check_file(preset_file_path.str());
                string   line;
                int      preset_category_number = 0;

                while (getline(check_file, line)) {
                    int preset_category_finder = line.rfind("preset_category");
                    if (preset_category_finder > 0) {
                        getline(check_file, line);
                        istringstream(line.substr(line.rfind("pset:value") + 11)) >> preset_category_number;
                    }
                }

                PresetsObject new_preset_object;

                new_preset_object.type     = PRESET_TYPE_PRESET;
                new_preset_object.category = preset_category_number;

#ifdef _WIN32
                new_preset_object.name = string(findData.name);
#else
                new_preset_object.name = string(dirp->d_name);
#endif

                new_preset_object.dir         = dir;
                new_preset_object.unfold      = false;
                new_preset_object.is_built_in = false;

                preset_list.push_back(new_preset_object);

                count_bundle_presets++;
            }
        }
#ifdef _WIN32
        _findclose(handle); // Close search handle
#else
        closedir(dp);
#endif

        d_stderr("Get %d presets from bundle path", count_home_presets);
    }
#endif // DISTRHO_PLUGIN_TARGET_LV2

    sort(preset_list.begin() + number_of_categories, preset_list.end(), _sortByName);
}

// -------------------------------------------------------------
// Utils

/**
 * Given a param symbol name, get the index of the parameter.
 */
int Presets::_getParamSymbolIndex(const char* symbol)
{
    for (unsigned int i = 0; i < TRICERATOPS_PARAM_COUNT; i++) {
        const char* current_symbol = TriceratopsParam::paramSymbol((ParamId)i);

        if (strcmp(symbol, current_symbol) == 0)
            return i;
    }

    return -1;
}

/*
 * Check if given param value is legal.
 * If true, return as-is, otherwise return the default value.
 */
float Presets::_sanitizeParam(ParamId param_id, float value)
{
    if (value < TriceratopsParam::paramMinValue(param_id) || value > TriceratopsParam::paramMaxValue(param_id)) {
        d_stderr("-- Warning: parameter [%s] value is invalid. Fallback to its default value", TriceratopsParam::paramSymbol(param_id));

        return TriceratopsParam::paramDefaultValue(param_id);
    }

    return value;
}
