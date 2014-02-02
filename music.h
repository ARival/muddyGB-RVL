/* music.h -- music theory
 */

#include "boueux.h"
#include "sound.h"

#ifndef MUSIC_H
#define MUSIC_H

enum note
{ C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B, C2 };

const char *note_names[] = {
  "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

extern SCALE ionian[];
extern SCALE aeolian[];
extern SCALE harmonic[];
extern SCALE blues[];
extern SCALE dorian[];
extern SCALE lydian[];

void build_scale (UBYTE * scale, UBYTE tonic, SCALE scale_type[]);
void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode);
void build_blue_freq_map (USHORT * freq_map, UBYTE tonic, USHORT freq_list[]);

#endif
