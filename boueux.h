/* boueux.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define BOUEUX_VERSION "0.0"

#define USHORT unsigned short
#define SCALE const UBYTE const

#define NUM_MODES 6
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad () & J_## KEY) {}

enum waveform_enum
{ pulse_50, pulse_25, pulse_12_5, sawlike, waver, perfect_5ths };

UBYTE scale_position (UBYTE keys);

void play_note (UBYTE note);
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void update_waveform (void);

#endif
