#include "MKL25Z4.h" 

#define SW_POS 6
#define RED_LED 18	 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1	 // PortD Pin 1
#define MASK(x) (1 << (x))
#define PTB0_Pin 0
#define PTB1_Pin 1
#define C_Note 262
#define D_Note 294
#define E_Note 330
#define F_Note 349
#define G_Note 392
#define A_Note 440
#define B_Note 494

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

unsigned int counter = 0;
unsigned int color = 0;
unsigned int port = 0;
static uint8_t rx_data;

enum color_t
{
	RED,
	GREEN,
	BLUE
};

enum color_t color_list[3] = {RED, GREEN, BLUE};

static void delay(volatile uint32_t nof)
{
	while (nof != 0)
	{
		__asm("NOP");
		nof--;
	}
}

void clear_led()
{
	PTB->PDOR |= MASK(RED_LED);
	PTB->PDOR |= MASK(GREEN_LED);
	PTD->PDOR |= MASK(BLUE_LED);
}

void led_control(enum color_t color)
{
	clear_led();
	if (color == RED)
		PTB->PDOR &= ~MASK(RED_LED);
	if (color == GREEN)
		PTB->PDOR &= ~MASK(GREEN_LED);
	if (color == BLUE)
		PTD->PDOR &= ~MASK(BLUE_LED);
}

// UART2 Transmit Poll
void UART2_Transmit_Poll(uint8_t data) {
	while(!(UART2->S1 & UART_S1_TDRE_MASK));
	UART2->D = data;
}

// UART2 Receive Poll
uint8_t UART2_Receive_Poll(void) {
	while(!(UART2->S1 & UART_S1_RDRF_MASK));
	return (UART2->D);
}




/**
//-----------------------Interrupts-----------------------//

void PORTD_IRQHandler()
{
	// Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);

	// Updating some variable / flag
	color++;
	delay(0xFu);

	// Clear INT Flag
	PORTD->ISFR = 0xffffffff;
}
*/

void InitGPIO(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}
/*
void initSwitch(void)
{
	// enable clock for PortD
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);
	PORTD->PCR[SW_POS] |= (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a));

	// Set PORT D Switch bit to input
	PTD->PDDR &= ~MASK(SW_POS);

	// Enable Interrupts
	NVIC_SetPriority(PORTD_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

void initPWM (void) {

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
*/


/* Init UART2 */

void initUART2 (uint32_t baud_rate) {

	uint32_t divisor, bus_clock;

	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[UART_TX_PORTE22] &= PORT_PCR_MUX_MASK; 
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);

	PORTE->PCR [UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;

	PORTE->PCR [UART_RX_PORTE23] |= PORT_PCR_MUX (4);

	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));

	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2; 
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR (divisor >> 8);
	UART2->BDL = UART_BDL_SBR (divisor);

	UART2->C1 = 0; 
	UART2->S2 = 0;
	UART2->C3 = 0;

	UART2->C2 = ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
}

// Delay Routine
int tBrain_main()
{
	SystemCoreClockUpdate();
// setup code for UART, RGD LED, etc.
InitGPIO();
	initUART2(BAUD_RATE);
	while(1)
{
rx_data = UART2_Receive_Poll();
	if (rx_data == 0x31) {
		led_control(color_list[0]);
	} else if (rx_data == 0x33) {
		led_control(color_list[1]);
	} else if (rx_data == 0x35) {
		led_control(color_list[1]);
	} else {
		clear_led();
	}
	/*
	switch(rx_data){
		case 0x31:
			led_control(color_list[0]);
		case 0x30:
			clear_led();
	}
	*/
/*
Perform necessary if-else checks to compare rx_data with expected values.
Take appropriate action if any expected value is received
*/
}
}