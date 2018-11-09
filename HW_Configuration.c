#include "DSP28x_Project.h"     // DSP28x Headerfile
#include "F2806x_Cla_defines.h"
#include "BuckHardware.h"



void Comparator_Config(void)
{

    /* Configure Comparator 2: link cmp2 to pwm4*/
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1;
    Comp2Regs.COMPCTL.bit.COMPDACEN = 1;             // Power up Comparator locally
    Comp2Regs.COMPCTL.bit.COMPSOURCE = 0;            // Connect the inverting input to internal DAC
    Comp2Regs.DACCTL.bit.DACSOURCE = 1;              // 0 - DACVAL; 1 - Internal ramp for slope compensation
    Comp2Regs.COMPCTL.bit.QUALSEL = 5;               // Comparator output must be active for 4 consecutive clocks before resetting the RAMP
    Comp2Regs.DACCTL.bit.RAMPSOURCE = 3;             // 0 - PMW1; 1 - PWM2,...so on link cmp2 to pwm4
    Comp2Regs.RAMPDECVAL_SHDW = slopeval;
    EPwm2Regs.HRPCTL.bit.PWMSYNCSEL = 1;             // PWM SYNC generated at CTR = ZRO for synchronizing internal ramp
    Comp2Regs.COMPCTL.bit.CMPINV = 0;                // Comparator Output passed
    Comp2Regs.DACVAL.bit.DACVAL = 1280>>5;           // Set DAC output - Input is Q15 - Convert to Q10
    EDIS;
}


void ADCs_Config(void)
{
    InitAdc();
    InitAdcAio();

    EALLOW;
    AdcRegs.INTSEL1N2.bit.INT1E = 1;       /* Enabled ADCINT1 */
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0;    /* Disable ADCINT1 Continuous mode */
    AdcRegs.INTSEL1N2.bit.INT1SEL = 0;     /* setup EOC0 to trigger ADCINT1 to fire */
    AdcRegs.ADCSOC0CTL.bit.CHSEL = 0x3;    /* Set SOC0 channel select to ADCINB0 */
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL = ADCTRIG_EPWM1_SOCB; //6;    /* Set SOC0 start trigger on EPWM1B, due to round-robin SOC0 converts first then SOC1 */
    AdcRegs.ADCSOC0CTL.bit.ACQPS = 6;      /* Set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1) */
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 0;   /* Configure early interrupts */
    EDIS;
}


void PWMs_Config(void)
{
  /* Configure PWMs */

   EALLOW;
   /* Assumes ePWM1 clock is already enabled in InitSysCtrl(); */
   EPwm1Regs.ETSEL.bit.SOCBEN = 1;        /* Enable SOC on B group */
   EPwm1Regs.ETSEL.bit.SOCBSEL = 6;       /* Select SOC from from CPMB on upcount */
   EPwm1Regs.ETPS.bit.SOCBPRD = 1;        /* Generate pulse on 1st event */

   /* Set period / duty / count mode */
   EPwm1Regs.TBPHS.half.TBPHS = 0;             /* Set Phase register to zero */
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;     /* Phase loading disabled */
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;    /* Clock ratio to SYSCLKOUT */
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;

   EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;          /* Set PWM1A on Zero */
   EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;        /* Clear PWM1A on match on count A up */
   EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;          /* Set PWM1B on Zero */
   EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;        /* Clear PWM1B on match on count B up */

   EPwm1Regs.CMPB = DutyCycle_PWMB;                       /* Set compare B value */
   EPwm1Regs.CMPA.half.CMPA = DutyCycle_PWMA;             /* Set compare A value */
   EPwm1Regs.TBPRD = PERIOD;                   /* Set period for ePWM1 */
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;  /* Count up and start */
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // sync "down-stream"
   EDIS;


   /* Assumes ePWM1 clock is already enabled in InitSysCtrl(); */
    EALLOW;

    /*PWM4: Set period / duty / count mode */
    EPwm4Regs.TBPHS.half.TBPHS = 0;             /* Set Phase register to zero */
    EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;     /* Phase loading disabled */
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;    /* Clock ratio to SYSCLKOUT */
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;          /* Set PWM4A on Zero */
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;        /* Clear PWM4A on match on count A up */
    EPwm4Regs.AQCTLB.bit.ZRO = AQ_SET;          /* Set PWM1B on Zero */
    EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;        /* Clear PWM1B on match on count B up */

   // EPwm4Regs.CMPB = 324;                       /* Set compare B value */
    EPwm4Regs.CMPA.half.CMPA =360;             /* Set compare A value */
    EPwm4Regs.TBPRD = PERIOD;                   /* Set period for ePWM1 */
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;  /* Count up and start */
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // sync "down-stream"
    EDIS;


    /* Configure trip zone based on Comparator 2 output and configure blanking */
    EALLOW;
    EPwm4Regs.TZSEL.bit.DCAEVT2 = 1;                      /* Digital compare, output A, cycle by cycle */
    EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;                /* EPWM4A will go low */
    EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_HI;                /* EPWM4B goes HI */

    EPwm4Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP2OUT;    // DCAH = Comparator 2 output
    EPwm4Regs.TZDCSEL.bit.DCAEVT2 = TZ_DCAH_HI;           /** DCAEVT2 =  DCAL High (will become active as Comparator output goes High) */
                                                          /* DCAEVT2 = DCAH high (will become EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP2OUT; DCAH = Comparator 2 output*/
    EPwm4Regs.DCACTL.bit.EVT2SRCSEL = DC_EVT2;            /* DCAEVT2 = DCAEVT2 (not filtered) */
    EPwm4Regs.DCACTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;   /* Take async path */
    EPwm4Regs.DCFCTL.bit.PULSESEL = 0;                    /* Time-base counter equal to period (TBCTR =TBPRD) */
    EPwm4Regs.DCFCTL.bit.BLANKINV = 0;                    /* Blanking window inverted */
    EPwm4Regs.DCFCTL.bit.BLANKE = 1;                      /* Blanking Window Enable */
    EPwm4Regs.DCFCTL.bit.SRCSEL = 0;                      /* Source Is DCAEVT1 Signal */
    EPwm4Regs.DCFWINDOW = 0; //34;                        /*length blanking window: this corresponds to: 34*11.1ns=377.4ns*/
    EPwm4Regs.DCFOFFSET = 5;                              /* 5*11.1=55.5ns blanking window offset & then blanking window begins */
    EDIS;

    // Active high complementary PWMs - Set up the deadband
    EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm4Regs.DBRED = 15;
    EPwm4Regs.DBFED = 15;

}


