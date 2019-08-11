#include "DSP2833x_Device.h"    
#include "DSP2833x_Examples.h"

void InitXInterrupt(void)
{
	asm(" EALLOW");
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;
	GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 24; //GPIO24 XINT1
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;       //XINT1
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;     // falling edge
	asm(" EDIS");
}



