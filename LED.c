#include "MKL25Z4.h" 
#define MASK(x) (1 << (x))
/*
typedef struct node {
	int portNumber;
	PORT_Type* port;
} led_node;

typedef enum state {
	ON, OFF
} states;

void initLED(int portNumber, PORT_Type* port) {
	led_node node;
	node.portNumber = portNumber;
	node.port = port;
	
	node.port->PCR[node.portNumber] &= ~PORT_PCR_MUX_MASK;
	node.port->PCR[node.portNumber] |= PORT_PCR_MUX(1);
	PTB->PDDR |= (MASK(node.portNumber));
}

void setLEDOutput(led_node node, states state) {
	switch (state) {
		case ON:
			PTB->PDOR &= ~MASK(node.portNumber);
			break;
		case OFF:
			PTB->PDOR |= MASK(node.portNumber);
	}
}

void cycleLED(void) {
	
}


void runningLED(void) {
	led_node node0 = initLED(13, PORTA);
	led_node node1 = initLED(5, PORTD);
	led_node node2 = initLED(0, PORTD);
	led_node node3 = initLED(2, PORTD);
	led_node node4 = initLED(3, PORTD);
	led_node node5 = initLED(16, PORTA);
	led_node node6 = initLED(17, PORTC);
	led_node node7 = initLED(16, PORTC);
	led_node node8 = initLED(13, PORTC);
	led_node node9 = initLED(12, PORTC);

	led_node nodes[10] = {node0, node1, node2, node3, node4, node5, node6, node7, node8, node9};
	int left = 1;
	int right = 0;
	for (;;) {
		if (left == 1) {
			for (int i = 0; i < 10; i++) {
				setLEDOutput(nodes[i], ON);
				setLEDOutput(nodes[i-1], OFF);
				if (i == 9) {
					left = 0;
					right = 0;
					break;
				}
			}
		}
	}
}
*/