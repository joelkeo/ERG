<img width="806" alt="Screenshot 2025-04-17 at 2 11 31 PM" src="https://github.com/user-attachments/assets/5f6e1496-38d4-47c9-9f93-dc62842213b3" />

# Inertia: a rhythm generating MIDI arpeggiator .vst plugin
### uses a novel approach for generating rhythms which change dynamically but start and end exactly on beat, great for "exponential rhythm" generation
#### .vst3 download and support: https://discord.gg/Z8BCBDgh

## How-to (install and use)
https://www.youtube.com/watch?v=BA2aAcK3R1I&t=12s&ab_channel=Inferno

## Overview
- Real-time MIDI processing with low latency
- Custom, edittable arpeggio-note editor
- Intuitive, custom user interface
- Built using modern C++ and the JUCE framework

## MIDI Pipeline
- Found in `Synth.h/.cpp`
- Works on a per-audio-buffer basis
- Multiple stages required since the user can generate various phase signals, and can create arbitrary arp patterns
1. Registers new MIDI messages recieved form host (in `MidiScheduler.h/.cpp`), and handles events (such as when a point in time is found with no active host notes)
2. Computes the phase signal from the current buffer (in `PhaseSignal.h/.cpp`), but real logic is held in (`VRPS.h/.cpp` and its inheriting classes)
3. Computes arp notes based on phase signal: notes which start at 0 (for "All" currently held down noes), - 127 (# represents number of note that is currently held from host starting at the bottom note, going upwards) (in `ArpPattern.h/.cpp`)
4. Converts arp notes to host understandable MIDI notes (back in `MidiScheduler.h/.cpp`)

## Classes explanation
- `PluginProcessor.h/.cpp`: Inherits from JUCE's `AudioProcessor` class. Root of MIDI handling, and parameter and plugin state management. Real pipline logic found in `Synth.h/.cpp`
- `PluginEditor.h/.cpp`: Root of UI handling
- `ArpPattern.h/.cpp`: Handles arpeggio note logic including converting phase signals to arpeggio notes
- `ArpPatternComponent.h/.cpp`: Renders arpeggio pattern visually and forwards (normalized) editting events onto ArpPattern
- `PolynomialPhaseSignal.h/cpp`: Handles phase signal generation based on the parameters. Inherits from VRPS which generalizes variable rate phase signals (for when I want to include Exponential and other phase signals)
- `EnvelopeComponent.h/cpp`: Renders visually the phase and rate (deriviate of phase) signals received from the VRPS. Also visualizes current phase/rate location as notes are held down. Works for aribtrary phase/rate signals, uses many binary searches to find phase integer multiples (which modulo to zero-crossings)
- `MidiScheduler.h/.cpp`: Handles MIDI info received from host, and converts ArpNote data into actual MIDI notes

## Basic idea behind the math:
- We usually think of traversal through a MIDI pattern in terms of the rate
- However, if we consider the rate's antiderivate (phase), we can with much more precision align the traversal start and end to the host's rhythmic units (beats, bars, etc.) even if the rate is variable
- In our case a traveral typically consists of multiple looped arp patterns, but this concept is expandable (eg. a single pattern, targetting modulation signals, etc.)
- The goal is to have the phase start and end at integer multiples: which, when moduloed w/ 1, represent the very beginning of the arp pattern
- If the phase starts and ends at integer multiples, the rhythm will align to the hosts rhythm at the beginning at end, regardless of how it may move in-between
- this desmos graph I created visualizes this quite well: https://www.desmos.com/calculator/aijan9qq9g
- this demo demonstrates how the plugin may be used in a simple context. The last trigger of the vocal sample via inertia *would* happen exactly at Bar 9 in FL Studio's playlist: https://www.youtube.com/watch?v=YOqxku7qMeM&ab_channel=Inferno
