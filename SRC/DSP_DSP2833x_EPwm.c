// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:19 $
//###########################################################################
//
// FILE:   DSP2833x_EPwm.c
//
// TITLE:  DSP2833x ePWM Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define  mDeadTime     0.2e-6F
#define  mPwmFreq      150e6F
#define  mSwitchFreq   85e3F
//---------------------------------------------------------------------------
// InitEPwm: 
//---------------------------------------------------------------------------
// This function initializes the ePWM(s) to a known state.
//
void InitEPwm(void)
{
	unsigned int DeadTime, EPwmPRD;
	DeadTime = mDeadTime * mPwmFreq;
	EPwmPRD = ( unsigned int ) ( 0.5F * mPwmFreq / mSwitchFreq );
	asm(" EALLOW");
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	EPwm1Regs.TBPRD = EPwmPRD;
	EPwm1Regs.TBCTR = 0;
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;
	EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;
	EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1; //TBCLK=SYSCLKOUT
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW; // period register is loaded from its shadow register
	EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;


	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW; // 阴影寄存器，使能
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // Load on either CTR = Zero or CTR = PRD
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD; // Load on either CTR = Zero or CTR = PRD

	EPwm1Regs.CMPA.half.CMPA = 0.1*EPwmPRD;
	EPwm1Regs.CMPB = 0;
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
	//	EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;
	//	EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;
	EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm1Regs.DBFED = DeadTime;
	EPwm1Regs.DBRED = DeadTime;

	EPwm1Regs.ETSEL.bit.INTEN = 1;
	EPwm1Regs.ETSEL.bit.INTSEL = 1;

	EPwm1Regs.ETPS.bit.INTPRD = 1;
	//EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
	//EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;
	//EPwm1Regs.ETSEL.bit.SOCAEN = 1;

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	asm(" EDIS");
	/*
	unsigned int DeadTime, EPwmPRD;
	DeadTime = mDeadTime * mPwmFreq;
	EPwmPRD = ( unsigned int ) ( 0.5F * mPwmFreq / mSwitchFreq );
	asm(" EALLOW");
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	// -------------------------------------------------------------------------------------------------
	// PWM1
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
	EPwm1Regs.TBPRD = EPwmPRD;
	EPwm1Regs.TBCTR = 0;
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;
	EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;
	EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1; //TBCLK=SYSCLKOUT
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW; // period register is loaded from its shadow register
	EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW; // 阴影寄存器，使能
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // Load on either CTR = Zero or CTR = PRD
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD; // Load on either CTR = Zero or CTR = PRD

	//EPwm1Regs.CMPA.half.CMPA = 0.1*gEPwmPRD;
	EPwm1Regs.CMPB = 0;
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	//	EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;
	//	EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;
	EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm1Regs.DBFED = DeadTime;
	EPwm1Regs.DBRED = DeadTime;

	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	// -------------------------------------------------------------------------------------------------
	// PWM2
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
	EPwm2Regs.TBPRD = EPwmPRD;
	EPwm2Regs.TBCTR = 0;
	EPwm2Regs.TBPHS.half.TBPHS = 0x0000;
	EPwm2Regs.TBCTL.bit.FREE_SOFT = 3;
	EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1; //TBCLK=SYSTEMOUT
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; //
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	//EPwm2Regs.CMPA.half.CMPA = 0.2*gEPwmPRD;
	EPwm2Regs.CMPB = 0;
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	//	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;
	//	EPwm2Regs.AQCTLB.bit.CAD = AQ_SET;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBFED = DeadTime;
	EPwm2Regs.DBRED = DeadTime;

	//EPwm2Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
	EPwm2Regs.ETPS.bit.SOCAPRD = ET_1ST;
	EPwm2Regs.ETSEL.bit.SOCAEN = 1;
	// -------------------------------------------------------------------------------------------------
	// PWM3
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
	EPwm3Regs.TBPRD = EPwmPRD;
	EPwm3Regs.TBCTR = 0;
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000;
	EPwm3Regs.TBCTL.bit.FREE_SOFT = 3;
	EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1; //TBCLK=SYSTEMOUT
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;

	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	//EPwm3Regs.CMPA.half.CMPA = 0.3*gEPwmPRD;
	EPwm3Regs.CMPB = 0;
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	//	EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;
	//	EPwm3Regs.AQCTLB.bit.CAD = AQ_SET;
	EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm3Regs.DBFED = DeadTime;
	EPwm3Regs.DBRED = DeadTime;
	// -------------------------------------------------------------------------------------------------
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	asm(" EDIS");
	*/
}

//===========================================================================
// End of file.
//===========================================================================
