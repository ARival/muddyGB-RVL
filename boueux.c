/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>
#include "boueux.h"
#include "sound.h"
#include "music.h"

UBYTE blue_mode = OFF;
USHORT blue_map[8];

void
main ()
{
  UBYTE keys;
  UBYTE pos, old_pos = 0;
  UBYTE octave = 0;
  UBYTE mode = 0;
  UBYTE root = C;
  UBYTE duty = 0;
  SCALE scale[8];

  INIT_SOUND;
  MASTER_VOLUME = HIGH;
 
  printf ("*** Boueux v%s\n", BOUEUX_VERSION);
  
  build_scale_mode (scale, root, mode);
  
  for (;;)
   {
    keys = joypad ();
    pos = scale_position (keys);
    
    /* Change octave */
    if (PRESSED (START))
     {
      octave = !octave;
      printf ("\n*** octave +%d\n", octave);
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
         duty = (duty + 1) % 3;
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
        play_note (scale, pos, octave);
        printf ("%s ", note_names[scale[pos - 1] % OCTAVE_LEN]);
       }
      else /* Stop note */
       {
        CH1_VOL = OFF;
        printf (". ");
       }
      old_pos = pos;
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

#define BUILD(TYPE) \
  printf ("\n*** %s %s\n", note_names[tonic], #TYPE); \
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
      printf ("\n*** %s blue\n", note_names[tonic]);
      build_scale (scale, tonic, ionian);
      build_blue_freq_map (blue_map, tonic, note_frequencies);
      blue_mode = ON;
      break;
    case 4: BUILD (dorian);
    case 5: BUILD (lydian);
   }
}

void
update_duty_cycle (UBYTE duty)
{
  switch (duty)
   {
    case 0:
     puts ("\n*** pulse width 12.5%\n");
     SET_PULSE_WIDTH (CH1, 12_5);
     break;
    case 1:
     puts ("\n*** pulse width 25%\n");
     SET_PULSE_WIDTH (CH1, 25);
     break;
    case 2:
     puts ("\n*** pulse width 50%\n");
     SET_PULSE_WIDTH (CH1, 50);
     break;
   }
}