// Gpio_select -
void Gpio_select(void)
{
    EALLOW;

    /************************************ PWMs  *****************************************/
    //  PWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;     // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv
    //  GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;      // 1=OUTput,  0=INput
     GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;    // uncomment if --> Set Low initially
    //  GpioDataRegs.GPASET.bit.GPIO0 = 1;      // uncomment if --> Set High initially
    //------------------------------------------------------------------------------------
    //  EPWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;     // 0=GPIO,  1=EPWM1B,  2=Resv,  3=COMP1OUT
    //  GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;      // 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;    // uncomment if --> Set Low initially
    //  GpioDataRegs.GPASET.bit.GPIO1 = 1;      // uncomment if --> Set High initially
    //------------------------------------------------------------------------------------
    //PWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;     // 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
    //  GpioCtrlRegs.GPADIR.bit.GPIO2 = 0;      // 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;    // uncomment if --> Set Low initially
    //  GpioDataRegs.GPASET.bit.GPIO2 = 1;      // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    // PWM4B
     GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;     // 0=GPIO,  1=EPWM2B,  2=SPISOMI-A,  3=COMP2OUT
     //  GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;      // 1=OUTput,  0=INput
     GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;    // uncomment if --> Set Low initially
     //  GpioDataRegs.GPASET.bit.GPIO3 = 1;      // uncomment if --> Set High initially


    /**************************************************************************************/
    //GPIO FUNCION
    //Use these for BUCK Dev board
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;     // 1=OUTput,  0=INput
    //  GpioDataRegs.GPBCLEAR.bit.GPIO12 = 1;   // uncomment if --> Set Low initially
    GpioDataRegs.GPASET.bit.GPIO12 = 1;     // uncomment if --> Set High initially

    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;     // 1=OUTput,  0=INput
    //  GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;   // uncomment if --> Set Low initially
    GpioDataRegs.GPASET.bit.GPIO22 = 1;     // uncomment if --> Set High initially

    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;     // 1=OUTput,  0=INput
    //  GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;   // uncomment if --> Set Low initially
    GpioDataRegs.GPBSET.bit.GPIO32 = 1;     // uncomment if --> Set High initially

    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;     // 1=OUTput,  0=INput
    //  GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;   // uncomment if --> Set Low initially
    GpioDataRegs.GPBSET.bit.GPIO39 = 1;     // uncomment if --> Set High initially

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;    // 0=GPIO,  1=EPWM6A,  2=Resv,
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;     // 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;   // uncomment if --> Set Low initially
    // GpioDataRegs.GPASET.bit.GPIO10 = 1;     // uncomment if --> Set High initially


    /**************************************************************************************/
    // COMPARATOR IOs
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 =3;     // 0=GPIO,  1=EPWM2B,  2=SPISOMI-A,  3=COMP2OUT
    //GpioCtrlRegs.GPBDIR.bit.GPIO43 = 1;      // 1=OUTput,  0=INput
    GpioDataRegs.GPBCLEAR.bit.GPIO43 = 1;    // uncomment if --> Set Low initially
    //GpioDataRegs.GPASET.bit.GPIO3 = 1;      // uncomment if --> Set High initially

    EDIS;
}


