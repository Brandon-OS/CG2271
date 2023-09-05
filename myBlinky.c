#include "MKL25Z4.h" // Device header

#define RED_LED 18	 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1	 // PortD Pin 1
#define MASK(x) (1 << (x))

unsigned int counter = 0;
unsigned int color = 0;
unsigned int port = 0;

enum color_t
{
	RED,
	GREEN,
	BLUE
};

enum color_t color_list[3] = {RED, GREEN, BLUE};

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

//-----------------------Interrupts-----------------------//

void PORTD_IRQHandler()
{
	// Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);

	// Updating some variable / flag
	if (PORTD->ISFR)
	{
		color++;
	}

	// Clear INT Flag
	PORTD->ISFR = 0xffffffff;
}

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

void initSwitch(void)
{
	// enable clock for PortD
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);

	/* Select GPIO and enable pull-up resistors and interrupts on
	falling edges of pin connected to switch*/
	PORTD->PCR[SW_POS] |= (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a));

	// Set PORT D Switch bit to input
	PTD->PDDR &= ~MASK(SW_POS);

	// Enable Interrupts
	NVIC_SetPriority(PORTD_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn)
}

static void delay(volatile uint32_t nof)
{
	while (nof != 0)
	{
		__asm("NOP");
		nof--;
	}
}

int main(void)
{
	initSwitch();
	InitGPIO();

	while (1)
	{
		clear_led();
		led_control(color_list[color % 3]);
	}
}