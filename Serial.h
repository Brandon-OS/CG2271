#ifndef SERIAL_H
#define SERIAL_H
#include "MKL25Z4.h"
#include <stdbool.h>
// Function prototypes
void initUART2(void);
void UART2_IRQHandler(void);

#endif
