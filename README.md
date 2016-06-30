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
* The A button bent up the pitch by a full tone.
* The B button drop down the pitch by a semitone.
* The START button raise the pitch by an octave
* The SELECT button lower the pitch by an octave
* while SELECT + START pressed got the options mode
  * + RIGHT selects the current mode.
  * + UP or DOWN selects the scale tonic (base note).
  * + LEFT selects the waveform.

Available modes
---------------

major, minor, Blues

(see scales.c)

Available waveforms
-------------------

* Pulse waves at 50%, 25% and 12.5% duty cycle
* A saw-like wave (12.5% + 25% pulse waves)
* "Wavering" (CH2 slightly higher frequency than CH1)
* Pulse waves perfect 5ths apart

(see `play_note` and `update_waveform` in boueux.c)

For future versions
-------------------

experimenting with various waveforms
