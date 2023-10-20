#include "MKL25Z4.h" 

#define PTB0_Pin 0 //FRONT RIGHT
#define PTB1_Pin 1 //BACK RIGHT
#define PTB2_Pin 2 //FRONT LEFT
#define PTB3_Pin 3 //BACK LEFT
#define MOTOR_FREQ 500
#define MOTOR_DUTY 1

int pwm_cal(float duty_cycle) {
	return 6000*(duty_cycle/100);
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
	
	PORTB->PCR[PTB2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB2_Pin] |= PORT_PCR_MUX (3);

	PORTB->PCR[PTB3_Pin] &= ~PORT_PCR_MUX_MASK; 
	PORTB->PCR[PTB3_Pin] |= PORT_PCR_MUX (3);

	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC (1);
	
	TPM1->MOD = 6000;
	TPM2->MOD = 6000;
	
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD (1) | TPM_SC_PS (4));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD (1) | TPM_SC_PS (4));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM1_C0SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
	
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM1_C1SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
	
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM2_C0SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
	
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
	TPM2_C1SC |= (TPM_CnSC_ELSB (1) | TPM_CnSC_MSB (1));
}

void rightMotorMove(int dir, float val) {
	uint32_t pwm = pwm_cal(val);
	switch (dir) {
		case 1: 
			TPM1_C0V = pwm;
			TPM1_C1V = 0;
			break;
		case -1:
			TPM1_C0V = 0;
			TPM1_C1V = pwm;
			break;
	}
}

void leftMotorMove(int dir, float val) {
	uint32_t pwm = pwm_cal(val);
	switch (dir) {
		case 1: 
			TPM2_C0V = pwm;
			TPM2_C1V = 0;
			break;
		case -1:
			TPM2_C0V = 0;
			TPM2_C1V = pwm;
			break;
	}
}

void forward(void) {
	rightMotorMove(1, 100);
	leftMotorMove(1, 100);
}

void backward(void) {
	rightMotorMove(-1, 100);
	leftMotorMove(-1, 100);
}

void left(void) {
	rightMotorMove(-1, 100);
	leftMotorMove(1, 100);
}

void right(void) {
	rightMotorMove(1, 100);
	leftMotorMove(-1, 100);
}

void stop(void) {
	rightMotorMove(1, 0);
	leftMotorMove(1, 0);
}

void rightForward(void) {
	rightMotorMove(1, 5);
	leftMotorMove(1, 100);
}

void rightBackwark(void) {
	rightMotorMove(-1, 5);
	leftMotorMove(-1, 100);
}

void leftForward(void) {
	rightMotorMove(1, 100);
	leftMotorMove(1, 5);
}

void leftBackward(void) {
	rightMotorMove(-1, 100);
	leftMotorMove(-1, 5);
}