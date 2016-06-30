/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>
#include <time.h>
#include "muddygb_dsd.h"
#include "sound.h"
#include "music.h"

void main() {

    UBYTE keys;
    UBYTE pos, old_pos = 0;
    int note, old_note = 0;
    int octave_min = 0;
    int octave_max = 3;
    int relative_octave = 0;
    UINT absolute_octave;
    UBYTE waveform = pulse_50;
    UBYTE mode = 0;
    UBYTE root = C;
    SCALE scale[8];
    int start_pressed = 0;
    int select_pressed = 0;

    int bend = 0;
    int bendcount = 0;
    int bendwait = 12;
    int dontbend = 0;

    font_t big_font, small_font;
    font_init ();
    big_font = font_load (font_ibm);
    small_font = font_load (font_spect);
    font_set (big_font);
 
    printf (";; MuddyGB-DSD v%s\n", MUDDYGBDSD_VERSION);

    INIT_SOUND;
    MASTER_VOLUME = OFF;
    update_waveform (waveform);
    MASTER_VOLUME = HIGH;

    build_scale_mode (scale, root, mode);

    /* Stop note */
    CH1_VOL = OFF;
    CH2_VOL = OFF;
  
    for (;;) {
        keys = joypad ();
        pos = scale_position (keys);

        if (pos) {
            note = scale[pos - 1] + relative_octave*OCTAVE_LEN;

            /* Lower by semitone */
            if (PRESSED (B) && !PRESSED (A)) note -= 1;

            if (PRESSED (A)){
                // bend up
                if (!dontbend){
                    bendcount++;
                    if (bendcount == bendwait){
                        bendcount = 0;
                        if (PRESSED (B) && 12 < bend) {
                            bend--;
                            play_note(note, waveform, bend);
                        } else if (bend < 24){
                            bend++;
                            play_note(note, waveform, bend);
                        }
                    }
                }
            } else {
                dontbend = 0;
                if (bend != 0) {
                    bendcount++;
                    if (bendcount == bendwait) {
                        bendcount = 0;
                        bend--;
                        play_note(note, waveform, bend);
                    }
                }
            }
        } else {
            bend = 0;
            bendcount = 0;
        }

        /* Change octave */

        if (PRESSED (SELECT) && PRESSED(START)) {
            /* Change mode */
            if (PRESSED (RIGHT)) {
                mode = (mode + 1) % NUM_MODES;
                WAIT_KEY_UP (RIGHT);
                build_scale_mode (scale, root, mode);
            }
            /* Change waveform */
            if (PRESSED (LEFT)) {
                WAIT_KEY_UP (LEFT);
                waveform = (waveform + 1) % NUM_WAVEFORMS;
                update_waveform (waveform);
            }
            /* Increment root note */
            if (PRESSED (UP)) {
                WAIT_KEY_UP (UP);
                root = (root + 1) % OCTAVE_LEN;
                build_scale_mode (scale, root, mode);
            }
            /* Decrement root note */
            if (PRESSED (DOWN)) {
                WAIT_KEY_UP (DOWN);
                if (root == 0)
                root = OCTAVE_LEN - 1;
                else
                root = (root - 1) % OCTAVE_LEN;
                build_scale_mode (scale, root, mode);
            }

            continue;

        } else if (PRESSED (START) && !start_pressed){
            if (relative_octave < octave_max){
                relative_octave++;
            }
            start_pressed = 1;
            //printf ("\n;; rel octave +%d\n", relative_octave);
        } else if (PRESSED (SELECT) && !select_pressed){
            if (octave_min < relative_octave){
                relative_octave--;
            }
            select_pressed = 1;
            //printf ("\n;; rel octave +%d\n", relative_octave);
        } else {
            if (!PRESSED(START))
                start_pressed = 0;
            if (!PRESSED(SELECT))
                select_pressed = 0;
        }

        if ((note != old_note) || (pos != old_pos)) {
            if (pos) {
                /* Note will be played */
                bend = 0;
                bendcount = 0;
                if PRESSED (A) {
                    dontbend = 1;
                }

                play_note(note, waveform, bend);

                font_set(small_font);

                if (note >= 0)
                    printf(note_names[note % OCTAVE_LEN]);
                else
                    printf(note_names[note + OCTAVE_LEN]);

                absolute_octave = note / OCTAVE_LEN + 3;
                printf("%d", absolute_octave);

                printf(" ");
                font_set(big_font);
            } else {
                /* Stop note */
                CH1_VOL = OFF;
                CH2_VOL = OFF;
            }
        }

        old_note = note;
        old_pos = pos;
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

void play_note (short note, UBYTE waveform, int bend) {
    UINT freq, freq2 = 0;
    freq = getFrequencies(note, bend);
    /* (+ 1) because B2 needs to be able to be represented, because using
    * the A button on C3 will give B2. */

    CH1_VOL = HIGH;

    switch (waveform) {
        case perfect_5ths:
            freq2 = getFrequencies(note + (G - C), bend);
            play_freq_ch1 (freq, bend);
            CH2_VOL = HIGH;
            play_freq_ch2 (freq2, bend);
            break;
        case waver:
            play_freq_ch1 (freq, bend);
            CH2_VOL = HIGH;
            play_freq_ch2 (freq + 1, bend);
            break;
        default:
            play_freq_ch1 (freq, bend);
            CH2_VOL = OFF;
            //play_freq_ch2 (freq, bend);
            break;
    }
}

#define BUILD(TYPE) \
    printf ("\n;; mode %s ", note_names[tonic]); \
    printf ("%s\n", #TYPE); \
    build_scale (scale, tonic, TYPE); \
    break;

void build_scale_mode (UBYTE * scale, UBYTE tonic, UBYTE mode) {
    switch (mode) {
        case 0: BUILD (major);
        case 1: BUILD (minor);
        case 2: BUILD (blues);
    }
}

void update_waveform (UBYTE waveform) {
    CH1 = RESET;
    CH2 = RESET;

    switch (waveform) {
        case pulse_50:
            puts ("\n;; waveform 50%");
            SET_PULSE_WIDTH (CH1, 50);
            SET_PULSE_WIDTH (CH2, 50);
            break;
        case pulse_25:
            puts ("\n;; waveform 25%");
            SET_PULSE_WIDTH (CH1, 25);
            SET_PULSE_WIDTH (CH2, 25);
            break;
        case pulse_12_5:
            puts ("\n;; waveform 12.5%");
            SET_PULSE_WIDTH (CH1, 12_5);
            SET_PULSE_WIDTH (CH2, 12_5);
            break;
        case sawlike:
            puts ("\n;; waveform sawlike");
            SET_PULSE_WIDTH (CH1, 50);
            SET_PULSE_WIDTH (CH2, 12_5);
            break;
        case waver:
            puts ("\n;; waveform waver");
            SET_PULSE_WIDTH (CH1, 12_5);
            SET_PULSE_WIDTH (CH2, 25);
            break;
        case perfect_5ths:
            puts ("\n;; waveform 5ths");
            SET_PULSE_WIDTH (CH1, 50);
            SET_PULSE_WIDTH (CH2, 50);
            break;
    }
}