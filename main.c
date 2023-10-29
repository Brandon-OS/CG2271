/*----------------------------------------------------------------------------
 * Include Headers
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "MKL25Z4.h" 
#include "MotorControl.h"
#include "Serial.h"
#include "Audio.h"
//#include "common.h"
//#include "LED.h"

#define MASK(x) (1 << (x))
#define NUM_LEDS 10

bool runSound = 0;
bool endSound = 0;
bool finish = 0;
bool running = 0;
bool station = 0;
bool isAlight = 0;

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


void offAllLED(led_node* nodes) {
	for (int i = 0; i < NUM_LEDS; i++) {
		setLEDOutput(nodes[i], OFF);
	}
}

void flashLEDFast(void *argument) {
	led_node* nodes = initBackLED();
	for (;;) {
		//osEventFlagsWait(flagStation, 0x01, osFlagsNoClear, osWaitForever);
		if (station == 1) {
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
}

void flashLEDSlow(void *argument) {
	led_node* nodes = initBackLED();
	for (;;) {
		//osEventFlagsWait(flagRunning, 0x01, osFlagsNoClear, osWaitForever);
		if (running == 1) {
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
}

void lightAllLED(void *argument) {
	led_node* nodes = initFrontLED();
	for (;;) {
		if (station) {
	for (int i = 0; i < NUM_LEDS; i++) {
		//osEventFlagsWait(flagStation, 0x01, osFlagsNoClear, osWaitForever);
			isAlight = 1;
		setLEDOutput(nodes[i], ON);
	}
}
}
}

void runningLED(void *argument) {
	led_node* nodes = initFrontLED();
	int left = 1;
	int right = 0;
	for (;;) {
		//osEventFlagsWait(flagRunning, 0x01, osFlagsNoClear, osWaitForever);
		if (running == 1) {
			if (isAlight) offAllLED(nodes);
			isAlight = 0;
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
}

static uint64_t runningled_stk[64];
static uint64_t flashled_stk1[64];
static uint64_t flashled_stk2[64];
static uint64_t allled_stk[64];

const osThreadAttr_t runningled_attr = {
	.stack_mem  = &runningled_stk[0],
  .stack_size = sizeof(runningled_stk)
};
const osThreadAttr_t flashled_attr1 = {
	.stack_mem  = &flashled_stk1[0],
  .stack_size = sizeof(flashled_stk1)
};
const osThreadAttr_t flashled_attr2 = {
	.stack_mem  = &flashled_stk2[0],
  .stack_size = sizeof(flashled_stk2)
};
const osThreadAttr_t allled_attr = {
	.stack_mem  = &allled_stk[0],
  .stack_size = sizeof(allled_stk)
};

void runningLED(void *argument);
void lightAllLED(void *argument);
void flashLEDFast(void *argument);
void flashLEDSlow(void *argument);

void test(void);

osThreadId_t runningLedThread(void) {
	return osThreadNew(runningLED, NULL, &runningled_attr);
}
osThreadId_t flashLedStationaryThread(void) {
	return osThreadNew(flashLEDFast, NULL, &flashled_attr1);
}
osThreadId_t flashLedMovingThread(void) {
	return osThreadNew(flashLEDSlow, NULL, &flashled_attr2);
}
osThreadId_t lightAllLedThread(void) {
	return osThreadNew(lightAllLED, NULL, &allled_attr);
}
 /*----------------------------------------------------------------------------
 * Flags
 *---------------------------------------------------------------------------*/
 
//osEventFlagsId_t flagRunningSound, flagEndingSound, flagFinish, flagRunning, flagStation;

const osThreadAttr_t threadAttr = {
	.priority = osPriorityRealtime
};
 
 int run_song[] = {
	NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, 
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
};
 
volatile uint8_t rx_data;

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
	
  if (UART2->S1 & UART_S1_RDRF_MASK) {
    rx_data = UART2->D;
  }
}

void run_sound_thread() {
	int size = 88;
	for (;;) {
		for (int i = 0; i < size; ++i) {
			//osEventFlagsWait(flagRunningSound, 0x01, osFlagsNoClear, osWaitForever);
			if (runSound == 1) {
				if (run_song[2*i] != 0) {
					for (int j = 0; j < 16/run_song[2*i+1]; j++) {
						int freq = freq_calc(run_song[2*i]);
						TPM0->MOD = freq;
						TPM0_C3V = duty_cycle_calc(freq, (float) 0.1);	
					}
				}
				osDelay(200);
			}
		}
	}
}

