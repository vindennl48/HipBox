#!/bin/bash

jack_control start
jack_control ds alsa
jack_control dps device hw:USB
#jack_control dps device hw:Pro
jack_control dps rate 44100
jack_control dps nperiods 2
jack_control dps period 128
