/* scales.c -- available scales
 */

#include "music.h"

/* Define each scale type available, in semitones from tonic.
 * C is defined as 0, so scales are defined relative to C.
 */

SCALE major[] = { C, D,  E,  F,  G,  A,  B, C2  };
SCALE dorian[] = { C, D, Ds,  F,  G,  A, As, C2  };
SCALE phrygian[] = { C, Cs,  Ds, F,  G,  Gs,  As, C2  };
SCALE lydian[] = { C, D,  E, Fs,  G,  A,  B, C2  };
SCALE myxolydian[] = { C, D,  E,  F,  G,  A,  As, C2  };
SCALE minor[] = { C, D, Ds,  F,  G, Gs, As, C2  };
SCALE locrian[] = { C, Cs,  Ds, F,  Fs,  Gs,  As, C2  };
SCALE blues[] = { C, Ds, F, Fs,  G, As, C2, Ds2 };

//SCALE  harmonic[] = { C, D, Ds,  F,  G, Gs,  B, C2  };
//SCALE wholetone[] = { C, D,  E, Fs, Gs, As, C2, D2  };

/* The 'blue' scale uses quarter-tones. (see music.c.)
 * It is equivalent to C, D, Ed, F, G, A, Bd, C2.
 */
