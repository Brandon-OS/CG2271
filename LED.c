#include "MKL25Z4.h" 
#define MASK(x) (1 << (x))

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

void cycleLED(void) {
	
}


void runningLED(void) {
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
		for (int i = 0; i < 10; i++) {
			setLEDOutput(nodes[i], ON);
			osDelay(2000);
			setLEDOutput(nodes[i], OFF);
			//if (i != 0) setLEDOutput(nodes[i-1], OFF);
			//osDelay(2000);
		}
	
	
	/**
	int left = 1;
	int right = 0;
	for (;;) {
		if (left == 1) {
			for (int i = 0; i < 10; i++) {
				setLEDOutput(nodes[i], ON);
				if (i != 0) setLEDOutput(nodes[i-1], OFF);
				if (i == 9) {
					left = 0;
					right = 1;
					break;
				}
			}
		}
		
		if (right == 1) {
			for (int i = 9; i >= 0; i--) {
				setLEDOutput(nodes[i], ON);
				if (i!= 9) setLEDOutput(nodes[i+1], OFF);
				if (i == 0) {
					left = 1;
					right = 0;
					break;
				}
			}
		}
	}
	**/
}