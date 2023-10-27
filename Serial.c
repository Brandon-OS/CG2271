#include "MKL25Z4.h" // Device header

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

// UART2 Receive Poll
uint8_t UART2_Receive_Poll(void) {
	while(!(UART2->S1 & UART_S1_RDRF_MASK));
	return (UART2->D);
}

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

	NVIC_SetPriority(UART2_IRQn, 1);      
	NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);      
  UART2->C2 |= UART_C2_RIE_MASK | UART_C2_TE_MASK | UART_C2_RE_MASK;
}

uint8_t serial(void) {
	while(1) {
		uint8_t rx_data = UART2_Receive_Poll();
		return rx_data;
	}
}