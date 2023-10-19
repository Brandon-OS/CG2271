#ifndef AUDIO_H
#define AUDIO_H
#include "MKL25Z4.h"

#define C_Note 262
#define D_Note 294
#define E_Note 330
#define F_Note 349
#define G_Note 392
#define A_Note 440
#define B_Note 494

// Function prototypes
void initAudio(void);
void playAudio(int*, int);

#endif