void end_sound_thread (void *argument) {
	for (;;) {
		//osEventFlagsWait(flagEndingSound, 0x01, osFlagsWaitAny, osWaitForever);
		if (endSound == 1){
			playEndAudio();
		}
	}
}

void motor_thread (void *argument) {
	for (;;) {
		//osEventFlagsWait(flagRunning, 0x01, osFlagsNoClear, osWaitForever);
		forward();
	}
}

void control (void* argument) {
	//osEventFlagsClear(flagRunning, 0x01);
	//osEventFlagsClear(flagEndingSound, 0x01);
	//osEventFlagsSet(flagRunningSound, 0x01);
	//osEventFlagsSet(flagStation, 0x01);
	running = 0;
	endSound = 0;
	runSound = 1;
	station = 1;
	for (;;) {
		if (rx_data == 0x30) { 
			//osEventFlagsClear(flagStation, 0x01);
			//osEventFlagsSet(flagRunning, 0x01);
			station = 0; 
			running = 1; 
			forward();
		} else if (rx_data == 0x31) { 
			//osEventFlagsClear(flagStation, 0x01);
			//osEventFlagsSet(flagRunning, 0x01);
			station = 0; 
			running = 1; 			
			backward();
		} else if (rx_data == 0x32) { 
			//osEventFlagsClear(flagStation, 0x01);
			//osEventFlagsSet(flagRunning, 0x01);
			station = 0; 
			running = 1;
			left();
		} else if (rx_data == 0x33) {  
			//osEventFlagsSet(flagRunning, 0x01);
			//osEventFlagsClear(flagStation, 0x01);
			station = 0; 
			running = 1;
			right();
		} else if (rx_data == 0x34) { 
			//osEventFlagsClear(flagRunning, 0x01);
			//osEventFlagsSet(flagStation, 0x01);
			station = 1; 
			running = 0;
			stop();
		} else if (rx_data == 0x35) { 
			//osEventFlagsSet(flagRunning, 0x01);
			//osEventFlagsClear(flagStation, 0x01);
			station = 0; 
			running = 1;
			leftForward();
		} else if (rx_data == 0x36) { 
			//osEventFlagsSet(flagRunning, 0x01);
			//osEventFlagsClear(flagStation, 0x01);
			station = 0; 
			running = 1;
			rightForward();
			
		} else if (rx_data == 0x37) {  
			//osEventFlagsSet(flagRunning, 0x01);
			//osEventFlagsClear(flagStation, 0x01);
			station = 0; 
			running = 1;
			leftBackward();
		} else if (rx_data == 0x38) {
			//osEventFlagsSet(flagRunning, 0x01);
			//osEventFlagsClear(flagStation, 0x01);
			station = 0; 
			running = 1;
			rightBackward();
			
		} else if (rx_data == 0x39) { 
			//osEventFlagsClear(flagRunningSound, 0x01);
			//osEventFlagsSet(flagEndingSound, 0x01);
			runSound = 0;
			endSound = 1;
		} else if (rx_data == 0x40) {
			stopSound();
			endSound = 0;
			runSound = 0;
			//osEventFlagsClear(flagRunningSound, 0x01);
			//osEventFlagsClear(flagEndingSound, 0x01);			
		} else if (rx_data == 0x41) {
			endSound = 0;
			runSound = 1;
			//osEventFlagsClear(flagEndingSound, 0x01);
			//osEventFlagsSet(flagRunningSound, 0x01);
		}
		osDelay(50); 
	}
}
 
int main (void) {
  // System Initialization
	SystemCoreClockUpdate();
	
	//flagEndingSound = osEventFlagsNew(NULL);
	//flagRunningSound = osEventFlagsNew(NULL);
	//flagFinish = osEventFlagsNew(NULL);
	//flagStation = osEventFlagsNew(NULL);
	//flagRunning = osEventFlagsNew(NULL);
	
	initMotor();
	initAudio();
	initUART2();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(end_sound_thread, NULL, NULL);
	osThreadNew(run_sound_thread, NULL, NULL);
	osThreadNew(control, NULL, &threadAttr);
	//osThreadNew(motor_thread, NULL, NULL);
	//osThreadNew(poll_thread, NULL, NULL);
	runningLedThread();
	flashLedStationaryThread();
	flashLedMovingThread();
	lightAllLedThread();
	
	//osThreadNew(motor_thread, NULL, NULL);
	osKernelStart();
  for (;;) {}
}
