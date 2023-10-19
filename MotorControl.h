#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include "MKL25Z4.h"

int freq_calc(int freq);
int duty_cycle_calc(int freq, int duty);
char parseMove(uint8_t data);

// Function prototypes
void initPWM(void);
void forward(void);
void left(void);
void right(void);
void stop(void);

#endif
