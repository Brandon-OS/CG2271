#include "MKL25Z4.h" 
#include "cmsis_os2.h"
#include "common.h"
#define MASK(x) (1 << (x))
#define NUM_LEDS 10

typedef struct node {
	int portNumber;
	PORT_Type* port;
	GPIO_Type* gpio;
} led_node;

typedef enum state {
	ON, OFF
} states;

static void delay(volatile uint32_t nof) {  
	while(nof!=0) {
    __asm("NOP");    
		nof--;
 }}


led_node initLED(int portNumber, PORT_Type* port, GPIO_Type* gpio) {
	SIM->SCGC5 |= ((SIM_SCGC5_PORTA_MASK) | (SIM_SCGC5_PORTC_MASK) | (SIM_SCGC5_PORTD_MASK));
	led_node node;
	node.portNumber = portNumber;
	node.port = port;
	node.gpio = gpio;
	
	node.port->PCR[node.portNumber] &= ~PORT_PCR_MUX_MASK;
	node.port->PCR[node.portNumber] |= PORT_PCR_MUX(1);
	node.gpio->PDDR |= (MASK(node.portNumber));
	
	return node;
}

void setLEDOutput(led_node node, states state) {
	switch (state) {
		case ON:
			node.gpio->PDOR |= MASK(node.portNumber);
			break;
		case OFF:
			node.gpio->PDOR &= ~(MASK(node.portNumber));
			break;
	}
}

led_node* initFrontLED(void) {
	led_node node0 = initLED(13, PORTA, PTA);
	led_node node1 = initLED(5, PORTD, PTD);
	led_node node2 = initLED(0, PORTD, PTD);
	led_node node3 = initLED(2, PORTD, PTD);
	led_node node4 = initLED(3, PORTD, PTD);
	led_node node5 = initLED(16, PORTA, PTA);
	led_node node6 = initLED(17, PORTC, PTC);
	led_node node7 = initLED(16, PORTC, PTC);
	led_node node8 = initLED(13, PORTC, PTC);
	led_node node9 = initLED(12, PORTC, PTC);

	led_node nodes[10] = {node0, node1, node2, node3, node4, node5, node6, node7, node8, node9};
	return nodes;
}

led_node* initBackLED() {
	led_node node0 = initLED(21, PORTE, PTE);
	led_node node1 = initLED(20, PORTE, PTE);
	led_node node2 = initLED(5, PORTE, PTE);
	led_node node3 = initLED(4, PORTE, PTE);
	led_node node4 = initLED(3, PORTE, PTE);
	led_node node5 = initLED(2, PORTE, PTE);
	led_node node6 = initLED(11, PORTB, PTB);
	led_node node7 = initLED(10, PORTB, PTB);
	led_node node8 = initLED(9, PORTB, PTB);
	led_node node9 = initLED(8, PORTB, PTB);

	led_node nodes[10] = {node0, node1, node2, node3, node4, node5, node6, node7, node8, node9};
	return nodes;
}


void offAllLED(void) {
	led_node* nodes = initFrontLED();
	for (int i = 0; i < NUM_LEDS; i++) {
		setLEDOutput(nodes[i], OFF);
	}
}

void flashLEDFast(void *argument) {
	led_node* nodes = initBackLED();
	offAllLED();
	for (;;) {
		osEventFlagsWait(flagStation, 0x01, osFlagsNoClear, osWaitForever);
		setLEDOutput(nodes[0], ON);
		setLEDOutput(nodes[1], ON);
		setLEDOutput(nodes[2], ON);
		setLEDOutput(nodes[3], ON);
		setLEDOutput(nodes[4], ON);
		setLEDOutput(nodes[5], ON);
		setLEDOutput(nodes[6], ON);
		setLEDOutput(nodes[7], ON);
		setLEDOutput(nodes[8], ON);
		setLEDOutput(nodes[9], ON);
		
		osDelay(250);
		
		setLEDOutput(nodes[0], OFF);
		setLEDOutput(nodes[1], OFF);
		setLEDOutput(nodes[2], OFF);
		setLEDOutput(nodes[3], OFF);
		setLEDOutput(nodes[4], OFF);
		setLEDOutput(nodes[5], OFF);
		setLEDOutput(nodes[6], OFF);
		setLEDOutput(nodes[7], OFF);
		setLEDOutput(nodes[8], OFF);
		setLEDOutput(nodes[9], OFF);
		
		osDelay(250);
	}
}

void flashLEDSlow(void *argument) {
	led_node* nodes = initBackLED();
	offAllLED();
	for (;;) {
		osEventFlagsWait(flagRunning, 0x01, osFlagsNoClear, osWaitForever);
		setLEDOutput(nodes[0], ON);
		setLEDOutput(nodes[1], ON);
		setLEDOutput(nodes[2], ON);
		setLEDOutput(nodes[3], ON);
		setLEDOutput(nodes[4], ON);
		setLEDOutput(nodes[5], ON);
		setLEDOutput(nodes[6], ON);
		setLEDOutput(nodes[7], ON);
		setLEDOutput(nodes[8], ON);
		setLEDOutput(nodes[9], ON);
		
		osDelay(500);
		
		setLEDOutput(nodes[0], OFF);
		setLEDOutput(nodes[1], OFF);
		setLEDOutput(nodes[2], OFF);
		setLEDOutput(nodes[3], OFF);
		setLEDOutput(nodes[4], OFF);
		setLEDOutput(nodes[5], OFF);
		setLEDOutput(nodes[6], OFF);
		setLEDOutput(nodes[7], OFF);
		setLEDOutput(nodes[8], OFF);
		setLEDOutput(nodes[9], OFF);
		
		osDelay(500);
	}
}

void lightAllLED(void *argument) {
	led_node* nodes = initFrontLED();
	for (int i = 0; i < NUM_LEDS; i++) {
		osEventFlagsWait(flagStation, 0x01, osFlagsNoClear, osWaitForever);
		setLEDOutput(nodes[i], ON);
	}
}

void runningLED(void *argument) {
	led_node* nodes = initFrontLED();
	offAllLED();
	int left = 1;
	int right = 0;
	for (;;) {
		osEventFlagsWait(flagRunning, 0x01, osFlagsNoClear, osWaitForever);
		if (left == 1) {
			for (int i = 0; i < NUM_LEDS; i++) {
				setLEDOutput(nodes[i], ON);
				osDelay(50);
				setLEDOutput(nodes[i], OFF);
				if (i == 9) {
					left = 0;
					right = 1;
					break;
				}
			}
		}
		
		if (right == 1) {
			for (int i = NUM_LEDS-1; i >= 0; i--) {
				setLEDOutput(nodes[i], ON);
				osDelay(50);
				setLEDOutput(nodes[i], OFF);
				if (i == 0) {
					left = 1;
					right = 0;
					break;
				}
			}
		}
	}
}