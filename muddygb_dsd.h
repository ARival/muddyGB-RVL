/* boueux.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define MUDDYGBDSD_VERSION "0.1.1a"

#define UINT unsigned int
#define UBYTE unsigned char
#define SCALE UBYTE

#define NUM_MODES 3
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad () & J_## KEY) {}

enum Waveforms
{ square, waver, perfect_5ths, pulsemod,
  NUM_WAVEFORMS };

UBYTE scale_position (UBYTE keys);

void play_note (short note, UBYTE waveform, int bend);
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void update_waveform (UBYTE waveform);

#endif
