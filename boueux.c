/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>
#include <rand.h>
#include "boueux.h"
#include "sound.h"
#include "music.h"

BOOLEAN blue_mode = OFF;
USHORT blue_map[8];

void play_p5 (UBYTE * scale, UBYTE pos, UBYTE octave);

void
main ()
{
  UBYTE keys;
  UBYTE pos, old_pos = 0;
  UBYTE relative_octave = 0;
  UBYTE absolute_octave;
  UBYTE mode = 0;
  UBYTE root = C;
  UBYTE duty = 0;
  SCALE scale[8];
  UBYTE rand_data;

  font_t big_font, small_font;
  font_init ();
  big_font = font_load (font_ibm);
  small_font = font_load (font_spect);
  font_set (big_font);
 
  initarand (0);
  
  INIT_SOUND;
  MASTER_VOLUME = HIGH;
 
  printf (";; Boueux v%s\n", BOUEUX_VERSION);
  
  build_scale_mode (scale, root, mode);
  
  for (;;)
   {
    keys = joypad ();
    pos = scale_position (keys);
    
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
         duty = (duty + 1) % 6;
         update_duty_cycle (duty);
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

     if (pos != old_pos)
     {
      if (pos) /* Play note */
       {
        CH1_VOL = HIGH;
        CH2_VOL = HIGH;
        
        if (duty == perfect_5ths)
          play_p5 (scale, pos, relative_octave);
        else
          play_note (scale, pos, relative_octave);
          
        font_set (small_font);
        printf (note_names[scale[pos - 1] % OCTAVE_LEN]);
        
        absolute_octave = relative_octave + scale[pos - 1]/OCTAVE_LEN + 3;
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
      old_pos = pos;
     }
    
    if (duty == noisy)
     {
      rand_data = arand();
      // create noisiness in channel 1
      NR13_REG = ((NR13_REG + (USHORT)(rand_data/127)) - 1);
     }
     
    delay (1);
   }
}

UBYTE
scale_position (UBYTE keys)
{
  if (PRESSED(LEFT)  && ONLY_ONE_DPAD_KEY) return 1;
  if (PRESSED(LEFT)  && PRESSED(DOWN))     return 2;
  
  if (PRESSED(DOWN)  && ONLY_ONE_DPAD_KEY) return 3;
  if (PRESSED(DOWN)  && PRESSED(RIGHT))    return 4;
  
  if (PRESSED(RIGHT) && ONLY_ONE_DPAD_KEY) return 5;
  if (PRESSED(RIGHT) && PRESSED(UP))       return 6;
  
  if (PRESSED(UP)    && ONLY_ONE_DPAD_KEY) return 7;
  if (PRESSED(UP)    && PRESSED(LEFT))     return 8;

  return 0;
}

void
play_note (UBYTE * scale, UBYTE pos, UBYTE octave)
{
  if (blue_mode)
    play_freq (blue_map[pos - 1]);
  else
    play_freq (note_frequencies[scale[pos - 1] + octave*OCTAVE_LEN]);
}

void
play_p5 (UBYTE * scale, UBYTE pos, UBYTE octave)
{
  USHORT freq1 = (note_frequencies[scale[pos - 1] + octave*OCTAVE_LEN]);
  USHORT freq2 = (note_frequencies[scale[pos - 1] + octave*OCTAVE_LEN + 5]);
  
  /* Channel 1 */
  NR13_REG = (unsigned char) freq1;
  NR14_REG = 0x80 | (freq1 >> 8);
  
  /* Channel 2 */
  NR23_REG = (unsigned char) freq2;
  NR24_REG = 0x80 | (freq2 >> 8);
}

#define BUILD(TYPE) \
  printf ("\n;; %s %s\n", note_names[tonic], #TYPE); \
  build_scale (scale, tonic, TYPE); \
  blue_mode = OFF; \
  break;

void
build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode)
{
  switch (mode)
   {
    case 0: BUILD (ionian);
    case 1: BUILD (aeolian);
    case 2: BUILD (harmonic);
    case 3:
      printf ("\n;; %s blue\n", note_names[tonic]);
      build_scale (scale, tonic, ionian);
      build_blue_freq_map (blue_map, tonic, note_frequencies);
      blue_mode = ON;
      break;
    case 4: BUILD (dorian);
    case 5: BUILD (lydian);
    case 6: BUILD (wholetone);
    case 7: BUILD (blues);
   }
}

void
update_duty_cycle (UBYTE duty)
{
  CH1 = RESET;
  CH2 = RESET;
  
  switch (duty)
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
    case noisy:
     puts ("\n;; waveform noisy");
     SET_PULSE_WIDTH (CH1, 12_5);
     SET_PULSE_WIDTH (CH2, 25);
     break;
    case perfect_5ths:
     puts ("\n;; perfect 5ths");
     SET_PULSE_WIDTH (CH1, 50);
     SET_PULSE_WIDTH (CH2, 12_5);
     break;
   }
}
