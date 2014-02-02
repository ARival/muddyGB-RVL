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
  UBYTE keys, pos = 0, new_pos;
  SCALE scale[8];

  printf ("Boueux v%s\n", BOUEUX_VERSION);

  INIT_SOUND;
  MASTER_VOLUME = HIGH;
  
  build_scale (scale, C, ionian);
  
  for (;;)
   {
    keys = joypad();
    new_pos = scale_position(keys);
    
    if (pos != new_pos)
     {
      pos = new_pos;
      
      if (pos)
       {
        CH1_VOL = HIGH;
        play_freq (note_frequencies[scale[pos - 1]]);
        printf ("%s ", note_names[scale[pos - 1] % 12]);
       }
      else
       {
        CH1_VOL = OFF;
        printf (". ");
       }
     }
      
    delay (LOOP_WAIT_AMOUNT);
   }
}
