// TI File $Revision: /main/2 $
// Checkin $Date: March 1, 2007   16:06:07 $
//###########################################################################
//
// FILE:	DSP2833x_Sci.c
//
// TITLE:	DSP2833x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitSci: 
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void InitSci(void)
{
	//*****************SCI C Initialization*****************

	asm(" EALLOW");

		ScicRegs.SCICTL1.bit.SWRESET = 0;
		ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;
		ScicRegs.SCICCR.bit.LOOPBKENA = 0;
		ScicRegs.SCICCR.bit.PARITY = 0;
		ScicRegs.SCICCR.bit.PARITYENA = 1;
		ScicRegs.SCICCR.bit.SCICHAR = 7;
		ScicRegs.SCICCR.bit.STOPBITS = 0;
	//	ScicRegs.SCICCR.all = 0x07;

		ScicRegs.SCICTL1.bit.RXENA = 1;
		ScicRegs.SCICTL1.bit.TXENA = 1;
		ScicRegs.SCICTL1.bit.SLEEP = 0;
		ScicRegs.SCICTL1.bit.TXWAKE = 1;
		ScicRegs.SCICTL1.bit.RXERRINTENA = 0;

	//	ScicRegs.SCIHBAUD = 0x03;//LSPCLK = 75Mhz,波特率9600时
	//	ScicRegs.SCILBAUD = 0xD0;

	//	ScicRegs.SCIHBAUD = 0x01;//LSPCLK = 75Mhz,波特率19200时
	//	ScicRegs.SCILBAUD = 0xE7;
	//	ScicRegs.SCIHBAUD = 0x00;//LSPCLK = 75Mhz,波特率12800时
	//	ScicRegs.SCILBAUD = 0x48;
//		ScicRegs.SCIHBAUD = 0x02;//LSPCLK = 75Mhz,波特率14400时
//		ScicRegs.SCILBAUD = 0x8A;
		ScicRegs.SCIHBAUD = 0x00;//LSPCLK = 75Mhz,波特率38400时
		ScicRegs.SCILBAUD = 0xF3;
//		ScicRegs.SCIHBAUD = 0x00;//LSPCLK = 75Mhz,波特率115200时
//		ScicRegs.SCILBAUD = 0x50;
//		ScicRegs.SCIHBAUD = 0x00;//LSPCLK = 75Mhz,波特率187500时
//		ScicRegs.SCILBAUD = 0x31;


		ScicRegs.SCICTL2.bit.RXBKINTENA = 0;
		ScicRegs.SCICTL2.bit.TXINTENA = 0;

		ScicRegs.SCIFFRX.bit.RXFFIENA = 0;
		ScicRegs.SCIFFRX.bit.RXFFIL = 15;
		ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
		ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
		ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;
		ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;

		ScicRegs.SCIFFTX.bit.SCIFFENA = 1;
		ScicRegs.SCIFFTX.bit.SCIRST = 0;
		ScicRegs.SCIFFTX.bit.SCIRST = 1;
		ScicRegs.SCIFFTX.bit.TXFFIENA = 0;
		ScicRegs.SCIFFTX.bit.TXFFIL = 0;
		ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
		ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 0;
		ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;

		ScicRegs.SCIFFCT.bit.CDC = 0;
		ScicRegs.SCIFFCT.bit.FFTXDLY = 0;

		ScicRegs.SCIPRI.bit.FREE = 1;
		ScicRegs.SCIPRI.bit.SOFT = 1;

		ScicRegs.SCICTL1.bit.SWRESET = 1;

	asm(" EDIS");
}	

//===========================================================================
// End of file.
//===========================================================================
