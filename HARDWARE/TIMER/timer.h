#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "common.h"
#include "led.h"
#include "adc.h"
#include "dac.h"
#include "Kalman.h"


extern int ADCValueInner;
extern int ADCValueOuter;
extern int ADCValueInner1;

extern int PInner, IntegrateInner;
extern int PInner1, IntegrateInner1;
extern int POuter, IntegrateOuter;

extern int OutputInner;
extern int OutputOuter;



extern unsigned char key_choice;

void Timer4_Init(void); 
void Timer2_Init(void);
 
#endif
