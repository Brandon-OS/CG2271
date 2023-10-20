#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include "MKL25Z4.h"

int freq_calc(int freq);
int duty_cycle_calc(int freq, int duty);
char parseMove(uint8_t data);

// Function prototypes
void initMotor(void);
void rightMotorMove(int dir, float val);
void leftMotorMove(int dir, float val);	
void forward(void);
void backward(void);
void left(void);
void right(void);
void stop(void);
void rightForward(void);
void rightBackwark(void);
void leftForward(void);
void leftBackward(void);

#endif
