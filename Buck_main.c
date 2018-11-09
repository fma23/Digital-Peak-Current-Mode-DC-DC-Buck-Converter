
// FILE:   Boost_main.c
// AUTHOR: Farid Mabrouk


// Included Files
//
#include "DSP28x_Project.h"     // DSP28x Headerfile BoostControl_Ref
#include "F2806x_Cla_defines.h"  
#include "stdio.h"
#include "F2806x_Epwm.h"
#include <stdbool.h>
#include "F2806x_CpuTimers.h"

#include "buck_shared.h"
#include "main.h"




struct CPUTIMER_VARS *Timer;

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;


// These are defined by the linker file and used to copy the CLA code from its load address to its run address in CLA program memory
extern Uint16 Cla1funcsLoadStart;
extern Uint16 Cla1funcsLoadEnd;
extern Uint16 Cla1funcsLoadSize;
extern Uint16 Cla1funcsRunStart;
extern Uint16 Cla1Prog_Start;


// These are defined by the linker file and used to copy the CLA math tables from its load address to its run address into one of the CLA data RAMs
extern Uint16 Cla1mathTablesLoadStart;
extern Uint16 Cla1mathTablesLoadEnd;
extern Uint16 Cla1mathTablesLoadSize;
extern Uint16 Cla1mathTablesRunStart;

//unsigned int Vin[300];
//unsigned int HsinkTemp[300];
//unsigned int Current_Out[300];

unsigned int ArrayA;
unsigned int ArrayB;
unsigned int ArrayC;


float PID1outArray[2000];



void main(void)
{

   // Step 1: Setup the system clock Disable the watchdog timer, initialize the system clock,PLL and configure the peripheral clock.
   InitSysCtrl();

   // Copy time critical code and Flash setup code to RAM The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart symbols are created by the linker. Refer to the linker files.
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

    // Call Flash Initialization to setup flash waitstates This function must reside in RAM
    InitFlash();       // Call the flash wrapper init function


    //Initialize PIE control Intialize PIE control, disable all interrupts and then copy over the PIE Vector table from BootROM to RAM
    DINT;                                      //Disable CPU Interrupts
    InitPieCtrl();
    IER = 0x00000000;
    IFR = 0x00000000;
    InitPieVectTable();


    // Assign user defined ISR to the PIE vector table
    EALLOW;
    PieVectTable.TINT1      = &cpu_timer1_isr;
    PieVectTable.CLA1_INT1  = &cla1_task1_isr;
    PieVectTable.CLA1_INT4  = &cla1_task4_isr;
    EDIS;

    // Enable INT11.1 INT 11.2. 11.3 in the PIE (CLA Task1,2,3,4)
    PieCtrlRegs.PIEIER11.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER11.bit.INTx4 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  //Enable Timer 1

    /* Enable ADCINT1 in PIE */
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  /* Enable INT 1.1 in the PIE */


    //Enable CLA interrupts
    IER |= M_INT11;
    IER |= M_INT13;                           // Enable CPU int1 which is connected to CPU-Timer1,
    IER |= M_INT2;

    EINT;/* Enable Global interrupt INTM */
    ERTM;/* Enable Global realtime interrupt DBGM */


    init_cla();

    //Configure GPIOs
    Gpio_select();

    //Configure ADCs
    ADCs_Config();

    //Configure PWM
    PWMs_Config();

    //Configure Comparators
    Comparator_Config();


    GpioDataRegs.GPASET.bit.GPIO10=1;   //active load for buckdevboard

    InitCpuTimers();
    //configure timer1;Frequency in MHz, period in Useconds
    ConfigCpuTimer(&CpuTimer1,90,1000000);

    // To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
    // of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the
    // below settings must also be updated.
    CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0


     while(1)
     {
     }

#if HARDWARE
 {
     __asm(" ESTOP0");
 }
 #endif

 }

__interrupt
void cpu_timer1_isr(void)
{

  GpioDataRegs.GPBTOGGLE.bit.GPIO39=1;

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


__interrupt void
cla1_task1_isr(void)
{
    static int b=0;

    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    PieCtrlRegs.PIEACK.bit.ACK11 = 1;

    GpioDataRegs.GPATOGGLE.bit.GPIO22=1;

    if(b<2000){
        PID1outArray[b]=temp2; //PID2_Out;
        b++;
        }

}

__interrupt void
cla1_task4_isr(void)
{
    GpioDataRegs.GPBTOGGLE.bit.GPIO32=1;
    PieCtrlRegs.PIEACK.bit.ACK11 = 1;
}
/*********************************************************************************************
// This function will copy the specified memory contents from
// one location to another.
//
//  Uint16 *SourceAddr        Pointer to the first word to be moved
//                          SourceAddr < SourceEndAddr
//  Uint16* SourceEndAddr     Pointer to the last word to be moved
//  Uint16* DestAddr          Pointer to the first destination word
//
// No checks are made for invalid memory locations or that the
// end address is > then the first start address.
*********************************************************************************************/
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

//
// End of File
//

void init_cla(void)
{

    //
    // This code assumes the CLA clock is already enabled in
    // the call to InitSysCtrl();
    //
    // EALLOW: is needed to write to EALLOW protected registers
    // EDIS: is needed to disable write to EALLOW protected registers
    //
    // Initalize the interrupt vectors for Task 7 (CLA FIR Filter)
    // and for Task 8 (FIR filter initalization)
    //
    // The symbols used in this calculation are defined in the CLA
    // assembly code and in the CLAShared.h header file

    EALLOW;
    Cla1Regs.MVECT1 = (Uint16)((Uint32)&Cla1Task1 -(Uint32)&Cla1Prog_Start);
    Cla1Regs.MVECT4 = (Uint16)((Uint32)&Cla1Task4 -(Uint32)&Cla1Prog_Start);
    EDIS;

    // Task 7 has the option to be started by either EPWM7_INT or ADCINT7
    // In this case we will allow ADCINT7 to start CLA Task 7
    //
    EALLOW;
    Cla1Regs.MPISRCSEL1.bit.PERINT1SEL = CLA_INT1_ADCINT1;   // ADCINT1 starts Task 1
    EDIS;

    // Copy the CLA program code from its load address to the CLA program
    // memory. Once done, assign the program memory to the CLA
    //
    // Make sure there are at least two SYSCLKOUT cycles between assigning
    // the memory to the CLA and when an interrupt comes in
    //
    memcpy(&Cla1funcsRunStart, &Cla1funcsLoadStart, (Uint32)&Cla1funcsLoadSize);
    EALLOW;
    Cla1Regs.MMEMCFG.bit.PROGE = 1;
    EDIS;

    //
    // Enable the IACK instruction to start a task
    // Enable the CLA interrupt 8 and interrupt 7
    //
    EALLOW;
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all =0x00FF;
    EDIS;

    // Force CLA task 4 using the IACK instruction
    Cla1ForceTask4andWait();
}










