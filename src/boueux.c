/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include "input.h"
#include "sound.h"
#include "music.h"
#include "boueux.h"

#define BUILD_SCALE build_scale_mode (scale, root, mode);

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
  
  BUILD_SCALE;
  
  for (;;)
   {
    keys = joypad ();
    pos = scale_position (keys);
    
    if (PRESSED (START))
     {
      octave = !octave;
      printf ("\n; octave +%d\n", octave);
      WAIT_KEY_UP (START);
     }

    if (PRESSED (SELECT))
     {
       if (PRESSED (RIGHT))
        {
         mode = (++mode) % NUM_MODES;
         BUILD_SCALE;
         WAIT_KEY_UP (RIGHT);
        }
       if (PRESSED (UP))
        {
         root = (root + 1) % 12;
         BUILD_SCALE;
         WAIT_KEY_UP (UP);
        }
       if (PRESSED (DOWN))
        {
         if (root == 0)
           root = 11;
         else
           root = (root - 1) % 12;
         BUILD_SCALE;
         WAIT_KEY_UP (DOWN);
        }
       continue;
     }

     if (pos != old_pos)
     {
      if (pos) /* note being played? */
       {
        CH1_VOL = HIGH;
        play_freq (note_frequencies[scale[pos - 1] + octave*12]);
        printf ("%s ", note_names[scale[pos - 1] % 12]);
       }
      else
       {
        CH1_VOL = OFF;
        printf (". ");
       }
      old_pos = pos;
     }
      
    delay (LOOP_WAIT_AMOUNT);
   }
}
