//###########################################################################
//
// TITLE:   divide_shared_data.c
//
// Description: Declare shared memory variables and assign them to specific 
// CLA-accessible memory locations
//
//! \addtogroup f2806x_example_cla_list
//! \b Memory \b Allocation \n
//!  - CLA1 to CPU Message RAM
//!    - Res - Result of the division operation
//!  - CPU to CLA1 Message RAM
//!    - Num - Numerator of input
//!    - Den - Denominator of input
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

//
// Included Files
//
#include "DSP28x_Project.h"
#include <stdbool.h>


//
// Include the test header file whose name is based on the test name
// which is defined by the macro TEST on the command line
//
#include "buck_shared.h"


//
// Globals
//

////PID Variables


#pragma DATA_SECTION(temp2, "Cla1ToCpuMsgRAM");
float temp2;


#if (COMPENSATOR ==3)

#pragma DATA_SECTION(PID2, "Cla1ToCpuMsgRAM")
PID PID2;

#pragma DATA_SECTION(PID2_Out, "Cla1ToCpuMsgRAM")
float PID2_Out;


#endif

#if(COMPENSATOR==2||COMPENSATOR==3)

#pragma DATA_SECTION(KP_Coeff, "Cla1ToCpuMsgRAM")
#pragma DATA_SECTION(KI_Coeff, "Cla1ToCpuMsgRAM")
#pragma DATA_SECTION(KD_Coeff, "Cla1ToCpuMsgRAM")
float   KP_Coeff;
float   KI_Coeff;
float   KD_Coeff;

#endif


#if(COMPENSATOR==1||COMPENSATOR==2)

#pragma DATA_SECTION(Cmpns2_Coef, "Cla1ToCpuMsgRAM")
CNTL2P2Z_Coef Cmpns2_Coef;

#pragma DATA_SECTION(Cmpns2, "Cla1ToCpuMsgRAM")
CNTL2P2Z_CLA Cmpns2;

#endif


#pragma DATA_SECTION(BuckControl_Ref,"Cla1ToCpuMsgRAM");
float BuckControl_Ref;


//#pragma DATA_SECTION(VoutSlewRate,"Cla1ToCpuMsgRAM");
//float VoutSlewRate;
//
//#pragma DATA_SECTION(BUCK_ADCREF,"Cla1ToCpuMsgRAM");
//float BUCK_ADCREF;


//
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
// End of File
//

