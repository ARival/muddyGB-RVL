/* boueux.c -- main program loop
 */

#include <gb/gb.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <stdio.h>
#include <time.h>
#include "muddygb_rvl.h"
#include "sound.h"
#include "music.h"
#include "gfx/pianoroll.c"
#include "gfx/pianolayoutcolor.c"
#include "gfx/ui.h"

void main() {
    extern UBYTE _cpu;

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
    int bendstrength = 12;

    /* Portamento stuff */
    int initialNote = 0;
    int targetMult = 0;
    int targetNote = -1;
    int portaOn = 0;

    /* Vibrato stuff */
    int vibOn = 0;
    int vibamt = 0;
    int vibIndex = 0;
    int vibCount = 0;
    const int vibratoValues[] = {
       -1,-3,-7,-3,-1,1,3,7,3,1,0
    };

    const int vibratoValueLength = 11;

    int pulseinit = 0;
    int pulsecount = 0;
    int pulsewait = 24;
    int pulsephase = 0;


    


    UWORD CGBPalettes [8*4] = {
        PianoRollCGBPal0c0,PianoRollCGBPal0c1,PianoRollCGBPal0c2,PianoRollCGBPal0c3,
        PianoRollCGBPal1c0,PianoRollCGBPal1c1,PianoRollCGBPal1c2,PianoRollCGBPal1c3,
        PianoRollCGBPal2c0,PianoRollCGBPal2c1,PianoRollCGBPal2c2,PianoRollCGBPal2c3,
        PianoRollCGBPal3c0,PianoRollCGBPal3c1,PianoRollCGBPal3c2,PianoRollCGBPal3c3,
        PianoRollCGBPal4c0,PianoRollCGBPal4c1,PianoRollCGBPal4c2,PianoRollCGBPal4c3,
        PianoRollCGBPal5c0,PianoRollCGBPal5c1,PianoRollCGBPal5c2,PianoRollCGBPal5c3,
        PianoRollCGBPal6c0,PianoRollCGBPal6c1,PianoRollCGBPal6c2,PianoRollCGBPal6c3,
        PianoRollCGBPal7c0,PianoRollCGBPal7c1,PianoRollCGBPal7c2,PianoRollCGBPal7c3
    };

    font_t big_font, small_font;
    font_init ();
    /* if Gameboy Color, change colors of font */
    if (_cpu == 0x11)
        color(DKGREY,WHITE, SOLID );
    big_font = font_load (font_ibm);
    /* if Gameboy Color, change colors of font */
    if (_cpu == 0x11)
        color(LTGREY,WHITE, SOLID );
    small_font = font_load (font_spect);
    font_set (big_font);
    



 
    set_bkg_palette(0,1,&CGBPalettes[0]);
    set_bkg_palette(1,1,&CGBPalettes[4]);
    set_bkg_palette(2,1,&CGBPalettes[8]);
    set_bkg_palette(3,1,&CGBPalettes[12]);
    set_bkg_palette(4,1,&CGBPalettes[16]);
    set_bkg_palette(5,1,&CGBPalettes[20]);
    set_bkg_palette(6,1,&CGBPalettes[24]);
    set_bkg_palette(7,1,&CGBPalettes[28]);
    

    gotoxy(HUDPositions[0], HUDPositions[1]);
    printf ("MuddyGB-RVL v%s", MUDDYGBRVL_VERSION);

    font_set(small_font);

    INIT_SOUND;
    MASTER_VOLUME = OFF;
    update_waveform (waveform);
    MASTER_VOLUME = HIGH;

    build_scale_mode (scale, root, mode);

    set_bkg_data(0xC1, 12, PianoRoll);
    //set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayout);




  /* Select VRAM bank 1 (which stores the attributes) */
  VBK_REG = 1;

  /* Set attributes */
    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayoutBLK0PLN1);


  /* Select VRAM bank 0 (which stores the tiles) */
  VBK_REG = 0;

  /* Set data */
    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayoutBLK0PLN0);

    SHOW_BKG;


    /* Stop note */
    CH1_VOL = OFF;
    CH2_VOL = OFF;

    /* 
    disable_interrupts();
    add_VBL(process_audio);
    enable_interrupts();
    set_interrupts(VBL_IFLAG);
    */
  
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
            /* Increment octave */
            if (jp & J_UP ){
                if (relative_octave < octave_max){
                    relative_octave++;
                }
                //printf ("\n;; rel octave +%d\n", relative_octave);
            } 
            /* Decrement octave */
            if (jp & J_DOWN ){
                if (octave_min < relative_octave){
                    relative_octave--;
                }
                //printf ("\n;; rel octave +%d\n", relative_octave);
            } 
        } else if (PRESSED( SELECT )) {

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
            /* move parallel mode left */
            if (jp & J_LEFT ){
                mode = (mode-1 + NUM_MODES) % NUM_MODES;

                build_scale_mode (scale, (scale[6]+12) % 12, mode);
                root = scale[0];
            } 
            /* move parallel mode right */
            if (jp & J_RIGHT ){
                mode = (mode + 1) % NUM_MODES;
                build_scale_mode (scale, scale[1] % 12 , mode);
                root = scale[0];
            } 
        } else {
            portaOn = PRESSED (B);
            // vibOn = PRESSED (A);


            if (pos) {  // Check for A and B keys here.
                note = scale[pos - 1] + relative_octave*OCTAVE_LEN;

                /* Lower by semitone */
                //if (PRESSED (B) && !PRESSED (A)) note -= 1;
                if (PRESSED (A)) {
                    vibOn = 1;
                    //if (process_audio) {
                        vibamt = vibratoValues[vibIndex];
                        vibIndex++;
                        if (vibIndex >= vibratoValueLength) {
                            vibIndex = 0;
                        }
                        play_note(note, waveform, vibamt, 0);
                    //}

                } else if (vibOn) {
                    // reset indexes
                    vibOn = 0;
                    vibIndex = 0;
                    vibamt = 0;
                    play_note(note, waveform, vibamt, 0);
                }

            } else {
                bend = 0;
                bendcount = 0;
            }

            if ( portaOn ){
                if (old_pos == 0 && pos) {
                    portaOn = 0;
                } else if (targetNote != 0){

                    targetMult = targetNote *12;
                    if (bend < targetMult){
                        bend+=bendstrength;
                        play_note(initialNote, waveform, bend, 0);
                    } else if (bend > targetMult){
                        bend-=bendstrength;
                        play_note(initialNote, waveform, bend, 0);
                    } else if (bend == targetMult) {
                        //printf("reached target note: %d\n", targetNote);
                        bend = 0;
                        initialNote = note;
                        targetNote = 0;
                        play_note(note, waveform, bend, 0);
                    }
                }

            } else {
                bend = 0;
            }
            /* Change octave */

            if (pos != old_pos ) {
                if (pos) {
                    if (portaOn){
                        
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

                    
                    noteInt = note;

                    if (note >= 0) {
                        while (noteInt >= OCTAVE_LEN)
                        {
                            noteInt -= OCTAVE_LEN;
                        }
                        printf(note_names[noteInt]);
                    }
                    else
                        printf(note_names[note + OCTAVE_LEN]);

                    

                    absolute_octave = note / OCTAVE_LEN + 3;
                    printf("%d", absolute_octave);

                    // noteInt = note % OCTAVE_LEN;
                    


                    gotoxy(0, 16);
                    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayoutBLK0PLN0);
                    set_bkg_tiles(7 + PianoOffset[noteInt],16,2,2, PianoNotesDown[noteInt]);

                    //printf(" ");
                    //font_set(big_font);
                    old_pos = pos;
                } else if (!portaOn) {
                    /* Stop note */
                    CH1_VOL = OFF;
                    CH2_VOL = OFF;
                    targetNote = 0;
                    set_bkg_tiles(0, 16, PianoLayoutWidth, PianoLayoutHeight, PianoLayoutBLK0PLN0);
                    old_pos = pos;
                }
            }

            
            //old_pos = pos;
            //gotoxy(HUDPositions[6], HUDPositions[7] +2);
            //printf("%d %d P:%d", old_pos, pos, portaOn);

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
        process_echo();
        wait_vbl_done();
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

void play_note (short note, UBYTE waveform, short bend, UINT8 newNote ) {
    UINT freq, freq2 = 0;
    freq = getFrequencies(note, bend+process_audio);
    /* (+ 1) because B2 needs to be able to be represented, because using
    * the A button on C3 will give B2. */
    freqCH1 = (unsigned char) freq;
    //bendCH1 = newNote == 1 ? 0x80 | freq >> 8 : 0x00 | freq >> 8 ;
    bendCH1 =  0x80 | freq >> 8;

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

void process_echo () {
    echoShit[echoCounter] = NR11_REG;
    echoShit[echoCounter+1] = NR12_REG & 0x8F; 
    echoShit[echoCounter+2] = freqCH1;
    echoShit[echoCounter+3] = bendCH1;
    echoCounter +=4;
    if (echoCounter == 96) echoCounter = 0;
    NR21_REG = echoShit[echoCounter] ;
    NR22_REG = echoShit[echoCounter+1] ;
    NR23_REG = echoShit[echoCounter+2] ;
    NR24_REG = echoShit[echoCounter+3] ;
    //gotoxy(HUDPositions[6], HUDPositions[7]+2);
    //printf("regs = %x", echoShit[echoCounter + 2]);
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
