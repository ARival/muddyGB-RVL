#include <stdio.h>
#include "input.h"
#include "music.h"
#include "sound.h"

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
