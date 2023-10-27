#ifndef LED_H
#define LED_H
#include "MKL25Z4.h"
#include "cmsis_os2.h"
#include "common.h"

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

#endif
