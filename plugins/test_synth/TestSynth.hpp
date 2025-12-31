/*
TestSynth.hpp
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

#include "../../DPF/distrho/DistrhoPlugin.hpp"
#include <unordered_map>

#include <oscillators.hpp>

class TestSynth : public DISTRHO::Plugin {
public:
    TestSynth(); // inherits from Plugin(uint32_t parameterCount, uint32_t programCount, uint32_t stateCount)

protected:
// information
// Get the plugin label, consisten of _, a-z, A-Z and 0-9 characters
virtual const char* getLabel() const override { return "TestSynth"; }

// Get the plugin author
virtual const char* getMaker() const override { return "Jonah Hamer-Wilson"; }

// Get the plugin license name
virtual const char* getLicense() const override { return "ISC"; }

// Get the plugin unique Id, used by LADSPA, DSSI, VST2 and VST3 plugin formats.
virtual int64_t getUniqueId() const override { return d_cconst('T', 'e', 's', 't'); }

// Get the plugin version, in hexadecimal.
virtual uint32_t getVersion() const override; // set in cpp file as it needs to be modified often

// Processing
virtual void run(const float** inputs, float** outputs, uint32_t frames, const DISTRHO::MidiEvent* midiEvents, uint32_t midiEventCount) override;

// misc
virtual void activate() override;
virtual void deactivate() override;
virtual void sampleRateChanged (double newSampleRate) override;

void update_frequency_coefficient(uint16_t new_frequency_value);

struct MIDI_Message_Type {enum MIDI_message_type : uint8_t {
    note_off             = 0x00,
    note_on              = 0x10,
    polyphonic_aftertouch= 0x20,
    control_change       = 0x30, // also includes channel mode messages
    program_change       = 0x40,
    channel_aftertouch   = 0x50,
    pitch_bend           = 0x60,
    system_common        = 0x70,
};};


// processing (internal)
void process_midi_event(const DISTRHO::MidiEvent& midi_event);

// properties
double sample_period;
uint64_t frames_since_start;

float frequency_coefficient;
const float max_frequency_coefficient_st = 2; // maximum deviation from center frequency in semitones

std::unordered_map<uint8_t, Note> active_notes; // Information about currently active notes, indexed by the note number
typedef std::unordered_map<uint8_t, Note>::iterator Active_Notes_it;

Signal_Generator signal_generator;
Oscillator* sin_osc;
};

