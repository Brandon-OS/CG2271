#ifndef SERIAL_H
#define SERIAL_H
#include "MKL25Z4.h"

#define BAUD_RATE 9600
// Function prototypes
void initUART2(uint32_t);
uint8_t serial(void);

#endif
