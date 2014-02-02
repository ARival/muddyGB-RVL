/* input.h -- dealing with input
 */

#include "boueux.h"

#ifndef INPUT_H
#define INPUT_H

#define LOOP_WAIT_AMOUNT 1

#define PRESSED(KEY) (keys & J_## KEY)

UBYTE scale_position (UBYTE keys);

#endif
