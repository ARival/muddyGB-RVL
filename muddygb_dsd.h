/* boueux.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define MUDDYGBDSD_VERSION "0.1a"

#define UINT unsigned int
#define UBYTE unsigned char
#define SCALE UBYTE

#define NUM_MODES 7
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad () & J_## KEY) {}

enum Waveforms
{ pulse_50, pulse_25, pulse_12_5, sawlike, waver, perfect_5ths, wawa, echo,
  NUM_WAVEFORMS };

UBYTE scale_position (UBYTE keys);

void play_note (short note, UBYTE waveform);
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void update_waveform (UBYTE waveform);
void wawa_update (void);

#endif
