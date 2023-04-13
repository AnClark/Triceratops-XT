/****************************************************************************
   triceratops.hpp - Triceratops DPF plugin interface

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

#ifndef TRICERATOPS_PLUGIN_HPP
#define TRICERATOPS_PLUGIN_HPP

#include "DistrhoPlugin.hpp"
#include "params.hpp"

#include "dsp/JCRev.h"
#include "dsp/eq.h"
#include "lfo.h"
#include "nix-echo.hpp"
#include "synth.h"

#include <memory>

constexpr int max_notes = 12;

/*
 * Synthesizer engine interface.
 */
class Triceratops {
public:
    explicit Triceratops(double rate);
    ~Triceratops();

    void run(float* output_left, float* output_right, uint32_t n_samples, const DISTRHO::MidiEvent* midiEvents, uint32_t midiEventCount);
    void activate() { } /* Dummy */

    void  setParam(ParamId index, float value);
    float getParam(ParamId index);

private:
    double sample_rate;

    // The two parameters are passed by run(), and not used here
    // float* output_left;
    // float* output_right;

    LFO* lfo1;
    LFO* lfo2;
    LFO* lfo3;

    float* lfo1_out;
    float* lfo2_out;
    float* lfo3_out;

    int lfo1_count;
    int lfo2_count;
    int lfo3_count;

    float lfo1_rand;
    float lfo2_rand;
    float lfo3_rand;

    int midi_keys[128];
    int current_synth;
    int old_synth;

    float* pitch_bend;
    float* channel_after_touch;

    synth* synths[max_notes];

    nixecho* echo;
    noise*   nixnoise;
    JCRev*   reverb[16];
    EQSTATE* eq_left;
    EQSTATE* eq_right;

    float params[TRICERATOPS_PARAM_COUNT];
    void  _connectParamPorts();
};

START_NAMESPACE_DISTRHO

/*
 * Plugin interface.
 */
class TriceratopsPlugin : public Plugin {
    std::unique_ptr<Triceratops> fSynthesizer;

    double fSampleRate;
    double fBufferSize;

public:
    TriceratopsPlugin();
    ~TriceratopsPlugin();

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // Information

    /**
        Get the plugin label.@n
        This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const noexcept override
    {
        return DISTRHO_PLUGIN_NAME;
    }

    /**
           Get an extensive comment/description about the plugin.@n
           Optional, returns nothing by default.
         */
    const char* getDescription() const override
    {
        return "Polyphonic synthesizer LV2 plugin";
    }

    /**
           Get the plugin author/maker.
         */
    const char* getMaker() const noexcept override
    {
        return DISTRHO_PLUGIN_BRAND;
    }

    /**
           Get the plugin license (a single line of text or a URL).@n
           For commercial plugins this should return some short copyright information.
         */
    const char* getLicense() const noexcept override
    {
        return "GPLv3";
    }

    /**
        Get the plugin version, in hexadecimal.
        @see d_version()
        */
    uint32_t getVersion() const noexcept override
    {
        return d_version(0, 1, 0);
    }

    /**
           Get the plugin unique Id.@n
           This value is used by LADSPA, DSSI and VST plugin formats.
           @see d_cconst()
         */
    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('t', 'r', 'c', 't');
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // ----------------------------------------------------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // ----------------------------------------------------------------------------------------------------------------
    // Audio/MIDI Processing

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount) override;

    // ----------------------------------------------------------------------------------------------------------------
    // Callbacks (optional)

    void bufferSizeChanged(int newBufferSize);
    void sampleRateChanged(double newSampleRate) override;

    // ----------------------------------------------------------------------------------------------------------------

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriceratopsPlugin)
};

END_NAMESPACE_DISTRHO

#endif // TRICERATOPS_PLUGIN_HPP
