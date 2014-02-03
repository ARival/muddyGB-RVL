/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>
#include <time.h>
#include "boueux.h"
#include "sound.h"
#include "music.h"

UBYTE waveform = pulse_50;

void
main ()
{
  UBYTE keys;
  UBYTE pos, old_pos = 0;
  UBYTE note, old_note = 0;
  UBYTE relative_octave = 0;
  UBYTE absolute_octave;
  UBYTE mode = 0;
  UBYTE root = C;
  SCALE scale[8];

  font_t big_font, small_font;
  font_init ();
  big_font = font_load (font_ibm);
  small_font = font_load (font_spect);
  font_set (big_font);
 
  INIT_SOUND;
  MASTER_VOLUME = OFF;
  update_waveform ();
  MASTER_VOLUME = HIGH;
  
  printf (";; Boueux v%s\n", BOUEUX_VERSION);
  
  build_scale_mode (scale, root, mode);
  
  for (;;)
   {
    keys = joypad ();
    pos = scale_position (keys);
    
    if (pos)
     {
      note = scale[pos - 1] + relative_octave*OCTAVE_LEN;
    
      /* Raise by perfect 4th */
      if (PRESSED (B)) note += 5; /* a perfect fourth = 5 semitones */
      /* Lower by semitone */
      if (PRESSED (A)) note -= 1;
     }
    
    /* Change octave */
    if (PRESSED (START))
     {
      relative_octave = !relative_octave;
      printf ("\n;; rel octave +%d\n", relative_octave);
      WAIT_KEY_UP (START);
     }

    if (PRESSED (SELECT))
     {
       /* Change mode */
       if (PRESSED (RIGHT))
        {
         mode = (mode + 1) % NUM_MODES;
         WAIT_KEY_UP (RIGHT);
         build_scale_mode (scale, root, mode);
        }
       /* Change waveform */
       if (PRESSED (LEFT))
        {
         WAIT_KEY_UP (LEFT);
         waveform = (waveform + 1) % NUM_WAVEFORMS;
         update_waveform ();
       }
       /* Increment root note */
       if (PRESSED (UP))
        {
         WAIT_KEY_UP (UP);
         root = (root + 1) % OCTAVE_LEN;
         build_scale_mode (scale, root, mode);
        }
       /* Decrement root note */
       if (PRESSED (DOWN))
        {
         WAIT_KEY_UP (DOWN);
         if (root == 0)
           root = OCTAVE_LEN - 1;
         else
           root = (root - 1) % OCTAVE_LEN;
         build_scale_mode (scale, root, mode);
        }
        
       continue;
     }

     if ((note != old_note) || (pos != old_pos))
     {
      if (pos) /* Note will be played */
       {
        CH1_VOL = HIGH;
        CH2_VOL = HIGH;
        
        play_note (note);
        
        font_set (small_font);
        printf (note_names[note % OCTAVE_LEN]);
        
        absolute_octave = note/OCTAVE_LEN + 3;
        printf ("%d", absolute_octave);
        
        printf (" ");
        font_set (big_font);
       }
      else /* Stop note */
       {
        CH1_VOL = OFF;
        CH2_VOL = OFF;
        printf (". ");
       }
     }
    
    if (waveform == wawa) wawa_update();
    
    old_note = note;
    old_pos = pos;
   }
}

UBYTE
scale_position (UBYTE keys)
{
  // is only one d-pad key being pressed?
  BOOLEAN nothing_else = !((keys & 0x0F) & ((keys & 0x0F) - 1));

  if (PRESSED (LEFT)  && nothing_else)    return 1;
  if (PRESSED (LEFT)  && PRESSED (DOWN))  return 2;
  
  if (PRESSED (DOWN)  && nothing_else)    return 3;
  if (PRESSED (DOWN)  && PRESSED (RIGHT)) return 4;
  
  if (PRESSED (RIGHT) && nothing_else)    return 5;
  if (PRESSED (RIGHT) && PRESSED (UP))    return 6;
  
  if (PRESSED (UP)    && nothing_else)    return 7;
  if (PRESSED (UP)    && PRESSED (LEFT))  return 8;

  return 0;
}

void
play_note (UBYTE note)
{
  USHORT freq, freq2 = 0;
  freq = note_frequencies[note];
  
  switch (waveform)
   {
    case perfect_5ths:
      freq2 = note_frequencies[note + (G - C)];
      play_freq_ch1 (freq);
      play_freq_ch2 (freq2);
      break;
    case waver:
      play_freq_ch1 (freq);
      play_freq_ch2 (freq + 1);
      break;
    case echo:
      play_freq_ch1 (freq);
      delay (500);
      play_freq_ch2 (freq);
      break;
    default:
      play_freq_ch1 (freq);
      play_freq_ch2 (freq);
      break;
   }
}

#define BUILD(TYPE) \
  printf ("\n;; mode %s %s\n", note_names[tonic], #TYPE); \
  build_scale (scale, tonic, TYPE); \
  break;

void
build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode)
{
  switch (mode)
   {
    case 0: BUILD (ionian);
    case 1: BUILD (aeolian);
    case 2: BUILD (harmonic);
    case 3: BUILD (dorian);
    case 4: BUILD (lydian);
    case 5: BUILD (wholetone);
    case 6: BUILD (blues);
   }
}

void
update_waveform (void)
{
  CH1_VOL = HIGH;
  CH2_VOL = HIGH;
  
  switch (waveform)
   {
    case pulse_50:
     puts ("\n;; waveform 50%");
     SET_PULSE_WIDTH (CH1, 50);
     SET_PULSE_WIDTH (CH2, 50);
     break;
    case pulse_25:
     puts ("\n;; waveform 25%");
     SET_PULSE_WIDTH (CH1, 25);
     SET_PULSE_WIDTH (CH2, 25);
     break;
    case pulse_12_5:
     puts ("\n;; waveform 12.5%");
     SET_PULSE_WIDTH (CH1, 12_5);
     SET_PULSE_WIDTH (CH2, 12_5);
     break;
    case sawlike:
     puts ("\n;; waveform sawlike");
     SET_PULSE_WIDTH (CH1, 50);
     SET_PULSE_WIDTH (CH2, 12_5);
     break;
    case waver:
     puts ("\n;; waveform waver");
     SET_PULSE_WIDTH (CH1, 12_5);
     SET_PULSE_WIDTH (CH2, 25);
     break;
    case perfect_5ths:
     puts ("\n;; waveform 5ths");
     SET_PULSE_WIDTH (CH1, 50);
     SET_PULSE_WIDTH (CH2, 50);
     break;
    case wawa:
     puts ("\n;; waveform wawa");
     SET_PULSE_WIDTH (CH1, 50);
     SET_PULSE_WIDTH (CH2, 12_5);
     break;
    case echo:
     puts ("\n;; waveform echo");
     SET_PULSE_WIDTH (CH1, 50);
     SET_PULSE_WIDTH (CH2, 50);
     
     CH2_VOL = 0x88;
     break;
   }
}

void
wawa_update (void)
{
  switch ((clock () % 60) / 20)
   {
    case 0:
      SET_PULSE_WIDTH (CH2, 12_5);
      break;
    case 1:
      SET_PULSE_WIDTH (CH2, 25);
      break;
    case 2:
      SET_PULSE_WIDTH (CH2, 50);
      break;
   }
}
