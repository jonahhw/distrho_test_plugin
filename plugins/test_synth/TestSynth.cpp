/*
TestSynth.cpp
Test synthesizer, written by Jonah Hamer-Wilson using the Distrho plugin framework

License:
Copyright (C) 2025 Jonah Hamer-Wilson <updates@jonahhw.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

#include "TestSynth.hpp"
#include <cmath>

#define ENABLE_LOGGING false
#if ENABLE_LOGGING
#include <stdio.h>
#endif

// public
TestSynth::TestSynth() // inherits from Plugin(uint32_t parameterCount, uint32_t programCount, uint32_t stateCount)
     : DISTRHO::Plugin(0,0,0) {
    }

// protected
// misc
void TestSynth::activate() {
    if (ENABLE_LOGGING) printf("Sample rate: %f. Buffer size: %u\n", getSampleRate(), getBufferSize());
    if (ENABLE_LOGGING) printf("C++ version: %ld\n", __cplusplus);

    sample_period = 1/getSampleRate();

    frames_since_start = 0;

    frequency_coefficient = 1.f;

    sin_osc = new Sine_Oscillator(0, 0.5);
    signal_generator = Signal_Generator(sin_osc, &sample_period, &frequency_coefficient);
}
void TestSynth::deactivate() {
    delete sin_osc;
}

void TestSynth::update_frequency_coefficient(uint16_t new_frequency_value) {
    // Pitch bend has 14 bits of information, so the maximum possible value is 0x3fff, or 16383
    // This leaves us with a center value of 8192.
    const uint16_t mid_value = 0x2000;
    frequency_coefficient = float(new_frequency_value - mid_value)/mid_value;
    frequency_coefficient = pow(2, (max_frequency_coefficient_st/12)*frequency_coefficient);
}

void TestSynth::sampleRateChanged (double newSampleRate) {
    sample_period = 1/newSampleRate;
    if (ENABLE_LOGGING) printf("Sample rate: %f (%f)\n", getSampleRate(), newSampleRate);
}

// information

// Get the plugin version, in hexadecimal.
uint32_t TestSynth::getVersion() const {
    return d_version(0, 0, 1);
}

// Processing

void TestSynth::run(const float** /* inputs*/, float** outputs, uint32_t frames, const DISTRHO::MidiEvent* midiEvents, uint32_t midiEventCount) {
    // process MIDI events
    for (uint32_t m_idx = 0; m_idx < midiEventCount; ++m_idx) {
        process_midi_event(midiEvents[m_idx]);
    }

    // define useful shorthand
    float* const outL = outputs[0];
    float* const outR = outputs[1];

    // play notes
    for (uint32_t f_idx = 0; f_idx < frames; ++f_idx) {
        outL[f_idx] = 0;

        for (Active_Notes_it notes_it = active_notes.begin(); notes_it != active_notes.end(); notes_it++) {
            outL[f_idx] += signal_generator.pop_time_step(notes_it->second);
        }

        frames_since_start += 1;
        outR[f_idx] = outL[f_idx]; // plugin is mono for now; stereo may be introduced later
    }
}
void TestSynth::process_midi_event(const DISTRHO::MidiEvent& midi_event) {
    uint8_t message_type = midi_event.data[0] & 0x70;
    switch (message_type) {
    case MIDI_Message_Type::note_off: {
        uint8_t note_number = midi_event.data[1] & 0x7f;
        // uint8_t release_velocity = midi_event.data[2] & 0x7f; // no effect for release velocity yet

        active_notes.erase(note_number);
    } break;
    case MIDI_Message_Type::note_on: {
        uint8_t note_number = midi_event.data[1] & 0x7f;
        uint8_t press_velocity = midi_event.data[2] & 0x7f;

        Note new_note = Note(note_number, press_velocity, midi_event.frame);

        if (ENABLE_LOGGING) printf("Note pressed! Note number: %u. Frequency: %f. frames since pressed: %d \n", note_number, new_note.frequency, new_note.frames_since_pressed);
        
        active_notes[note_number] = new_note;
    } break;
    case MIDI_Message_Type::polyphonic_aftertouch: {
        // uint8_t note_number = midi_event.data[1] & 0x7f;
        // uint8_t pressure = midi_event.data[2] & 0x7f;
        // no effect currently
    } break;
    case MIDI_Message_Type::control_change: {
        // uint8_t control_number = midi_event.data[1] & 0x7f;
        // uint8_t value = midi_event.data[2] & 0x7f;
        // no effect currently
    } break;
    case MIDI_Message_Type::program_change: {
        // uint8_t program = midi_event.data[1] & 0x7f;
        // no effect currently
    } break;
    case MIDI_Message_Type::channel_aftertouch: {
        // uint8_t pressure = midi_event.data[1] & 0x7f;
        // no effect currently
    } break;
    case MIDI_Message_Type::pitch_bend: {
        uint8_t LSB = midi_event.data[1] & 0x7f;
        uint8_t MSB = midi_event.data[2] & 0x7f;
        uint16_t pitch_bend = (MSB << 7) + LSB;

        update_frequency_coefficient(pitch_bend);
    } break;
    case MIDI_Message_Type::system_common: {
        // no effect currently
    } break;
    default:
        // should never reach this
        break;
    }
}

// entry point for DPF plugins
Plugin* DISTRHO::createPlugin() {
    return new TestSynth();
}