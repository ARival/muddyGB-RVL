/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <gb/font.h>
#include <gb/console.h>
#include <stdio.h>
#include <time.h>
#include "muddygb_dsd.h"
#include "sound.h"
#include "music.h"
#include "gfx/pianoroll.c"
#include "gfx/pianolayout.c"
#include "gfx/ui.h"

void main() {

    UBYTE keys;
    UBYTE pos, old_pos = 0;
    int note = 0;
    int noteInt = 0;  // Used for the piano
    int oldNoteMap[4] = {0,0,0,0};
    int octave_min = 0;
    int octave_max = 3;
    int relative_octave = 0;
    UINT absolute_octave;
    UBYTE waveform = square;
    UBYTE mode = 0;
    UBYTE root = C;
    SCALE scale[8];


    int bend = 0;
    int bendcount = 0;
    int bendwait = 4;
    int dontbend = 0;

    // For note bending
    int initialNote = 0;
    int targetMult = 0;
    int targetNote = -1;
    int isPortamento = 0;

    int vibOn = 0;
    int vibamt = 0;
    int vibIndex = 0;
    int vibCount = 0;
    int vibWait = 30;
    //int dontbend = 0;
    const int vibratoValues[] = {
       -1,-3,-7,-3,-1,1,3,7,3,1,0
    };

    const int vibratoValueLength = 11;

    int pulseinit = 0;
    int pulsecount = 0;
    int pulsewait = 24;
    int pulsephase = 0;


    font_t big_font; //, small_font;
    font_init ();
    big_font = font_load (font_ibm);
    //small_font = font_load (font_spect);
    font_set (big_font);
 
    

    gotoxy(HUDPositions[0], HUDPositions[1]);
    printf ("MuddyGB-RVL v%s", MUDDYGBRVL_VERSION);


    INIT_SOUND;
    MASTER_VOLUME = OFF;
    update_waveform (waveform);
    MASTER_VOLUME = HIGH;

    build_scale_mode (scale, root, mode);

    set_bkg_data(0x61, 12, PianoRoll);
    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayout);
    /* Stop note */
    CH1_VOL = OFF;
    CH2_VOL = OFF;
  
    for (;;) {
        keys = joypad ();
        pos = scale_position (keys);
        jp = just_pressed(keys);

        if (PRESSED( START )) {
            if (jp & J_RIGHT) {
                mode = (mode + 1) % NUM_MODES;
                build_scale_mode (scale, root, mode);
            }
            /* Change waveform */
            if (jp & J_LEFT) {
                waveform = (waveform + 1) % NUM_WAVEFORMS;
                update_waveform (waveform);
            }
            /* Increment root note */
            if (jp & J_UP) {
                root = (root + 1) % OCTAVE_LEN;
                build_scale_mode (scale, root, mode);
            }
            /* Decrement root note */
            if (jp & J_DOWN) {
                if (root == 0)
                root = OCTAVE_LEN - 1;
                else
                root = (root - 1) % OCTAVE_LEN;
                build_scale_mode (scale, root, mode);
            }
        } else if (PRESSED( SELECT )) {

            if (jp & J_UP ){
                if (relative_octave < octave_max){
                    relative_octave++;
                }
                //printf ("\n;; rel octave +%d\n", relative_octave);
            } 
            if (jp & J_DOWN ){
                if (octave_min < relative_octave){
                    relative_octave--;
                }
                //printf ("\n;; rel octave +%d\n", relative_octave);
            } 
        } else {

            if (pos) {  // Check for A and B keys here.
                note = scale[pos - 1] + relative_octave*OCTAVE_LEN;

                /* Lower by semitone */
                //if (PRESSED (B) && !PRESSED (A)) note -= 1;
                if (PRESSED (A) ) {
                    vibOn = 1;
                    vibCount++;
                    if (vibCount >= vibWait) {
                        vibCount = 0;
                        vibamt = vibratoValues[vibIndex];
                        vibIndex++;
                        if (vibIndex >= vibratoValueLength) {
                            vibIndex = 0;
                        }
                        play_note(note, waveform, vibamt, 0);
                    }

                } else if (vibOn == 1){
                    // reset indexes
                    vibOn = 0;
                    vibCount = 0;
                    vibIndex = 0;
                    vibamt = 0;
                    play_note(note, waveform, vibamt, 0);
                }

                if (PRESSED (B)){
                    isPortamento = 1;
                    targetMult = targetNote *12;
                    if (bendcount == bendwait){
                        if (bend < targetMult){
                            bend++;
                            play_note(initialNote, waveform, bend, 0);
                        } else if (bend > targetMult){
                            bend--;
                            play_note(initialNote, waveform, bend, 0);
                        } else if (bend == targetMult) {
                            //printf("reached target note: %d\n", targetNote);
                            bend = 0;
                            initialNote = note;
                            targetNote = 0;
                            play_note(note, waveform, bend, 0);

                        }
                        bendcount = 0;
                    }
                    bendcount++;

                } else {
                    isPortamento = 0;
                    bend = 0;
                }
            } else {
                bend = 0;
                bendcount = 0;
            }

            /* Change octave */

            if ((pos != old_pos)) {
                if (pos) {
                    if (isPortamento){
                        
                        targetNote = note - initialNote;

                    } else {
                        /* Note will be played */
                        bend = 0;
                        bendcount = 0;
                        pulseinit = 0;
                        targetNote = 0;

                        play_note(note, waveform, bend, 1);

                        initialNote = note;

                    }

                    //font_set(small_font);
                    gotoxy(HUDPositions[6], HUDPositions[7]);
                    //printf(note_names[note + OCTAVE_LEN]);

                    
                    if (note >= 0)
                        printf(note_names[note % OCTAVE_LEN]);
                    else
                        printf(note_names[note + OCTAVE_LEN]);

                    

                    absolute_octave = note / OCTAVE_LEN + 3;
                    printf("%d", absolute_octave);

                    gotoxy(0, 16);
                    noteInt = note % OCTAVE_LEN;


                    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayout);
                    set_bkg_tiles(7 + PianoOffset[noteInt],16,2,2, PianoNotesDown[noteInt]);

                    //printf(" ");
                    //font_set(big_font);
                } else {
                    /* Stop note */
                    CH1_VOL = OFF;
                    CH2_VOL = OFF;
                    targetNote = 0;
                    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayout);
                }
            }

            old_pos = pos;

            if (waveform == pulsemod){
                //update pulsemod
                // this is draft code for testing ... implement it more efficient
                if (!pulseinit){
                    pulseinit = 1;
                    pulsecount = 0;
                    pulsephase = 0;
                }
                pulsecount++;
                if (pulsecount == pulsewait){
                    pulsecount = 0;
                    pulsephase++;
                    if (5 < pulsephase){
                        pulsephase = 0;
                    }
                    switch (pulsephase) {
                        case 0:
                            SET_PULSE_WIDTH (CH1, 12_5);
                            break;
                        case 1:
                            SET_PULSE_WIDTH (CH1, 25);
                            break;
                        case 2:
                            SET_PULSE_WIDTH (CH1, 50);
                            break;
                        case 3:
                            SET_PULSE_WIDTH (CH1, 75);
                            break;
                        case 4:
                            SET_PULSE_WIDTH (CH1, 50);
                            break;
                        case 5:
                            SET_PULSE_WIDTH (CH1, 25);
                            break;
                    }
                }
            } else {
                pulseinit = 0;
            }
        }
        oldPad = keys;
        // wait_vbl_done();
    }
}

