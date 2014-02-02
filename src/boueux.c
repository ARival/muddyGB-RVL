/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include "boueux.h"
#include "sound.h"
#include "music.h"

UBYTE scale_position (UBYTE keys);

void
main ()
{
  UBYTE keys;
  UBYTE pos, old_pos = 0;
  UBYTE octave = 0;
  UBYTE mode = 0;
  UBYTE root = C;
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
        play_freq (note_frequencies[scale[pos - 1] + octave*OCTAVE_LEN]);
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
  UBYTE dpad_keys = keys & 0x0F; // get lower nibble
  
  // only one dpad key being pressed?
  UBYTE nothing_else = !(dpad_keys & (dpad_keys - 1));
  
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
