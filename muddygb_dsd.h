/* boueux.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define MUDDYGBRVL_VERSION "0.1a"

#define UINT unsigned int
#define UBYTE unsigned char
#define SCALE UBYTE

#define NUM_MODES 3
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad () & J_## KEY) {}


enum Waveforms
{ square, waver, perfect_5ths, pulsemod, echo,
  NUM_WAVEFORMS };

UBYTE scale_position (UBYTE keys);
UINT8 oldPad = 0;
UINT8 jp = 0;

void play_note (short note, UBYTE waveform, short bend, int newNote );
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void update_waveform (UBYTE waveform);
UINT8 just_pressed (UINT8 newPad);

#endif
