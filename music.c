/* music.c -- music theory
 */

#include "music.h"
#include "scales.c"

void
build_scale (UBYTE * scale, UBYTE tonic, SCALE scale_type[])
{
  UBYTE i;
  for (i = 0; i != 8; i++)
    scale[i] = scale_type[i] + tonic;
}

/* Create array of frequencies with blue (non-chromatic) notes */
void
build_blue_freq_map (USHORT * freq_map, UBYTE tonic, USHORT freq_list[])
{
  UBYTE i, note;
  USHORT blued_freq;

  for (i = 0; i != 8; i++)
   {
    note = ionian[i] + tonic;

    if (i == 2 || i == 6)
     {
      blued_freq = (freq_list[note] + freq_list[note - 1]) / 2;
      freq_map[i] = blued_freq;
     }
    else
      freq_map[i] = freq_list[note];
   }
}
