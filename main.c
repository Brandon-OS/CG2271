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
 * Global Definitions
 *---------------------------------------------------------------------------*/
 uint8_t rx_data;
 
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

void motor_thread (void *argument) {
	for (;;) {
		rightForward();
		osDelay(10000);
		leftBackward();
		osDelay(10000);
	}
	
	/*
	char move = parseMove(rx_data);
	switch(move) {
		case 'F':
			forward();
		case 'L':
			left();
		case 'R':
			right();
		case 'S':
			stop();
	}
	*/
}

void brain_thread (void *argument) {
	for (;;) {
		rx_data = serial();
	}
}

void audio_thread (void *argument) {
	for (;;) {
		playEndAudio();
	}
}

int main (void) {
  // System Initialization
	SystemCoreClockUpdate();
	//initAudio();
	//initMotor();
	//initUART2(BAUD_RATE);
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	//osThreadNew(brain_thread, NULL, NULL);
	//osThreadNew(audio_thread, NULL, NULL);
	runningLedThread();
	//flashLedStationaryThread();
  //osThreadNew(motor_thread, NULL, NULL);    // Create application main thread
	osKernelStart();
/**	// Start thread execution
  for (;;) {
		playEndAudio();
	}**/
}
