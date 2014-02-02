/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include "input.h"
#include "sound.h"
#include "music.h"
#include "boueux.h"

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
  
  build_scale (scale, root, ionian);
  
  for (;;)
   {
    keys = joypad();
    pos = scale_position(keys);
    
    if (PRESSED (START))
     {
      octave = !octave;
      printf ("\noctave: %d\n", octave);
      WAIT_KEY_UP (START);
     }

    if (PRESSED (SELECT))
     {
       if (PRESSED (RIGHT))
        {
         mode = (++mode) % NUM_MODES;
         build_scale_mode (scale, root, mode);
         WAIT_KEY_UP (RIGHT);
        }
       continue;
     }

     if (pos != old_pos)
     {
      if (pos)
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
