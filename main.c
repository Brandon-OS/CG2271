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
#include "LED.h"

 /*----------------------------------------------------------------------------
 * Flags
 *---------------------------------------------------------------------------*/
 
 osEventFlagsId_t 
	flagRunningSound, flagEndingSound, 
	flagFinish,
	flagRunning, flagStation
;
 
 int run_song[] = {
	NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, //8  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,//13
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
};
 

void run_sound_thread() {
	int size = 88;
	for (;;) {
		for (int i = 0; i < size; ++i) {
			osEventFlagsWait(flagRunningSound, 0x01, osFlagsNoClear, osWaitForever);
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
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/


volatile uint8_t rx_data;

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
	
  if (UART2->S1 & UART_S1_RDRF_MASK) {
    rx_data = UART2->D;
  }
}

void poll_thread (void *argument) {
	for (;;) {
		rx_data = UART2_Receive_Poll();
	}
}

void end_sound_thread (void *argument) {
	for (;;) {
		osEventFlagsWait(flagEndingSound, 0x01, osFlagsWaitAny, osWaitForever);
		playEndAudio();
	}
}

void control (void* argument) {
	osEventFlagsClear(flagRunning, 0x01);
	osEventFlagsSet(flagRunningSound, 0x01);
	osEventFlagsSet(flagStation, 0x01);
	for (;;) {
		printf("Value of rx_data: %u\n", rx_data);
		if (rx_data == 0x30) { //forward
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			forward();
		} else if (rx_data == 0x31) { // backward
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			backward();
		} else if (rx_data == 0x32) { // left 
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			left();
		} else if (rx_data == 0x33) { // right 
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			right();
		} else if (rx_data == 0x34) { //stop
			osEventFlagsSet(flagStation, 0x01);
			osEventFlagsClear(flagRunning, 0x01);
			stop();
		} else if (rx_data == 0x35) { //right forward
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			rightForward();
		} else if (rx_data == 0x36) { //left forward
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			leftForward();
		} else if (rx_data == 0x37) { //right backward 
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			rightBackward();
		} else if (rx_data == 0x38) { //left backward
			osEventFlagsSet(flagRunning, 0x01);
			osEventFlagsClear(flagStation, 0x01);
			leftBackward();
		} else if (rx_data == 0x39) { //finish
			osEventFlagsClear(flagRunningSound, 0x01);
			osEventFlagsSet(flagEndingSound, 0x01);
		} else if (rx_data == 0x40) { // stop sound
			stopSound();
			osEventFlagsClear(flagRunningSound, 0x01);
			osEventFlagsClear(flagEndingSound, 0x01);			
		} else if (rx_data == 0x41) { // on run sound
			osEventFlagsClear(flagEndingSound, 0x01);
			osEventFlagsSet(flagRunningSound, 0x01);
		}
		
	}
}

 void motor_thread(void* argument) {
	 for (;;) {
			forward();
			backward();
	 }
 }
int main (void) {
  // System Initialization
	SystemCoreClockUpdate();
	flagEndingSound = osEventFlagsNew(NULL);
	flagRunningSound = osEventFlagsNew(NULL);
	flagFinish = osEventFlagsNew(NULL);
	
	flagStation = osEventFlagsNew(NULL);
	flagRunning = osEventFlagsNew(NULL);
	
	initAudio();
	initMotor();
	initUART2(BAUD_RATE);
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(end_sound_thread, NULL, NULL);
	osThreadNew(run_sound_thread, NULL, NULL);
	//osThreadNew(control, NULL, NULL);
	osThreadNew(poll_thread, NULL, NULL);
	runningLedThread();
	flashLedStationaryThread();
	//osThreadNew(motor_thread, NULL, NULL);
	osKernelStart();
  for (;;) {}
}
