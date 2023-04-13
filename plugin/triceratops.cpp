/****************************************************************************
   triceratops.cpp - Triceratops DPF plugin interface

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

#include "triceratops.hpp"
#include "DistrhoUtils.hpp"
#include "params.hpp"

START_NAMESPACE_DISTRHO

TriceratopsPlugin::TriceratopsPlugin()
    : Plugin(ParamId::TRICERATOPS_PARAM_COUNT, 0, 0) // parameters, programs, states
{
    fSampleRate = getSampleRate();
    fBufferSize = getBufferSize();

    fSynthesizer = std::make_unique<Triceratops>(fSampleRate);
}

TriceratopsPlugin::~TriceratopsPlugin()
{
}

void TriceratopsPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    parameter.hints = kParameterIsAutomatable;

    parameter.name       = TriceratopsParam::paramName(ParamId(index));
    parameter.shortName  = TriceratopsParam::paramSymbol(ParamId(index));
    parameter.symbol     = TriceratopsParam::paramSymbol(ParamId(index));
    parameter.ranges.min = TriceratopsParam::paramMinValue(ParamId(index));
    parameter.ranges.max = TriceratopsParam::paramMaxValue(ParamId(index));
    parameter.ranges.def = TriceratopsParam::paramDefaultValue(ParamId(index));

    if (TriceratopsParam::paramInt(ParamId(index)))
        parameter.hints |= kParameterIsInteger;
    else if (TriceratopsParam::paramBool(ParamId(index)))
        parameter.hints |= kParameterIsBoolean;

    // Handle enumeration values

    switch (index) {
        case TRICERATOPS_WAVE_ONE:
        case TRICERATOPS_WAVE_TWO:
        case TRICERATOPS_WAVE_THREE:
            parameter.enumValues.count          = 4;
            parameter.enumValues.restrictedMode = true;
            {
                ParameterEnumerationValue* const values = new ParameterEnumerationValue[4];
                parameter.enumValues.values             = values;

                values[0].label = "Saw";
                values[0].value = 0;
                values[1].label = "Square";
                values[1].value = 1;
                values[2].label = "Sine";
                values[2].value = 2;
                values[3].label = "White Noise";
                values[3].value = 3;
            }
            break;

        case TRICERATOPS_LFO1_WAVE:
        case TRICERATOPS_LFO2_WAVE:
        case TRICERATOPS_LFO3_WAVE:
            parameter.enumValues.count          = 7;
            parameter.enumValues.restrictedMode = true;
            {
                ParameterEnumerationValue* const values = new ParameterEnumerationValue[7];
                parameter.enumValues.values             = values;

                values[0].label = "Saw";
                values[0].value = 0;
                values[1].label = "Square";
                values[1].value = 1;
                values[2].label = "Sine";
                values[2].value = 2;
                values[3].label = "S&H";
                values[3].value = 3;
                values[4].label = "White Noise";
                values[4].value = 4;
                values[5].label = "Inv. Saw";
                values[5].value = 5;
                values[6].label = "Inv. Square";
                values[6].value = 6;
            }
            break;

        case TRICERATOPS_FILTER_MODE:
            parameter.enumValues.count          = 5;
            parameter.enumValues.restrictedMode = true;
            {
                ParameterEnumerationValue* const values = new ParameterEnumerationValue[5];
                parameter.enumValues.values             = values;

                values[0].label = "OFF";
                values[0].value = 0;
                values[1].label = "LPF";
                values[1].value = 1;
                values[2].label = "HPF";
                values[2].value = 2;
                values[3].label = "BPF";
                values[3].value = 3;
                values[4].label = "FRMNT";
                values[4].value = 4;
            }
            break;
    }

    setParameterValue(index, parameter.ranges.def);
}

float TriceratopsPlugin::getParameterValue(uint32_t index) const
{
    if (fSynthesizer == nullptr)
        return 0.0f;

    return fSynthesizer->getParam((ParamId)index);
}

void TriceratopsPlugin::setParameterValue(uint32_t index, float value)
{
    if (fSynthesizer != nullptr)
        fSynthesizer->setParam((ParamId)index, value);
}

void TriceratopsPlugin::activate()
{
    if (fSynthesizer != nullptr)
        fSynthesizer->activate();
}

void TriceratopsPlugin::run(const float** inputs, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount)
{
    if (fSynthesizer == nullptr)
        return;

    fSynthesizer->run(outputs[0], outputs[1], frames, midiEvents, midiEventCount);

    (void)inputs; // Avoid compiler warning
}

// Reinit synth engine on buffer size change.
void TriceratopsPlugin::bufferSizeChanged(int newBufferSize)
{
    if (fBufferSize != newBufferSize) {
        d_stderr("[DSP] Buffer size changed: from %d to %d", fBufferSize, newBufferSize);

        fBufferSize = newBufferSize;

        // Reinit synth engine
        fSynthesizer.reset();
        fSynthesizer = std::make_unique<Triceratops>(fSampleRate);
    } else {
        d_stderr("[DSP] Buffer size changed: same as current value, %d", fBufferSize);
    }
}

// Reinit synth engine on sample rate change.
void TriceratopsPlugin::sampleRateChanged(double newSampleRate)
{
    if (fSampleRate != newSampleRate) {
        d_stderr("[DSP] Sample rate changed: from %f to %f", fSampleRate, newSampleRate);

        fSampleRate = newSampleRate;

        // Reinit synth engine
        fSynthesizer.reset();
        fSynthesizer = std::make_unique<Triceratops>(fSampleRate);
    } else {
        d_stderr("[DSP] Sample rate changed: same as current value, %f", fSampleRate);
    }
}

Plugin* createPlugin()
{
    return new TriceratopsPlugin();
}

END_NAMESPACE_DISTRHO
