#include "MKL25Z4.h" 

#define PTB0_Pin 0
#define PTB1_Pin 1
#define MOTOR_FREQ 500
#define MOTOR_DUTY 1

int freq_calc(int freq) {
	return ((48000000 / 128) / freq) - 1;  // assume 48 MHz and 128 prescaler (PS)
}

int duty_cycle_calc (int freq, float duty_cycle) {
	return (((48000000 / 128) / freq) - 1) * duty_cycle;
}

char parseMove(uint8_t data) {
	switch (data) {
		case 0x30:
			return 'S';
		case 0x31:
			return 'F';
		case 0x32:
			return 'L';
		case 0x33:
			return 'R';
		case 0x34:
			return 'B';
	}
}

void initMotor (void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX (3);

	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK; 
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX (3);

	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC (1);

	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD (1) | TPM_SC_PS (7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM1_C0SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
}

void forward(void) {
	TPM1->MOD = freq_calc(MOTOR_FREQ); // frequency
	TPM1_C0V = duty_cycle_calc(MOTOR_FREQ, MOTOR_DUTY);
	TPM1_C1V = duty_cycle_calc(MOTOR_FREQ, MOTOR_DUTY);	// duty cycle
}

void left(void) {
	TPM1->MOD = freq_calc(MOTOR_FREQ); // frequency
	TPM1_C0V = duty_cycle_calc(MOTOR_FREQ, MOTOR_DUTY/2);
	TPM1_C1V = duty_cycle_calc(MOTOR_FREQ, MOTOR_DUTY);	// duty cycle
}

void right(void) {
	TPM1->MOD = freq_calc(MOTOR_FREQ); // frequency
	TPM1_C0V = duty_cycle_calc(MOTOR_FREQ, MOTOR_DUTY);
	TPM1_C1V = duty_cycle_calc(MOTOR_FREQ, MOTOR_DUTY/2);	// duty cycle
}

void stop(void) {
	TPM1->MOD = freq_calc(MOTOR_FREQ); // frequency
	TPM1_C0V = duty_cycle_calc(MOTOR_FREQ, 0);
	TPM1_C1V = duty_cycle_calc(MOTOR_FREQ, 0);	// duty cycle
}