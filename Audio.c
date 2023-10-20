#include "MKL25Z4.h" 
#include "MotorControl.h"
#include "Audio.h"
#include "cmsis_os2.h"

#define PTE30_Pin 30

int end_song[] = {

};

void initAudio(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTB->PCR[PTE30_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTE30_Pin] |= PORT_PCR_MUX (3);

	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC (1);

	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD (1) | TPM_SC_PS (7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);

	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM0_C3SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
}

void playEndAudio(int* notes, int size) {
	for (int i = 0; i < size; ++i) {
		if (notes[i] != 0) {
			int freq = freq_calc(notes[i]);
			TPM2->MOD = freq;
			TPM2_C0V = duty_cycle_calc(freq, (float) 0.5);	
		}
	}
}

void stop_sound(void) {
	TPM0_MOD = 100; 
	TPM0_C3V = 0;
	osDelay(50);
}