UBYTE scale_position (UBYTE keys) {
    // is only one d-pad key being pressed?
    BOOLEAN nothing_else = !((keys & 0x0F) & ((keys & 0x0F) - 1));

    if (PRESSED (LEFT)  && nothing_else)    return 1;
    if (PRESSED (LEFT)  && PRESSED (DOWN))  return 2;

    if (PRESSED (DOWN)  && nothing_else)    return 3;
    if (PRESSED (DOWN)  && PRESSED (RIGHT)) return 4;

    if (PRESSED (RIGHT) && nothing_else)    return 5;
    if (PRESSED (RIGHT) && PRESSED (UP))    return 6;

    if (PRESSED (UP)    && nothing_else)    return 7;
    if (PRESSED (UP)    && PRESSED (LEFT))  return 8;

    return 0;
}

void play_note (short note, UBYTE waveform, short bend, int newNote ) {
    UINT freq, freq2 = 0;
    freq = getFrequencies(note, bend);
    /* (+ 1) because B2 needs to be able to be represented, because using
    * the A button on C3 will give B2. */

    if (newNote)
        CH1_VOL = HIGH;

    switch (waveform) {
        case perfect_5ths:
            freq2 = getFrequencies(note + (G - C), bend);
            play_freq_ch1 (freq, newNote);
            if (newNote) CH2_VOL = HIGH;
            play_freq_ch2 (freq2, newNote);
            break;
        case waver:
            play_freq_ch1 (freq, newNote);
            if (newNote) CH2_VOL = HIGH;
            freq2 = getFrequencies(note, bend+2);
            play_freq_ch2 (freq2, newNote);
            break;
        default:
            play_freq_ch1 (freq, newNote);
            //CH2_VOL = OFF;
            //play_freq_ch2 (freq, bend);
            break;
    }
}

