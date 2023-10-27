#include "MKL25Z4.h" 
#include "MotorControl.h"
#include "Audio.h"
#include "cmsis_os2.h"

#define BAUD_RATE 9600
#define PTE30_Pin 30

int freq_calc(int freq) {
	return ((48000000 / 128) / freq) - 1;  // assume 48 MHz and 128 prescaler (PS)
}

int duty_cycle_calc (int freq, float duty_cycle) {
	return (((48000000 / 128) / freq) - 1) * duty_cycle;
}

int end_song[] = {
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2
};

void initAudio(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[PTE30_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE30_Pin] |= PORT_PCR_MUX (3);

	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC (1);

	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD (1) | TPM_SC_PS (7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);

	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM0_C3SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
}

void playEndAudio() {
	int size = 13; //88;
	for (int i = 0; i < size; ++i) {
		if (end_song[2*i] != 0) {
			for (int j = 0; j < 16/end_song[2*i+1]; j++) {
				int freq = freq_calc(end_song[2*i]);
				TPM0->MOD = freq;
				TPM0_C3V = duty_cycle_calc(freq, (float) 0.1);	
			}
		}
		osDelay(200);
	}
}

void stopSound(void) {
	TPM0_MOD = 100; 
	TPM0_C3V = 0;
	osDelay(50);
}
