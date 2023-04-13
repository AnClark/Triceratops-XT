/****************************************************************************
   params.cpp - Triceratops parameter manager

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

#include "params.hpp"

static const struct ParamDefinition triceratops_params[ParamId::TRICERATOPS_PARAM_COUNT] = {
    // symbol, name, min, max, def, type
    { "master_volume", "Volume", 0.000, 2.000, 0.750, PARAM_FLOAT },
    { "master_tune", "Master tune", -7, 7, 0, PARAM_FLOAT },
    { "amp_drive", "Drive", 0, 80, 0, PARAM_FLOAT },
    { "filter_mode", "Filter mode", 0, 4, 1, PARAM_INT },
    { "cutoff", "Filter cutoff", 0.00001, 0.88, 0.5, PARAM_FLOAT },
    { "resonance", "Filter resonance", 0.00001, 2, 0.42000383, PARAM_FLOAT },
    { "filter_key_follow", "Filter key follow", -1, 1, 0, PARAM_FLOAT },
    { "legato", "Legato", 0, 1, 0, PARAM_BOOL },
    { "sync", "Sync", 0, 1, 0, PARAM_BOOL },
    { "warmth", "Warmth", 0, 1, 1, PARAM_FLOAT },
    { "fm", "FM", 0, 1, 0, PARAM_BOOL },
    { "panic", "Panic", 0, 1, 0, PARAM_FLOAT },
    { "active_one", "Active 1", 0, 1, 1, PARAM_BOOL },
    { "volume_one", "Volume 1", 0, 1, 1, PARAM_FLOAT },
    { "pulsewidth_one", "pulsewidth 1", -0.5, 0.5, 0, PARAM_FLOAT },
    { "wave_one", "Waveform 1", 0, 3, 0, PARAM_INT },
    { "octave_one", "Octave 1", -5, 5, -2, PARAM_INT },
    { "detune_one", "Detune 1", -7.0000, 7.0000, 0.0000, PARAM_FLOAT },
    { "detune_centre_one", "Detune centre 1", 0, 1, 1, PARAM_BOOL },
    { "inertia_one", "Inertia 1", 0, 1, 0, PARAM_FLOAT },
    { "active_two", "Active 2", 0, 1, 1, PARAM_BOOL },
    { "volume_two", "Volume 2", 0, 1, 1, PARAM_FLOAT },
    { "pulsewidth_two", "pulsewidth 2", -0.5, 0.5, 0, PARAM_FLOAT },
    { "wave_two", "Waveform 2", 0, 3, 0, PARAM_INT },
    { "octave_two", "Octave 2", -5, 5, -2, PARAM_INT },
    { "detune_two", "Detune 2", -7.0000, 7.0000, 0.1000, PARAM_FLOAT },
    { "detune_centre_two", "Detune centre 2", 0, 1, 1, PARAM_BOOL },
    { "inertia_two", "Inertia 2", 0, 1, 0, PARAM_FLOAT },
    { "active_three", "Active 3", 0, 1, 1, PARAM_BOOL },
    { "volume_three", "Volume 3", 0, 1, 1, PARAM_FLOAT },
    { "pulsewidth_three", "pulsewidth 3", -0.5, 0.5, 0, PARAM_FLOAT },
    { "wave_three", "Waveform 3", 0, 3, 0, PARAM_INT },
    { "octave_three", "Octave 3", -5, 5, -2, PARAM_INT },
    { "detune_three", "Detune 3", -7.0000, 7.0000, -0.1000, PARAM_FLOAT },
    { "detune_centre_three", "Detune centre 3", 0, 1, 0, PARAM_BOOL },
    { "inertia_three", "Inertia 3", 0, 1, 0, PARAM_FLOAT },
    { "attack_one", "Attack 1", 0, 1, 0, PARAM_FLOAT },
    { "decay_one", "Decay 1", 0, 1, 0.75, PARAM_FLOAT },
    { "sustain_one", "Sustain 1", 0, 1, 0, PARAM_FLOAT },
    { "release_one", "Release 1", 0, 1, 0.3, PARAM_FLOAT },
    { "adsr1_route_one", "Route one", 0, 1, 1, PARAM_FLOAT },
    { "adsr1_route_one_dest", "Route one dest", 0, 14, 0, PARAM_FLOAT },
    { "adsr1_route_two", "Route two", 0, 1, 0, PARAM_FLOAT },
    { "adsr1_route_two_dest", "Route one dest", 0, 14, 3, PARAM_FLOAT },
    { "attack_two", "Attack 2", 0, 1, 0, PARAM_FLOAT },
    { "decay_two", "Decay 2", 0, 1, 0.5, PARAM_FLOAT },
    { "sustain_two", "Sustain 2", 0, 1, 0, PARAM_FLOAT },
    { "release_two", "Release 2", 0, 1, 0, PARAM_FLOAT },
    { "adsr2_route_one", "Route one", 0, 1, 0.4, PARAM_FLOAT },
    { "adsr2_route_one_dest", "Route one dest", 0, 14, 1, PARAM_FLOAT },
    { "adsr2_route_two", "Route two", 0, 1, 0, PARAM_FLOAT },
    { "adsr2_route_two_dest", "Route two dest", 0, 14, 4, PARAM_FLOAT },
    { "attack_three", "Attack 3", 0, 1, 0, PARAM_FLOAT },
    { "decay_three", "Decay 3", 0, 1, 0, PARAM_FLOAT },
    { "sustain_three", "Sustain 3", 0, 1, 0, PARAM_FLOAT },
    { "release_three", "Release 3", 0, 1, 0, PARAM_FLOAT },
    { "adsr3_lfo1_amount", "To LFO 1", 0, 1, 0, PARAM_BOOL },
    { "adsr3_lfo2_amount", "To LFO 2", 0, 1, 0, PARAM_BOOL },
    { "adsr3_lfo3_amount", "To LFO 3", 0, 1, 0, PARAM_BOOL },
    { "lfo1_retrig", "LFO1 RETRIG", 0, 1, 0, PARAM_BOOL },
    { "lfo1_speed", "LFO1 SPEED", 1, 600, 120, PARAM_FLOAT },
    { "lfo1_wave", "LFO1 WAVE", 0, 6, 0, PARAM_INT },
    { "lfo1_dco1_pitch", "LFO1 TO DCO1 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo1_dco2_pitch", "LFO1 TO DCO2 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo1_dco3_pitch", "LFO1 TO DCO3 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo1_filter", "LFO1 TO FILTER", 0, 0.25, 0, PARAM_FLOAT },
    { "lfo1_route_one", "LFO1 ROUTE ONE", 0, 1, 0, PARAM_FLOAT },
    { "lfo1_route_one_dest", "LFO1 ROUTE ONE DEST", 0, 10, 2, PARAM_FLOAT },
    { "lfo1_route_two", "LFO1 ROUTE TWO", 0, 1.0, 0, PARAM_FLOAT },
    { "lfo1_route_two_dest", "LFO1 ROUTE TWO DEST", 0, 10, 0, PARAM_FLOAT },
    { "lfo2_retrig", "LFO2 RETRIG", 0, 1, 0, PARAM_BOOL },
    { "lfo2_speed", "LFO2 SPEED", 1, 600, 120, PARAM_FLOAT },
    { "lfo2_wave", "LFO2 WAVE", 0, 6, 0, PARAM_INT },
    { "lfo2_dco1_pitch", "LFO2 TO DCO1 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo2_dco2_pitch", "LFO2 TO DCO2 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo2_dco3_pitch", "LFO2 TO DCO3 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo2_filter", "LFO2 TO FILTER", 0, 0.25, 0, PARAM_FLOAT },
    { "lfo2_route_one", "LFO2 ROUTE ONE", 0, 1, 0, PARAM_FLOAT },
    { "lfo2_route_one_dest", "LFO2 ROUTE ONE DEST", 0, 10, 0, PARAM_FLOAT },
    { "lfo2_route_two", "LFO2 ROUTE ", 0, 1.0, 0, PARAM_FLOAT },
    { "lfo2_route_two_dest", "LFO2 ROUTE TWO DEST", 0, 10, 0, PARAM_FLOAT },
    { "lfo3_retrig", "LFO3 RETRIG", 0, 1, 0, PARAM_BOOL },
    { "lfo3_speed", "LFO3 SPEED", 1, 600, 120, PARAM_FLOAT },
    { "lfo3_wave", "LFO3 WAVE", 0, 6, 0, PARAM_INT },
    { "lfo3_dco1_pitch", "LFO3 TO DCO1 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo3_dco2_pitch", "LFO3 TO DCO2 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo3_dco3_pitch", "LFO3 TO DCO3 PITCH", 0, 1, 0, PARAM_FLOAT },
    { "lfo3_filter", "LFO3 TO FILTER", 0, 0.25, 0, PARAM_FLOAT },
    { "lfo3_route_one", "LFO3 ROUTE ONE", 0, 1, 0, PARAM_FLOAT },
    { "lfo3_route_one_dest", "LFO3 ROUTE ONE DEST", 0, 10, 0, PARAM_FLOAT },
    { "lfo3_route_two", "LFO3 ROUTE ", 0, 1.0, 0, PARAM_FLOAT },
    { "lfo3_route_two_dest", "LFO3 ROUTE TWO DEST", 0, 10, 0, PARAM_FLOAT },
    { "fx_echo_active", "Echo active", 0, 1, 0, PARAM_BOOL },
    { "fx_echo_speed", "Echo speed", 512, 65536, 4096, PARAM_FLOAT },
    { "fx_echo_decay", "Echo decay", 0, 0.9, 0.25, PARAM_FLOAT },
    { "fx_echo_eq_low", "Echo eq low", 0, 2, 1, PARAM_FLOAT },
    { "fx_echo_eq_mid", "Echo eq mid", 0, 2, 1, PARAM_FLOAT },
    { "fx_echo_eq_high", "Echo eq high", 0, 2, 1, PARAM_FLOAT },
    { "unison_activate", "Unison activate", 0, 1, 0, PARAM_BOOL },
    { "unison_one", "DCO1 Unison", 0, 7, 0, PARAM_FLOAT }, // NOTE: The right unison range should be [0, 7], but not [0, 1]
    { "unison_two", "DCO2 Unison", 0, 7, 0, PARAM_FLOAT },
    { "unison_three", "DCO3 Unison", 0, 7, 0, PARAM_FLOAT },
    { "modifier_dirt", "Modifier - dirt level", 0, 1, 0, PARAM_FLOAT },
    { "fx_reverb_active", "Reverb active", 0, 1, 0, PARAM_BOOL },
    { "fx_reverb_decay", "Reverb decay", 0, 15, 4, PARAM_FLOAT },
    { "fx_reverb_mix", "Reverb mix", 0, 0.8, 0, PARAM_FLOAT },
    { "stereo_mode", "Stereo mode", 0, 1, 1, PARAM_BOOL },
    { "dco1_pan", "DCO1 Pan", 0, 1, 0.5, PARAM_FLOAT },
    { "dco2_pan", "DCO2 Pan", 0, 1, 0.5, PARAM_FLOAT },
    { "dco3_pan", "DCO3 Pan", 0, 1, 0.5, PARAM_FLOAT },
    { "modifier_ring", "Modifier - ring", 0, 1, 0, PARAM_BOOL },
    { "preset_category", "Preset category", 0, 1, 0, PARAM_FLOAT },
    { "pitch_bend_range", "Pitch bend range", 0, 24, 24, PARAM_FLOAT },
    { "midi_channel", "Midi Channel", 1, 16, 1, PARAM_FLOAT }
};

const char* TriceratopsParam::paramName(ParamId index)
{
    return triceratops_params[index].name;
}

const char* TriceratopsParam::paramSymbol(ParamId index)
{
    return triceratops_params[index].symbol;
}

float TriceratopsParam::paramDefaultValue(ParamId index)
{
    return triceratops_params[index].def;
}

float TriceratopsParam::paramMinValue(ParamId index)
{
    return triceratops_params[index].min;
}

float TriceratopsParam::paramMaxValue(ParamId index)
{
    return triceratops_params[index].max;
}

bool TriceratopsParam::paramFloat(ParamId index)
{
    return (triceratops_params[index].type == PARAM_FLOAT);
}

bool TriceratopsParam::paramBool(ParamId index)
{
    return (triceratops_params[index].type == PARAM_BOOL);
}

bool TriceratopsParam::paramInt(ParamId index)
{
    return (triceratops_params[index].type == PARAM_INT);
}
