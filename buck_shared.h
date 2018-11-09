//###########################################################################
//
// TITLE:   divide_shared.h
//
// Description: Declare shared variables
//
//###########################################################################
// $TI Release: F2806x Support Library v2.04.00.00 $
// $Release Date: Tue Jun 26 03:13:59 CDT 2018 $
// $Copyright:
// Copyright (C) 2009-2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#ifndef BUCK_SHARED_H_
#define BUCK_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

#include "Compensator_CLA.h"
//
// Task 1 (C) Variables
//

#if (COMPENSATOR ==3)

extern PID PID1;
extern PID PID2;
extern PID PID3;

extern float PID1_Out;
extern float PID2_Out;
extern float PID3_Out;

#endif

#if(COMPENSATOR==2||COMPENSATOR==3)
//PID Variables
extern float   KP_Coeff;
extern float   KI_Coeff;
extern float   KD_Coeff;
#endif


#if(COMPENSATOR==1||COMPENSATOR==2)

extern CNTL2P2Z_Coef Cmpns1_Coef;
extern CNTL2P2Z_Coef Cmpns2_Coef;
extern CNTL2P2Z_Coef Cmpns3_Coef;

extern CNTL2P2Z_CLA Cmpns1;
extern CNTL2P2Z_CLA Cmpns2;
extern CNTL2P2Z_CLA Cmpns3;

#endif

extern float temp1;
extern float temp2;
extern float temp3;

extern float BuckControl_Ref;

//extern float VoutSlewRate;
//extern float BUCK_ADCREF;


// Task 2 (C) Variables
//

//
// Task 3 (C) Variables
//

//
// Task 4 (C) Variables
//

//
// Task 5 (C) Variables
//

//
// Task 6 (C) Variables
//

//
// Task 7 (C) Variables
//

//
// Task 8 (C) Variables
//

//
// Common (C) Variables
//

//
// CLA C Tasks
//
__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

//
// C Function Prototypes
//
void test_run();
void test_report();

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif /*BUCK_SHARED_H_*/

//
// End of File
// 

