/* boueux.h -- main program loop
 */

#include <gb/gb.h>

#ifndef BOUEUX_H
#define BOUEUX_H

#define BOUEUX_VERSION "0.0"

#define USHORT unsigned short

#define NUM_MODES 6
#define OCTAVE_LEN 12

#define PRESSED(KEY) (keys & J_## KEY)
#define WAIT_KEY_UP(KEY) while (joypad() & J_## KEY) {}

#endif
