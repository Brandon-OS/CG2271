#include "MKL25Z4.h" 
#define MASK(x) (1 << (x))

typedef struct node {
	int portNumber;
	PORT_Type* port;
} led_node;

typedef enum state {
	ON, OFF
} states;

void initLED(led_node node) {
	node.port->PCR[node.portNumber] &= ~PORT_PCR_MUX_MASK;
	node.port->PCR[node.portNumber] |= PORT_PCR_MUX(1);
	node.port->PDDR |= (MASK(node.portNumber));
}

void setLEDOutput(led_node node, states state) {
}

void cycleLED(void) {
	
}