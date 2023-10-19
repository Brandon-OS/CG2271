#include "MKL25Z4.h" 
#include "MotorControl.h"

#define C_Note 262
#define D_Note 294
#define E_Note 330
#define F_Note 349
#define G_Note 392
#define A_Note 440
#define B_Note 494

#define PTE22_Pin 22
#define PTE23_Pin 23

void initAudio(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	PORTB->PCR[PTE22_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTE22_Pin] |= PORT_PCR_MUX (3);

	PORTB->PCR[PTE23_Pin] &= ~PORT_PCR_MUX_MASK; 
	PORTB->PCR[PTE23_Pin] |= PORT_PCR_MUX (3);

	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC (1);

	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD (1) | TPM_SC_PS (7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM2_C0SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
}

void playAudio(int* notes, int size) {
	for (int i = 0; i < size; ++i) {
		int freq = freq_calc(notes[i]);
		TPM2->MOD = freq;
		TPM2_C0V = duty_cycle_calc(freq, 1);
	}
}