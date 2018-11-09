#ifndef BuckHardware_H
#define BuckHardware_H


#include "F2806x_Epwm.h"



#define slopeval            51      /* Required slope compensation: steps=(T-1020ns)/TsysClk; TsysClk=11.11ns; blanking+margin=370ns+650ns;   slopeval=Vpp/steps     */
#define PERIOD              450    // period for PWM is set at 200KHz; system clock is at 90MHz
#define PHASE1              150
#define PHASE2              300

#define DutyCycle_PWMA      360 //80%; 450 counter corresponds to 100%
#define DutyCycle_PWMB      180 //40%

#define ADCTRIG_SOFT        0
#define ADCTRIG_CPU_TINT0   1
#define ADCTRIG_CPU_TINT1   2
#define ADCTRIG_CPU_TINT2   3
#define ADCTRIG_XINT2       4
#define ADCTRIG_EPWM1_SOCA  5
#define ADCTRIG_EPWM1_SOCB  6
#define ADCTRIG_EPWM2_SOCA  7
#define ADCTRIG_EPWM2_SOCB  8
#define ADCTRIG_EPWM3_SOCA  9
#define ADCTRIG_EPWM3_SOCB  10
#define ADCTRIG_EPWM4_SOCA  11
#define ADCTRIG_EPWM4_SOCB  12
#define ADCTRIG_EPWM5_SOCA  13
#define ADCTRIG_EPWM5_SOCB  14
#define ADCTRIG_EPWM6_SOCA  15
#define ADCTRIG_EPWM6_SOCB  16
#define ADCTRIG_EPWM7_SOCA  17
#define ADCTRIG_EPWM7_SOCB  18




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DECLARATIONS - GENERAL
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Used to indirectly access all EPWM modules
volatile struct EPWM_REGS *ePWM[] =
   { &EPwm1Regs,                                              //intentional: (ePWM[0] not used)
     &EPwm1Regs,
     &EPwm2Regs,
     &EPwm3Regs,
     &EPwm4Regs,
     &EPwm5Regs,
     &EPwm6Regs,
     &EPwm7Regs,
     &EPwm8Regs
   };


// Used for ADC Configuration
int     ChSel[16] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int     TrigSel[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int     ACQPS[16] =   {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};


// Used for DAC
int16 DACval;
int16 DACsrc;

// Defines



#define CLA_PROG_ENABLE      0x0001
#define CLARAM0_ENABLE       0x0010
#define CLARAM1_ENABLE       0x0020
#define CLARAM2_ENABLE       0x0040
#define CLA_RAM0CPUE         0x0100
#define CLA_RAM1CPUE         0x0200
#define CLA_RAM2CPUE         0x0400


//
// Constants
//
#define OSCCLK       (10000000UL)            // < Internal OSC frequency
#define SYSCLK       (80000000UL)            // < System Clock frequency

#define PWM1_INT_ENABLE  1
#define PWM1_TIMER_TBPRD   0x1FFF       // Configure the period for the timer

//
// < Low speed peripheral clock dividers
//
#define PER_CLK_DIV  (4)

#define PER_CLK      (SYSCLK/PER_CLK_DIV)    // < Low speed peripheral clock
#define DIV_SEL      (2)                     // < Clock divider selection
#define CLK_DIV      ((SYSCLK * DIV_SEL)/OSCCLK) // < Clock divider


#endif


