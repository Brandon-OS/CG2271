/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "MKL25Z4.h" 
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 

unsigned int led_on = 1;
unsigned int led_off = 0;

#define RED_LED 18
#define GREEN_LED 19
#define BLUE_LED 1
#define MASK(x) (1 << (x))


enum colors {
	RED, GREEN, BLUE
};

void offRGB() {
	PTB->PDOR |= MASK(RED_LED);
	PTB->PDOR |= MASK(GREEN_LED);
	PTD->PDOR |= MASK(BLUE_LED);
}

void led_control(enum colors color, int isON) {
	if (!isON) {
		offRGB();
		return;
	}
	
	if (color == RED)
		PTB->PDOR &= ~MASK(RED_LED);
	if (color == GREEN) 
		PTB->PDOR &= ~MASK(GREEN_LED);
	if (color == BLUE) 
		PTD->PDOR &= ~MASK(BLUE_LED);
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

 
void led_red_thread (void *argument) {
  // ...
  for (;;) {
		led_control(RED, led_on);
		osDelay(1000);
		led_control(RED, led_off);
		osDelay(1000);
	}
}

void led_green_thread (void *argument) {
  // ...
  for (;;) {
		led_control(GREEN, led_on);
		osDelay(1000);
		led_control(GREEN, led_off);
		osDelay(1000);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	InitGPIO();
	offRGB();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(led_green_thread, NULL, NULL);
  osThreadNew(led_red_thread, NULL, NULL);    // Create application main thread
	osKernelStart();                      // Start thread execution
  for (;;) {
	}
}
