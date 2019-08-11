// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:25 $
//###########################################################################
//
// FILE:	DSP2833x_Gpio.c
//
// TITLE:	DSP2833x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 


#define     mGPAMUX1_VALUE     0x00555555
#define     mGPAMUX2_VALUE     0x00000000
#define     mGPBMUX1_VALUE     0x00000000
#define     mGPBMUX2_VALUE     0x00055000
#define     mGPCMUX1_VALUE     0x00000000
#define     mGPCMUX2_VALUE     0x00000000

void InitGpio(void)
{
	asm(" EALLOW");
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
	//GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;

	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
	asm(" EDIS");



	/*
	asm(" EALLOW");
	// ------------------------------------------------------------------------
	GpioCtrlRegs.GPAMUX1.all  = mGPAMUX1_VALUE;
	GpioCtrlRegs.GPAMUX2.all  = mGPAMUX2_VALUE;
	GpioCtrlRegs.GPBMUX1.all  = mGPBMUX1_VALUE;
	GpioCtrlRegs.GPBMUX2.all  = mGPBMUX2_VALUE;
	GpioCtrlRegs.GPCMUX1.all  = mGPCMUX1_VALUE;
	GpioCtrlRegs.GPCMUX2.all  = mGPCMUX2_VALUE;

	GpioCtrlRegs.GPACTRL.all  = 0x00000000;
	GpioCtrlRegs.GPBCTRL.all  = 0x00000000;
	GpioCtrlRegs.GPAQSEL1.all = 0x00000000;
	GpioCtrlRegs.GPAQSEL2.all = 0x00000000;
	GpioCtrlRegs.GPBQSEL1.all = 0x00000000;
	GpioCtrlRegs.GPBQSEL2.all = 0xF0000000;

	GpioCtrlRegs.GPADIR.all   = 0x00000000;
	GpioCtrlRegs.GPBDIR.all   = 0x00000000;
	GpioCtrlRegs.GPCDIR.all   = 0x00000000;

	GpioCtrlRegs.GPAPUD.all   = 0x00000000;
	GpioCtrlRegs.GPBPUD.all   = 0x00000000;
	GpioCtrlRegs.GPCPUD.all   = 0x00000000;


	// ------------------------------------------------------------------------
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;

	GpioCtrlRegs.GPADIR.bit.GPIO18 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;

	GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;
	// ----------------------------------------
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 3;
	// ------------------------------------------------------------------------
	asm(" EDIS");
	*/
}

//===========================================================================
// End of file.
//===========================================================================
