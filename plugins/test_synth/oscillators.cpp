/*
oscillators.cpp
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

#include "oscillators.hpp"

Note::Note(uint8_t note_number_in, uint8_t velocity_in, uint32_t frames_until_press) {
    note_number = note_number_in;
    velocity = velocity_in;

    frequency = get_frequency_from_note_number(note_number);
    phase = 0;
    frames_since_pressed = -frames_until_press;
}

Note::Note() {
    // invalid Note object, but required by unordered_map
    note_number = 0;
    velocity = 0;
    frequency = 0;
    phase = 0;
    frames_since_pressed = 0;
}

float Note::get_frequency_from_note_number(uint8_t note_number) {
    return 440*pow(2, (note_number-69)/12.f);
}

Oscillator::Oscillator(float phase_shift, float duty_cycle_in) {
    phase_offset = phase_shift;
    duty_cycle = duty_cycle_in;
}

Signal_Generator::Signal_Generator(Oscillator* osc, double* sample_period_in, float* frequency_coefficient) {
    oscillator = osc;
    pitch_bend_coefficient = frequency_coefficient;
    sample_period = sample_period_in;
}

Signal_Generator::Signal_Generator() {
    oscillator = nullptr;
    pitch_bend_coefficient = nullptr;
    sample_period = nullptr;
}

float Signal_Generator::pop_time_step(Note& current_note) {
    current_note.frames_since_pressed += 1;

    // do not sound if the note hasn't started yet
    if (current_note.frames_since_pressed <= 1) {
        return 0;
    }

    // calculate phase
    float effective_frequency = current_note.frequency * (*pitch_bend_coefficient);
    current_note.phase += effective_frequency * (*sample_period);
    if (current_note.phase > 1) {
        current_note.phase -= 1;
    }

    // later: do envelope-related calculations. For now just use velocity
    float rel_velocity = current_note.velocity/127.f;

    return 0.5*rel_velocity * oscillator->evaluate(current_note.phase);
}

float Sine_Oscillator::evaluate(float phase) {
    return sin(2*M_PI*phase);
}