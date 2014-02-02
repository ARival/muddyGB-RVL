/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
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

  printf ("Boueux v%s\n", BOUEUX_VERSION);

  INIT_SOUND;
  MASTER_VOLUME = HIGH;
  
  build_scale_mode (scale, root, mode);
  
  for (;;)
   {
    keys = joypad ();
    pos = scale_position (keys);
    
    if (PRESSED (START))
     {
      octave = !octave;
      printf ("\n;; octave +%d\n", octave);
      WAIT_KEY_UP (START);
     }

    if (PRESSED (SELECT))
     {
       if (PRESSED (RIGHT))
        {
         mode = (mode + 1) % NUM_MODES;
         WAIT_KEY_UP (RIGHT);
         build_scale_mode (scale, root, mode);
        }
       if (PRESSED (LEFT))
        {
         duty = (duty + 1) % 3;
         WAIT_KEY_UP (LEFT);
         update_duty_cycle (duty);
       }
       if (PRESSED (UP))
        {
         root = (root + 1) % OCTAVE_LEN;
         WAIT_KEY_UP (UP);
         build_scale_mode (scale, root, mode);
        }
       if (PRESSED (DOWN))
        {
         if (root == 0)
           root = OCTAVE_LEN - 1;
         else
           root = (root - 1) % OCTAVE_LEN;
         WAIT_KEY_UP (DOWN);
         build_scale_mode (scale, root, mode);
        }
        
       continue;
     }

     if (pos != old_pos)
     {
      if (pos) /* note being played? */
       {
        CH1_VOL = HIGH;
        play_note (scale, pos, octave);
        printf ("%s ", note_names[scale[pos - 1] % OCTAVE_LEN]);
       }
      else
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
  // only one dpad key being pressed?
  UBYTE nothing_else = !(DPAD_KEYS & (DPAD_KEYS - 1));
  
  if (PRESSED(LEFT)  && nothing_else)   return 1;
  if (PRESSED(LEFT)  && PRESSED(DOWN))  return 2;
  
  if (PRESSED(DOWN)  && nothing_else)   return 3;
  if (PRESSED(DOWN)  && PRESSED(RIGHT)) return 4;
  
  if (PRESSED(RIGHT) && nothing_else)   return 5;
  if (PRESSED(RIGHT) && PRESSED(UP))    return 6;
  
  if (PRESSED(UP)    && nothing_else)   return 7;
  if (PRESSED(UP)    && PRESSED(LEFT))  return 8;

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
  printf ("\n; %s %s\n", note_names[tonic], #TYPE); \
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
      printf ("\n; %s blue\n", note_names[tonic]);
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
     puts ("\n; pulse width 12.5%\n");
     SET_PULSE_WIDTH (CH1, 12_5);
     break;
    case 1:
     puts ("\n; pulse width 25%\n");
     SET_PULSE_WIDTH (CH1, 25);
     break;
    case 2:
     puts ("\n; pulse width 50%\n");
     SET_PULSE_WIDTH (CH1, 50);
     break;
   }
}
