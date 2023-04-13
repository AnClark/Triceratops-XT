/****************************************************************************
   engine.cpp - Triceratops synthesizer engine interface

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

#include "DistrhoPlugin.hpp"
#include "params.hpp"
#include "triceratops.hpp"
#include <cstdint>

Triceratops::Triceratops(double rate)
{
    Triceratops* const self = this;

    self->eq_left = new EQSTATE();
    init_3band_state(self->eq_left, 220, 5000, rate);
    self->eq_left->lg = 0.0; // BASS
    self->eq_left->mg = 1.0; // MIDS
    self->eq_left->hg = 1.0; // HIGHS

    self->eq_right = new EQSTATE();
    init_3band_state(self->eq_right, 220, 5000, rate);
    self->eq_right->lg = 0.0; // BASS
    self->eq_right->mg = 1.0; // MIDS
    self->eq_right->hg = 1.0; // HIGHS

    self->lfo1_out = (float*)malloc(sizeof(float) * 4096);
    self->lfo2_out = (float*)malloc(sizeof(float) * 4096);
    self->lfo3_out = (float*)malloc(sizeof(float) * 4096);

    memset(self->lfo1_out, 0, sizeof(float) * 4096);
    memset(self->lfo2_out, 0, sizeof(float) * 4096);
    memset(self->lfo3_out, 0, sizeof(float) * 4096);

    self->echo = new nixecho();
    self->echo->set_sample_rate(rate);
    self->nixnoise = new noise();

    self->pitch_bend    = new float();
    self->pitch_bend[0] = 0;

    self->channel_after_touch    = new float();
    self->channel_after_touch[0] = 0;

    for (int x = 0; x < 16; ++x) {
        self->reverb[x] = new JCRev((float)x / 3, rate);
        self->reverb[x]->clear();
        self->reverb[x]->setEffectMix(1.0);
    }

    self->lfo1_rand = self->nixnoise->tick();
    self->lfo2_rand = self->nixnoise->tick();
    self->lfo3_rand = self->nixnoise->tick();

    self->current_synth = 0;
    self->old_synth     = 0;
    for (int x = 0; x < 128; x++) {
        self->midi_keys[x] = -1;
    }

    // store the sample rate in "self" so we can retrieve it in run()
    self->sample_rate = rate;

    for (int x = 0; x < max_notes; ++x) {
        self->synths[x] = new synth(rate, "DPF instance"); // bundle path is not actually used by synth
    }

    self->lfo1 = new LFO(rate);
    self->lfo2 = new LFO(rate);
    self->lfo3 = new LFO(rate);

    self->lfo1_count = 0;
    self->lfo2_count = 0;
    self->lfo3_count = 0;

    for (int x = 0; x < max_notes; ++x) {
        self->synths[x]->lfo1_out            = &self->lfo1_out;
        self->synths[x]->lfo2_out            = &self->lfo2_out;
        self->synths[x]->lfo3_out            = &self->lfo3_out;
        self->synths[x]->pitch_bend          = self->pitch_bend;
        self->synths[x]->channel_after_touch = self->channel_after_touch;
    }

    _connectParamPorts();
}

void Triceratops::_connectParamPorts()
{
    Triceratops* const self = this;

    float** test[max_notes];

    for (uint8_t param_index = 0; param_index < TRICERATOPS_PARAM_COUNT; param_index++) {
        for (int x = 0; x < max_notes; ++x) {
            test[x]              = &self->synths[x]->synth_params->TRICERATOPS_MASTER_VOLUME;
            test[x][param_index] = &params[param_index];
        }
    }
}

Triceratops::~Triceratops()
{
    Triceratops* const self = this;

    delete (self->echo);
    delete (self->nixnoise);

    for (int x = 0; x < max_notes; x++) {

        // delete(self->synths[x]);
    }

    free(self->lfo1_out);
    free(self->lfo2_out);
    free(self->lfo3_out);
    delete (self->lfo1);
    delete (self->lfo2);
    delete (self->lfo3);
}

void Triceratops::run(float* output_left, float* output_right, uint32_t n_samples, const MidiEvent* midiEvents, uint32_t midiEventCount)
{
    Triceratops* const self = this;

    memset(output_left, 0, sizeof(double) * (n_samples * 0.5));
    memset(output_right, 0, sizeof(double) * (n_samples * 0.5));

    if (self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_ACTIVE[0] == 1) {
        self->echo->eq->lg  = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_EQ_LOW[0];
        self->echo->eq->mg  = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_EQ_MID[0];
        self->echo->eq->hg  = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_EQ_HIGH[0];
        self->echo->eq2->lg = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_EQ_LOW[0];
        self->echo->eq2->mg = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_EQ_MID[0];
        self->echo->eq2->hg = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_EQ_HIGH[0];

        self->echo->echo_decay = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_DECAY[0];
        self->echo->echo_speed = self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_SPEED[0];
    }

    int midi_channel = self->synths[0]->synth_params->TRICERATOPS_MIDI_CHANNEL[0] - 1;

    /* Read incoming events */
    // LV2_ATOM_SEQUENCE_FOREACH(self->control_port, ev)
    for (uint32_t ev_index = 0; ev_index < midiEventCount; ev_index++) {
        auto const data = midiEvents[ev_index].data;

        // midi note pressed

        // cout << (int)data[0] << "," << (int)data[1] << "," << (int)data[2] << endl;

        if (data[0] == 0xE0 + midi_channel) // PITCHBEND
        {
            self->pitch_bend[0] = ((float)(((int)data[2] * 128) + (int)data[1]) / 8192) - 1;
        }

        if (data[0] == 0xD0 + midi_channel) {
            self->channel_after_touch[0] = ((float)((int)data[1]) / 64);
        }

        if (data[0] == 0x90 + midi_channel && data[2] > 0) {

            int current_synth = self->current_synth;

            for (int x = 0; x < max_notes; ++x) {
                if (self->synths[x]->env_amp_state == self->synths[x]->env_state_release
                        && self->synths[x]->env_amp_level < 0.5
                    || self->synths[x]->env_amp_state == self->synths[x]->env_state_dormant) {
                    current_synth       = x;
                    self->current_synth = x;
                }
                if (self->synths[0]->synth_params->TRICERATOPS_LEGATO[0] == 1) {
                    current_synth       = 0;
                    self->current_synth = 0;
                }
            }

            self->synths[current_synth]->update_counter = -1;

            self->midi_keys[data[1]]              = current_synth;
            self->synths[current_synth]->midi_key = data[1];
            self->synths[current_synth]->velocity = data[2];

            float key_frequency = (44100 / fastishP2F(125 - data[1]));
            if (key_frequency > 18000)
                key_frequency = 18000;

            self->synths[current_synth]->dco_frequency = key_frequency;

            // IN LEGATO MODE ONLY USE INERTIA PORTAMENTO WHEN KEYS HELD DOWN

            if (self->synths[0]->synth_params->TRICERATOPS_LEGATO[0] == 1) {
                if (self->synths[current_synth]->env_amp_state == self->synths[current_synth]->env_state_release
                    || self->synths[current_synth]->env_amp_state == self->synths[current_synth]->env_state_dormant) {
                    self->synths[current_synth]->inertia_one.value = key_frequency
                                                                     * fastishP2F(12 * self->synths[current_synth]->synth_params->TRICERATOPS_OCTAVE_ONE[0]);
                    self->synths[current_synth]->inertia_two.value = key_frequency
                                                                     * fastishP2F(12 * self->synths[current_synth]->synth_params->TRICERATOPS_OCTAVE_TWO[0]);
                    self->synths[current_synth]->inertia_three.value = key_frequency
                                                                       * fastishP2F(12 * self->synths[current_synth]->synth_params->TRICERATOPS_OCTAVE_THREE[0]);
                }
            }

            // IN LEGATO MODE DO NOT RESET ADSR STATES TO ATTACK MODE UNLESS PREVIOUSLY IN RELEASE/DORMANT

            if (self->synths[0]->synth_params->TRICERATOPS_LEGATO[0] == 1
                && self->synths[current_synth]->env_amp_state != self->synths[current_synth]->env_state_release
                && self->synths[current_synth]->env_amp_state != self->synths[current_synth]->env_state_dormant) {

                if (self->synths[0]->synth_params->TRICERATOPS_LFO1_RETRIG[0] == 1) {
                    self->lfo1->phase = 0;
                    self->lfo1_count  = 0;
                }
                if (self->synths[0]->synth_params->TRICERATOPS_LFO2_RETRIG[0] == 1) {
                    self->lfo2->phase = 0;
                    self->lfo2_count  = 0;
                }
                if (self->synths[0]->synth_params->TRICERATOPS_LFO3_RETRIG[0] == 1) {
                    self->lfo3->phase = 0;
                    self->lfo3_count  = 0;
                }
            } else {

                // self->synths[current_synth]->lpO[0]->p = 0;
                self->synths[current_synth]->sinewave_osc[0]->phase = 0;
                // self->synths[current_synth]->lpO[1]->p = 0;
                self->synths[current_synth]->sinewave_osc[1]->phase = 0;
                // self->synths[current_synth]->lpO[2]->p = 0;
                self->synths[current_synth]->sinewave_osc[2]->phase = 0;
                // self->synths[current_synth]->lpO[3]->p = 0;
                self->synths[current_synth]->sinewave_osc[3]->phase = 0;
                // self->synths[current_synth]->lpO[4]->p = 0;
                self->synths[current_synth]->sinewave_osc[4]->phase = 0;
                // self->synths[current_synth]->lpO[5]->p = 0;
                self->synths[current_synth]->sinewave_osc[5]->phase = 0;

                self->synths[current_synth]->bf0_left = 0;
                self->synths[current_synth]->bf1_left = 0;
                self->synths[current_synth]->bf2_left = 0;
                self->synths[current_synth]->bf3_left = 0;
                self->synths[current_synth]->bf4_left = 0;

                self->synths[current_synth]->bf0_right = 0;
                self->synths[current_synth]->bf1_right = 0;
                self->synths[current_synth]->bf2_right = 0;
                self->synths[current_synth]->bf3_right = 0;
                self->synths[current_synth]->bf4_right = 0;

                // if (self->synths[0]->synth_params->TRICERATOPS_LFO3_RETRIG[0] !=1)
                self->synths[current_synth]->env_amp_level    = 0;
                self->synths[current_synth]->env_filter_level = 0;
                self->synths[current_synth]->env_lfo_level    = 0;

                self->synths[current_synth]->env_amp_state    = self->synths[current_synth]->env_state_attack;
                self->synths[current_synth]->env_filter_state = self->synths[current_synth]->env_state_attack;
                self->synths[current_synth]->env_lfo_state    = self->synths[current_synth]->env_state_attack;

                self->synths[current_synth]->inertia_one.value   = self->synths[self->old_synth]->inertia_one.value; // copies frequency for inertia
                self->synths[current_synth]->inertia_two.value   = self->synths[self->old_synth]->inertia_two.value;
                self->synths[current_synth]->inertia_three.value = self->synths[self->old_synth]->inertia_three.value;

                if (self->synths[0]->synth_params->TRICERATOPS_LFO1_RETRIG[0] == 1) {
                    self->lfo1->phase = 0;
                    self->lfo1_count  = 0;
                }
                if (self->synths[0]->synth_params->TRICERATOPS_LFO2_RETRIG[0] == 1) {
                    self->lfo2->phase = 0;
                    self->lfo2_count  = 0;
                }
                if (self->synths[0]->synth_params->TRICERATOPS_LFO3_RETRIG[0] == 1) {
                    self->lfo3->phase = 0;
                    self->lfo3_count  = 0;
                }

                self->old_synth = self->current_synth;
            }
        }

        // midi note depressed

        else if (data[0] == 0x80 + midi_channel || (data[0] & 0xF0 == 0x90 && data[2] == 0)) {
            for (int x = 0; x < max_notes; x++) {
                if (self->synths[x]->midi_key == data[1]) {
                    self->synths[x]->midi_key         = -1;
                    self->synths[x]->env_amp_state    = self->synths[x]->env_state_release;
                    self->synths[x]->env_filter_state = self->synths[x]->env_state_release;
                    self->synths[x]->env_lfo_state    = self->synths[x]->env_state_release;
                }
            }
        }
    }

    self->lfo1->setRate(self->synths[0]->synth_params->TRICERATOPS_LFO1_SPEED[0] / 60);
    self->lfo2->setRate(self->synths[0]->synth_params->TRICERATOPS_LFO2_SPEED[0] / 60);
    self->lfo3->setRate(self->synths[0]->synth_params->TRICERATOPS_LFO3_SPEED[0] / 60);

    int lfo1_wave = self->synths[0]->synth_params->TRICERATOPS_LFO1_WAVE[0];
    int lfo2_wave = self->synths[0]->synth_params->TRICERATOPS_LFO2_WAVE[0];
    int lfo3_wave = self->synths[0]->synth_params->TRICERATOPS_LFO3_WAVE[0];

    switch (lfo1_wave) {
        case 0:
            self->lfo1->setWaveform(LFO::sawtooth);
            break;

        case 1:
            self->lfo1->setWaveform(LFO::square);
            break;

        case 2:
            self->lfo1->setWaveform(LFO::sinus);
            break;

        case 5:
            self->lfo1->setWaveform(LFO::sawtooth);
            break;

        case 6:
            self->lfo1->setWaveform(LFO::square);
            break;
    }

    switch (lfo2_wave) {
        case 0:
            self->lfo2->setWaveform(LFO::sawtooth);
            break;

        case 1:
            self->lfo2->setWaveform(LFO::square);
            break;

        case 2:
            self->lfo2->setWaveform(LFO::sinus);
            break;

        case 5:
            self->lfo2->setWaveform(LFO::sawtooth);
            break;

        case 6:
            self->lfo2->setWaveform(LFO::square);
            break;
    }

    switch (lfo3_wave) {
        case 0:
            self->lfo3->setWaveform(LFO::sawtooth);
            break;

        case 1:
            self->lfo3->setWaveform(LFO::square);
            break;

        case 2:
            self->lfo3->setWaveform(LFO::sinus);
            break;

        case 5:
            self->lfo3->setWaveform(LFO::sawtooth);
            break;

        case 6:
            self->lfo3->setWaveform(LFO::square);
            break;
    }

    int lfo1_speed = (self->sample_rate * 30) / self->synths[0]->synth_params->TRICERATOPS_LFO1_SPEED[0];
    int lfo2_speed = (self->sample_rate * 30) / self->synths[0]->synth_params->TRICERATOPS_LFO2_SPEED[0];
    int lfo3_speed = (self->sample_rate * 30) / self->synths[0]->synth_params->TRICERATOPS_LFO3_SPEED[0];

    for (int x = max_notes - 1; x > -1; --x) {
        if (self->synths[x]->env_amp_state != self->synths[x]->env_state_dormant) {
            self->synths[x]->run(output_left, output_right, n_samples);
        }
    }

    for (uint32_t x = 0; x < n_samples; ++x) {

        // DO LFO1

        if (self->lfo1_count == 0) {
            self->lfo1_rand = self->nixnoise->tick();
        }

        if (lfo1_wave < 3)
            self->lfo1_out[x] = self->lfo1->tick();
        if (lfo1_wave == 5 || lfo1_wave == 6)
            self->lfo1_out[x] = 1 - self->lfo1->tick();

        if (lfo1_wave == 3) {
            float dummy       = self->lfo1->tick(); // keeps other LFO waveforms on tempo;
            self->lfo1_out[x] = self->lfo1_rand;
        }

        if (lfo1_wave == 4) {
            float dummy       = self->lfo1->tick(); // keeps other LFO waveforms on tempo;
            self->lfo1_out[x] = self->nixnoise->tick();
        }

        /*

        float routes = 0;
        float lfo1 = self->lfo1_out[x];
        float lfo2 = self->lfo2_out[x];
        float lfo3 = self->lfo3_out[x];

        if (self->synths[0]->synth_params->TRICERATOPS_LFO1_ROUTE_ONE_DEST[0]==12)
                routes += self->synths[0]->synth_params->TRICERATOPS_LFO1_ROUTE_ONE[0]* lfo1;

        if (self->synths[0]->synth_params->TRICERATOPS_LFO2_ROUTE_ONE_DEST[0]==12)
                routes += self->synths[0]->synth_params->TRICERATOPS_LFO2_ROUTE_ONE[0]* lfo2;

        if (self->synths[0]->synth_params->TRICERATOPS_LFO3_ROUTE_ONE_DEST[0]==12)
                routes += self->synths[0]->synth_params->TRICERATOPS_LFO3_ROUTE_ONE[0]* lfo3;

        if (self->synths[0]->synth_params->TRICERATOPS_LFO1_ROUTE_TWO_DEST[0]==12)
                routes += self->synths[0]->synth_params->TRICERATOPS_LFO1_ROUTE_TWO[0]* lfo1;

        if (self->synths[0]->synth_params->TRICERATOPS_LFO2_ROUTE_TWO_DEST[0]==12)
                routes += self->synths[0]->synth_params->TRICERATOPS_LFO2_ROUTE_TWO[0]* lfo2;

        if (self->synths[0]->synth_params->TRICERATOPS_LFO3_ROUTE_TWO_DEST[0]==12)
                routes += self->synths[0]->synth_params->TRICERATOPS_LFO3_ROUTE_TWO[0]* lfo3;

        */

        // DO LFO2

        if (self->lfo2_count == 0)
            self->lfo2_rand = self->nixnoise->tick();
        if (lfo2_wave == 5 || lfo2_wave == 6)
            self->lfo2_out[x] = 1 - self->lfo2->tick();

        if (lfo2_wave < 3)
            self->lfo2_out[x] = self->lfo2->tick();

        if (lfo2_wave == 3) {
            float dummy       = self->lfo2->tick(); // keeps other LFO waveforms on tempo;
            self->lfo2_out[x] = self->lfo2_rand;
        }

        if (lfo2_wave == 4) {
            float dummy       = self->lfo2->tick(); // keeps other LFO waveforms on tempo;
            self->lfo2_out[x] = self->nixnoise->tick();
        }

        // DO LFO3

        if (self->lfo3_count == 0)
            self->lfo3_rand = self->nixnoise->tick();

        if (lfo3_wave < 3)
            self->lfo3_out[x] = self->lfo3->tick();
        if (lfo3_wave == 5 || lfo3_wave == 6)
            self->lfo3_out[x] = 1 - self->lfo3->tick();

        if (lfo3_wave == 3) {
            float dummy       = self->lfo3->tick(); // keeps other LFO waveforms on tempo;
            self->lfo3_out[x] = self->lfo3_rand;
        }

        if (lfo3_wave == 4) {
            float dummy       = self->lfo3->tick(); // keeps other LFO waveforms on tempo;
            self->lfo3_out[x] = self->nixnoise->tick();
        }

        ++self->lfo1_count;
        if (self->lfo1_count > lfo1_speed)
            self->lfo1_count = 0;

        ++self->lfo2_count;
        if (self->lfo2_count > lfo2_speed)
            self->lfo2_count = 0;

        ++self->lfo3_count;
        if (self->lfo3_count > lfo3_speed)
            self->lfo3_count = 0;

        if (isnan(output_left[x]))
            output_left[x] = 0;
        if (isnan(output_right[x]))
            output_right[x] = 0;

        if (self->synths[0]->synth_params->TRICERATOPS_FX_ECHO_ACTIVE[0] == 1) {
            output_left[x]  = self->echo->do_left(output_left[x]);
            output_right[x] = self->echo->do_right(output_right[x]);
        }

        if (self->synths[0]->synth_params->TRICERATOPS_FX_REVERB_ACTIVE[0] == 1) {
            int reverb_decay = self->synths[0]->synth_params->TRICERATOPS_FX_REVERB_DECAY[0];
            self->reverb[reverb_decay]->setEffectMix(self->synths[0]->synth_params->TRICERATOPS_FX_REVERB_MIX[0]);
            self->reverb[reverb_decay]->tick(((output_left[x] + output_right[x]) * 0.2));
            output_left[x] += self->reverb[reverb_decay]->lastOutputL();
            output_right[x] += self->reverb[reverb_decay]->lastOutputR();
        }
    }
}

void Triceratops::setParam(ParamId index, float value)
{
    params[index] = value;
}

float Triceratops::getParam(ParamId index)
{
    return params[index];
}
