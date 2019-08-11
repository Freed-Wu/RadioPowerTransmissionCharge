
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

//unsigned int gAD_Record0[1000];
//#pragma DATA_SECTION( gAD_Record0 , "ZONE7SARAM" );

//volatile unsigned int gDriveCnt = 0;

interrupt void IntelAD_ISR( void )
{
	unsigned int AdData[16];

	// -------------------------------------------------------------------------------------------------

	// GpioDataRegs.GPBSET.bit.GPIO57 = 1;
	// -------------------------------------------------------------------------------------------------
	AdData[ 0] = AdcRegs.ADCRESULT0  >> 4;
	AdData[ 1] = AdcRegs.ADCRESULT1  >> 4;
	AdData[ 2] = AdcRegs.ADCRESULT2  >> 4;
	AdData[ 3] = AdcRegs.ADCRESULT3  >> 4;
	AdData[ 4] = AdcRegs.ADCRESULT4  >> 4;
	AdData[ 5] = AdcRegs.ADCRESULT5  >> 4;
	AdData[ 6] = AdcRegs.ADCRESULT6  >> 4;
	AdData[ 7] = AdcRegs.ADCRESULT7  >> 4;
	AdData[ 8] = AdcRegs.ADCRESULT8  >> 4;
	AdData[ 9] = AdcRegs.ADCRESULT9  >> 4;
	AdData[10] = AdcRegs.ADCRESULT10 >> 4;
	AdData[11] = AdcRegs.ADCRESULT11 >> 4;
	AdData[12] = AdcRegs.ADCRESULT12 >> 4;
	AdData[13] = AdcRegs.ADCRESULT13 >> 4;
	AdData[14] = AdcRegs.ADCRESULT14 >> 4;
	AdData[15] = AdcRegs.ADCRESULT15 >> 4;

	// -------------------------------------------------------------------------------------------------
	//gAD_B_Channel_01[gAD_Cnt] = AdData[ 0];
//	gAD_B_Channel_02[gAD_Cnt] = AdData[ 1];
//	gAD_B_Channel_03[gAD_Cnt] = AdData[ 2];
//	gAD_B_Channel_04[gAD_Cnt] = AdData[ 3];
//	gAD_B_Channel_05[gAD_Cnt] = AdData[ 4];
//	gAD_B_Channel_06[gAD_Cnt] = AdData[ 5];
//	gAD_B_Channel_07[gAD_Cnt] = AdData[ 6];
//	gAD_B_Channel_08[gAD_Cnt] = AdData[ 7];
//	gAD_B_Channel_09[gAD_Cnt] = AdData[ 8];
//	gAD_B_Channel_10[gAD_Cnt] = AdData[ 9];
//	gAD_B_Channel_11[gAD_Cnt] = AdData[10];
//	gAD_B_Channel_12[gAD_Cnt] = AdData[11];
//	gAD_B_Channel_13[gAD_Cnt] = AdData[12];
//	gAD_B_Channel_14[gAD_Cnt] = AdData[13];
//	gAD_B_Channel_15[gAD_Cnt] = AdData[14];
//	gAD_B_Channel_16[gAD_Cnt] = AdData[15];
//	gAD_Cnt++;
//	if( gAD_Cnt >= mRecordNum )
//	{
//		gAD_Cnt = 0;
//	}

	// -------------------------------------------------------------------------------------------------

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}

