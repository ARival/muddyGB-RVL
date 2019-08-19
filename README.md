MuddyGB_RVL.gb
=========

MuddyGB_RVL.gb is a fork of MuddyGB_DSD, which is itself a clone of MuddyGB a music progam for the Nintendo GameBoy.

This fork will focus on extending MuddyGB to be a robust live instrument for chiptune performers.

Latest binaries can be found
[here](https://github.com/ARival/muddyGB-RVL/releases).

To compile from source, run `make` with GBDK installed.
There are currently no configuration options.

Usage
-----

Usage is similar to muddyGB, but with a much different method of playing.

* The directional pad acts to play the notes of the chosen scale.
* The B button enables portamento between notes
* The A button performs a fixed vibrato
* while holding START:
  * + UP or DOWN changes the octave within a range of 3
  * + RIGHT cycles sequentially through modes.
  * + LEFT selects the waveform.
  * + A enables echo.
* while holding SELECT:
  * + UP or DOWN selects the scale tonic (base note).
  * + LEFT or RIGHT changes the relative mode, basically allowing for an offset of the current scale. You can use this to play along with specific songs more conveniently.

Available modes
---------------

All of the basic modes are represented. Currently, the blues scale is disabled, though it will be re-enabled in the near future.

(see scales.c)

Available waveforms
-------------------

* Pulse waves at 50%, 25% and 12.5% duty cycle
* A saw-like wave (12.5% + 25% pulse waves)
* "Wavering" (CH2 slightly higher frequency than CH1)
* Pulse waves perfect 5ths apart

(see `play_note` and `update_waveform` in muddygb_dsd.c)

For future versions
-------------------

minestones for v1.0:

* Add variable echo times
* Fix the Piano to go up and down octaves, and shift around
* Add more custom scales (blues, harmonic minor, etc)
* Add selectable envelopes (volume, pitch and pulse width)
* Possible Scale Editor!
