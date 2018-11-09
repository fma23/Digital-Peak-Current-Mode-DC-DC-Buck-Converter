#ifndef __Compensator_CLA__
#define __Compensator_CLA__



#define VoutSlewRate     24
// BUCK_ADCREF= 1222;  //for Vout=2V, ADC Resolution=1/4095; gain ratio=0.49; Vref=1/ADCresolution*Vout*0.49/3.3=1216;
#define BUCK_ADCREF     2432 //for Vout=4V, ADC Resolution=1/4095; gain ratio=0.49; Vref=1/ADCresolution*Vout*0.49/3.3=2432;

/* takes values: 1,2,3:
 when 1 implements 2P2Z
 when 2: implement PID using bilinear transformation
 when 3: implement PID using backward method */
#define  COMPENSATOR     3


//2P2Z Constants
#define a1_Coef              0.8285976581
#define a2_Coef              0.1714023419
#define b0_Coef              4.1703226660
#define b1_Coef              -5.9120992707
#define b2_Coef              1.9495912223



//#define a1_Coef              1.661612595
//#define a2_Coef              -0.661612595
//#define b0_Coef              2.810676213
//#define b1_Coef              0.252984346
//#define b2_Coef              -2.557691867



//PID Constants
//PID Constants for BUCK 4.0V, 2A
#define KP        6.2 //7.8// 5.0
#define KI        50000//50000 //15.0
#define KD        0.000005//0.000005 //0.1
#define T_Sample  0.000005


//current protection detect goes high at: 2.93*Ipn=2.93*40=117.2A; sensor gain is=(0.8/40)*1.5=0.03
//Thus: maximum voltage that can be seen from the sensor is: (0.03*117.2)/1.5 =2.34V


//Rsense=0.43, thus for 3.3V; max current sense is: 3.3/0.43=7.67A; DAC value corresponding to Max is: 2500*0.25=625; and V analog=(625/1023)*3.3=2.016V
//since 3.3V represents 7.67A, then 2.016 represents: (7.67*2.016)/3.3=4.68A

#define Max             2500
#define Min             0


//#define PIDOut_MAX      2500 //1944 // for 4V output: V(ADC)=4*0.49=1.96V, 3.3V represents: 1023/0.25=4092; thus 1.96 represents: 1.96*4092/3.3=2430; we take 80% out of it=1944
//#define PIDOut_MIN      0


#define PIDOut_MAX     3000
#define PIDOut_MIN     500



#define CNTL2P2Z_CLA(v, d)                                                                                                          \
/* Compute the error */                                                                                                             \
/*v.Errn=(v.Ref*0.000000059604644775390625)-v.Fdb;  */                                                                              \
v.Errn=v.Ref-v.Fdb;                                                                                                                 \
/* PreSat = e(n-2)*B2 + e(n-1)*B1 + e(n)*B0 + u(n-2)*A2 + u(n-1)*A1  - Ref converted to float from Q24 */                           \
v.OutPreSat = d.b2*v.Errn2 + d.b1*v.Errn1 + d.b0*v.Errn + d.a2*v.Out2 + d.a1*v.Out1;                                                \
/* store history of error*/                                                                                                         \
v.Errn2 = v.Errn1;                                                                                                                  \
v.Errn1 = v.Errn;                                                                                                                   \
v.Out=v.OutPreSat;                                                                                                                  \
/*  Saturate the output, use intrinsic for the CLA compiler */                                                                      \
v.Out=__mminf32(v.Out,d.OutMax);                                                                                                    \
v.Out=__mmaxf32(v.Out,d.OutMin);                                                                                                    \
/* store the history of outputs */                                                                                                  \
v.Out2 = v.Out1;                                                                                                                    \
v.Out1 = v.Out;


#if (COMPENSATOR ==3)

typedef volatile struct PID_D {
    float CurrInput;
    float PastInput;
    float Error_K0;
    float Error_K1;
    float Error_K2;

    float Integeral_Term;
    float Kp_Term;
    float Integral_Sum;
    float D_Input;
    float PIDOuput;
    float PIDOuput_K1;
    float Delta;
    float pidmax;
    float pidmin;
} PID;

#endif


#if(COMPENSATOR==1||COMPENSATOR==2)

typedef struct {
   float  b0;
   float  b1;
   float  b2;
   float  a1;
   float  a2;
   float  OutMax;        // Parameter: Maximum output
   float  OutMin;        // Parameter: Minimum output
}CNTL2P2Z_Coef;


typedef struct {
    float  Ref;           // Input: Reference input
    float  Fdb;           // Input: Feedback input
    float  Errn;          // Variable: Error
    float  Errn1;         // Parameter: Proportional gain
    float  Errn2;         // Variable: Proportional output
    float  Out;           // Variable: Integral output
    float  Out1;          // Variable: Derivative output
    float  Out2;          // Variable: Derivative output
    float  OutPreSat;     // Variable: Pre-saturated output
}CNTL2P2Z_CLA;

#endif






#endif

