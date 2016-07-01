/* sound.h -- playing sound
 */

#include "muddygb_dsd.h"
#include <gb/gb.h>

#ifndef SOUND_H
#define SOUND_H

#define ON 0xFF
#define OFF 0x00
#define RESET 0x80
#define HIGH 0xFF
#define LOW 0x00

#define SOUND_CHIP NR52_REG
#define SOUND_OUTPUT NR51_REG

#define INIT_SOUND SOUND_CHIP=RESET; SOUND_OUTPUT=ON;

#define MASTER_VOLUME NR50_REG

#define CH1_SWEEP NR10_REG
#define CH1_DUTY_CYCLE NR11_REG
#define CH1_VOL NR12_REG
#define CH1 NR14_REG

#define CH2_SWEEP NR20_REG
#define CH2_DUTY_CYCLE NR21_REG
#define CH2_VOL NR22_REG
#define CH2 NR24_REG

#define PULSE_WIDTH_12_5 0x00
#define PULSE_WIDTH_25 0x40
#define PULSE_WIDTH_50 0x80
#define PULSE_WIDTH_75 0xc0

#define SET_PULSE_WIDTH(CH, DUTY) (CH## _DUTY_CYCLE) = (PULSE_WIDTH_## DUTY);

UINT getFrequencies(int note, int bend);

extern const UINT note_frequencies[];

void play_freq_ch1 (UINT f, short bend);
void play_freq_ch2 (UINT f, short bend);

#endif
