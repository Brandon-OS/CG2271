#ifndef _COMMON_
#define _COMMON_

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "MKL25Z4.h"

extern osEventFlagsId_t 
	flagRunningSound, flagEndingSound, 
	flagFinish,
	flagRunning, flagStation
;

#endif