#define BUILD(TYPE) \
    gotoxy(HUDPositions[4], HUDPositions[5]); \
    printf ("mode %s", note_names[tonic]); \
    printf ("%s       ", #TYPE); \
    build_scale (scale, tonic, TYPE); \
    break;

void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode) {
    switch (mode) {
        case 0: BUILD (major);
        case 1: BUILD (dorian);
        case 2: BUILD (phrygian);
        case 3: BUILD (lydian);
        case 4: BUILD (myxolydian);
        case 5: BUILD (minor);
        case 6: BUILD (locrian);
        case 7: BUILD (blues);
    }
}

void update_waveform (UBYTE waveform) {
    CH1 = RESET;
    CH2 = RESET;

    gotoxy(HUDPositions[2], HUDPositions[3]);

    switch (waveform) {
        case square:

            printf ("waveform: square   ");
            SET_PULSE_WIDTH (CH1, 50);
            //SET_PULSE_WIDTH (CH2, 50);
            break;
        case pulse1:
            printf ("waveform: pulse25  ");
            SET_PULSE_WIDTH (CH1, 25);
            //SET_PULSE_WIDTH (CH2, 50);
            break;
        case pulse2:
            printf ("waveform: pulse12.5");
            SET_PULSE_WIDTH (CH1, 12_5);
            //SET_PULSE_WIDTH (CH2, 50);
            break;
        case waver:
            printf ("waveform: waver    ");
            SET_PULSE_WIDTH (CH1, 50);
            SET_PULSE_WIDTH (CH2, 50);
            break;
        case perfect_5ths:
            printf("waveform: 5ths      ");
            SET_PULSE_WIDTH (CH1, 50);
            SET_PULSE_WIDTH (CH2, 50);
            break;
        case pulsemod:
            printf ("waveform: pulsemod ");
            SET_PULSE_WIDTH (CH1, 12_5);
            //SET_PULSE_WIDTH (CH2, 12_5);
            break;
    }
}

UINT8 just_pressed (UINT8 newPad) {
    UINT8 tempPress = oldPad ^ newPad;

    return tempPress & newPad;

}
/* 
int compute_new_bend (int bend, int targetNote) {
    int counter = 0;

    if (bend < 0)
    {
        while (bend < -12)
        {
            counter++;
            bend += 12; 
        }
    }
    return 0;
}
*/
