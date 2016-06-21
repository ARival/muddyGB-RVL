MuddyGB_DSD.gb
=========

MuddyGB_DSD.gb is a clone of MuddyGB, a music progam for the Nintendo GameBoy.

Latest binaries can be found
[here](https://github.com/LokkoLori/muddyGB-DSD/releases).

To compile from source, run `make` with GBDK installed.
There are currently no configuration options.

Usage
-----

Usage is as in MuddyGB.

* The directional pad acts to play the notes of the chosen scale.
* The START button raises and lowers the scale by an octave.
* The A button lowers the pitch by a semitone.
* The B button raises the pitch by a perfect 4th.
* The SELECT button acts to select options.
  * SELECT + RIGHT selects the current mode.
  * SELECT + UP or DOWN selects the scale tonic (base note).
  * SELECT + LEFT selects the waveform.

Available modes
---------------

Ionian (Major), Aeolian (Natural minor), Harmonic minor,
Blues, Dorian, Lydian, Whole-tone

(see scales.c)

Available waveforms
-------------------

* Pulse waves at 50%, 25% and 12.5% duty cycle
* A saw-like wave (12.5% + 25% pulse waves)
* "Wavering" (CH2 slightly higher frequency than CH1)
* Pulse waves perfect 5ths apart
* "Wawa-ing" pulse waves 
* Echoing pulse waves

(see `play_note` and `update_waveform` in boueux.c)

For future versions
-------------------

The A button will glide the pitch up and down a semitone like in MuddyGB,
instead of dropping straight down.
