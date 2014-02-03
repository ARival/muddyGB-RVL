Boueux.gb
=========

Boueux.gb is a clone of MuddyGB, a music progam for the Nintendo GameBoy.
For directions on compiling from source, see INSTALL.

Usage
-----

The directional pad acts to play the notes of the chosen scale.

The START button raises and lowers the scale by an octave.

The SELECT button acts to select options.
SELECT + RIGHT selects the current mode.
SELECT + UP or DOWN selects the scale tonic (base note).
SELECT + LEFT selects the waveform.

Available modes
---------------

Ionian (Major), Aeolian (Natural minor), Harmonic minor,
Blues, Dorian, Lydian, Whole-tone

(see scales.c)

Available waveforms
-------------------

* Pulse waves at 50%, 25% and 12.5% duty cycle

* A saw-like wave (12.5% + 25%)

* "Wavering" (CH2 slightly higher frequency than CH1)

* Pulse waves perfect 5ths apart

(see `play_note` and `update_duty_cycle` in boueux.c)

For future versions
-------------------

The A button will pitch bend the note being played.
The B button will raise the pitch by a perfect 4th.
