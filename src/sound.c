/* sound.c -- playing sound
 */

#include "sound.h"

/* These frequencies are calculated like 2^11 - (2^17 / FrequencyInHertz).
 * So 2048 represents 2^17 Hz and 0 represents 64 Hz.
 * This list starts at C3 and ends at C5.
 */
const USHORT note_frequencies[] = {
  1046, 1102, 1155, 1205, 1253, 1297, 1340, 1379, 1417, 1452, 1486,
  1517, 1547, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750,
  1767, 1783, 1798
};

// Play frequency to channel 1
// Frequencies are in non-hertz values.
void
play_freq (USHORT freq)
{
  NR13_REG = (unsigned char) freq;
  NR14_REG = 0x80 | (freq >> 8);
}
