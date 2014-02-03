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
