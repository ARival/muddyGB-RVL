/* input.h -- dealing with input
 */

#include "boueux.h"

#ifndef INPUT_H
#define INPUT_H

#define LOOP_WAIT_AMOUNT 1
#define NUM_MODES 4

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad() & J_## KEY) {}

UBYTE scale_position (UBYTE keys);
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);

#endif
