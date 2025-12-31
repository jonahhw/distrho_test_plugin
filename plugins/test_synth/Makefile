#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# initially created by falkTX
# --------------------------------------------------------------

# Project name, used for binaries

NAME = test_synth

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	TestSynth.cpp \
	oscillators.cpp

# --------------------------------------------------------------
# Do some magic

include ../../DPF/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += lv2
# TARGETS += clap
# TARGETS += vst2
# TARGETS += vst3

all: $(TARGETS)

# --------------------------------------------------------------
