/* sound.c -- playing sound
 */

#include "sound.h"

/* These frequencies are calculated like 2^11 - (2^17 / FrequencyInHertz).
 * So 2048 represents 2^17 Hz (highest freq.) and 0 represents 64 Hz (lowest).
 * This list starts at C3 and ends at F7.
 */
const USHORT note_frequencies[] = {
  1046, 1102, 1155, 1205, 1253, 1297, 1340, 1379, 1417, 1452, 1486, 1517, 1547,
  1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783, 1798, 1812,
  1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915, 1923, 1930, 1936, 1943, 1949,
  1954, 1959, 1964, 1969, 1974, 1978, 1982, 1985, 1988, 1992, 1995, 1998, 2001
};

/* Play frequency to channel 1 and 2.
 * Frequencies are in non-hertz values.
 */

void
play_freq_ch1 (USHORT freq)
{
  NR13_REG = (unsigned char) freq;
  NR14_REG = 0x80 | (freq >> 8);
}

void
play_freq_ch2 (USHORT freq)
{
  NR23_REG = (unsigned char) freq;
  NR24_REG = 0x80 | (freq >> 8);
}
