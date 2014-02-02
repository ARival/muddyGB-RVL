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
  UBYTE keys, old_pos = 0, pos, octave = 0, mode = 0;
  SCALE scale[8];

  printf ("Boueux v%s\n", BOUEUX_VERSION);

  INIT_SOUND;
  MASTER_VOLUME = HIGH;
  
  build_scale (scale, C, ionian);
  
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
       if (PRESSED (LEFT))
        {
         mode = (++mode) % 3;
         printf ("\nmode: %d\n", mode);
         WAIT_KEY_UP (LEFT);
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
