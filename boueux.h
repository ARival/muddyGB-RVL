/* boueux.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define BOUEUX_VERSION "0.0"

#define USHORT unsigned short
#define SCALE const UBYTE const

#define NUM_MODES 8
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad () & J_## KEY) {}
#define DPAD_KEYS (keys & 0x0F)
#define ONLY_ONE_DPAD_KEY !(DPAD_KEYS & (DPAD_KEYS - 1))

enum waveform
{ pulse_50, pulse_25, pulse_12_5, sawlike, noisy, perfect_5ths };

UBYTE scale_position (UBYTE keys);

void play_note (UBYTE * scale, UBYTE pos, UBYTE octave);
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void update_duty_cycle (UBYTE duty);

#endif
