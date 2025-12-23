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

// public
TestSynth::TestSynth() // inherits from Plugin(uint32_t parameterCount, uint32_t programCount, uint32_t stateCount)
     : DISTRHO::Plugin(0,0,0) {
    }



// protected
// information

// Get the plugin version, in hexadecimal.
uint32_t TestSynth::getVersion() const {
    return d_version(0, 0, 1);
}

// Processing

void TestSynth::run(const float** inputs, float** outputs, uint32_t frames, const DISTRHO::MidiEvent* midiEvents, uint32_t midiEventCount) {

}



// entry point for DPF plugins
Plugin* DISTRHO::createPlugin() {
    return new TestSynth();
}