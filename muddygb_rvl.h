/* muddygb_RVL.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define MUDDYGBRVL_VERSION "0.7a"

#define UINT unsigned int
#define UBYTE unsigned char
#define SCALE UBYTE

#define NUM_MODES 7
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad () & J_## KEY) {}


enum Waveforms
{ square, pulse1, pulse2, waver, perfect_5ths, pulsemod, 
  NUM_WAVEFORMS };

UBYTE scale_position (UBYTE keys);
UINT8 oldPad = 0;
UINT8 jp = 0;
UINT8 echo_on = 0;

void vblank_isr ();
void play_note (short note, UBYTE waveform, short bend, UINT8 newNote );
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void update_waveform (UBYTE waveform);
UINT8 just_pressed (UINT8 newPad);

/* echo shit */
unsigned char echoShit[3*32] = { 0 } ;
char echoCounter = 0;
void process_echo();
UINT8 freqCH1=1;
UINT8 freqCH2=1;
UINT8 bendCH1=1;
UINT8 bendCH2=1;

#endif
