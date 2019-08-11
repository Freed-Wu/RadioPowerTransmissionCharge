
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


void InitWatchdog( void )
{
	EALLOW;
	SysCtrlRegs.WDCR = 0x00AA;
	EDIS;
}
