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
virtual void        run(const float** inputs, float** outputs, uint32_t frames, const DISTRHO::MidiEvent* midiEvents, uint32_t midiEventCount) override;

};

