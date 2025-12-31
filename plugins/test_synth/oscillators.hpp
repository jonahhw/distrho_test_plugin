/*
oscillators.hpp
Oscillator classes and functions, written by Jonah Hamer-Wilson using the Distrho plugin framework

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

struct Note {
    Note(uint8_t note_number_in, uint8_t velocity_in, uint32_t frames_until_press);
    Note();
    uint8_t note_number;
    uint8_t velocity;

    float frequency;
    float phase;
    int32_t frames_since_pressed;

    float get_frequency_from_note_number(uint8_t note_number);
};

class Oscillator {
    // abstract base class for all oscillators
    public:
    Oscillator(float phase_shift, float duty_cycle_in);
    virtual ~Oscillator() {};

    virtual float evaluate(float phase) = 0;    // phase is normalized between 0 and 1. Result should be equal to 0 at phase=0.

    protected:
    float phase_offset;
    float duty_cycle;  // generalized, modifies phase before passing into the signal function. Not yet implemented.
};

class Signal_Generator {
    public:
    Signal_Generator(Oscillator* osc, double* sample_period_in, float* frequency_coefficient);
    Signal_Generator();
    protected:
    Oscillator* oscillator; // can be a list in the future
    // Envelope* envelope; // to implement later
    const float* pitch_bend_coefficient;
    const double* sample_period;

    public:
    float pop_time_step(Note& current_note); // advance time, then get the value
};

class Sine_Oscillator : public Oscillator {
    public:
    Sine_Oscillator(float phase_shift = 0, float duty_cycle_in = 0.5) : Oscillator(phase_shift, duty_cycle_in) {}

    protected:
    virtual float evaluate(float phase) override;
};
