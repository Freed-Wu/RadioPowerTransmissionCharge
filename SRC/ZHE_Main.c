#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "matrix_key.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
// ADC start parameters
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
	#define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
	#define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
#endif
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0xf   // S/H width in ADC module periods                        = 16 ADC clocks
#define AVG        1000  // Average sample limit
#define ZOFFSET    0x00  // Average Zero offset
#define BUF_SIZE   40    // Sample buffer size
static char tempTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void num2char ( char* str, double number, int g, int l ) {
	int i;
	int temp = number / 1;
	double t2 = 0.0;

	for ( i = 1; i <= g; i++ ) {
		if ( temp == 0 )
			str[g - i] = tempTable[0];

		else
			str[g - i] = tempTable[temp % 10];

		temp = temp / 10;
	}

	* ( str + g ) = '.';
	temp = 0;
	t2 = number;

	for ( i = 1; i <= l; i++ ) {
		temp = t2 * 10;
		str[g + i] = tempTable[temp % 10];
		t2 = t2 * 10;
	}

	* ( str + g + l + 1 ) = '\0';
}
#define  mPI   3.1415927
extern interrupt void cpu_timer0_isr ( void );
/*extern interrupt void IntelAD_ISR ( void );*/
extern interrupt void User_EPWM1_INT_ISR ( void );
unsigned int turn = 0;
#define CHARGE_PERIOD 3
float gTs;
float gVm;
float gF;

float gVoltage;
unsigned int gCnt;

unsigned int samplesave0[10], samplesave1[10];
unsigned char a = 0;

volatile float gM;

//input:按键输入
//disp:屏幕显示
//gVm:显示的数字转换为输入电压的最大值
//mode:模式选择，启动后电压输入，按A切换电压、频率
//load:负载选择，启动后无负载，按B切换有无
char input = '\0';
char disp[12] = "";
int mode = 0;
int load = 0;
int dot = 0;
int num = 0;
volatile unsigned int samplevalue0, samplevalue1;
volatile float adc0 = 0, adc1 = 0;
int timer0count = 0;
int lcdflag = 0, lcdcount = 0, matrixflag = 0, adcindex = 0, adcflag = 0;

void main ( void ) {
	InitSysCtrl( );
	InitGpio( );
	asm ( " EALLOW" );
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
	GpioDataRegs.GPADAT.bit.GPIO3 = 1;
	GpioDataRegs.GPADAT.bit.GPIO5 = 1;
	GpioDataRegs.GPADAT.bit.GPIO7 = 1;
	GpioDataRegs.GPADAT.bit.GPIO9 = 1;
	asm ( " EDIS" );
	DINT;
	InitPieCtrl( );
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable( );
	/*    EALLOW;
	    SysCtrlRegs.HISPCP.all = ADC_MODCLK; // HSPCLK = SYSCLKOUT/ADC_MODCLK
	    EDIS;

		//initialize adc
		InitAdc();
		AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
		AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
		AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
		AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;// 1  级联模式
		AdcRegs.ADCTRL1.bit.CONT_RUN = 0;// 启动停止模式
		AdcRegs.ADCTRL1.bit.SEQ_OVRD = 1;// Enable Sequencer override feature
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x01;// convert and store in 1 results registersDELAY_US(100);
		AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
		AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x0;
		AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;
		while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);
		AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
	*/
	//InitXintf( );
	gTs = 1.0 / 85e3; //200k 开关频率
	gVm = 5 ; // gVm 输出有效值 1*1.414(根号2)
	gF = 100;// gF 频率
	gCnt = 0;
	gM = 0;
	InitEPwm( );
	/*lcd_init();
	Configio_Button();*/
	//initialize timer0
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	/*PieVectTable.XINT13 = &cpu_timer1_isr;*/
	/*PieVectTable.TINT2 = &cpu_timer2_isr;*/
	EDIS;
	InitCpuTimers();
	//For this example, only initialize the Cpu Timers
	ConfigCpuTimer ( &CpuTimer0, 150, 1000 );
	CpuTimer0Regs.TCR.all = 0x4001; // 设置TIE = 1，开启定时器0中断
	CpuTimer0.InterruptCount = 0;
	StartCpuTimer0();
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	//InitGlobalVariable( );
	asm ( " EALLOW" );
	PieVectTable.EPWM1_INT = &User_EPWM1_INT_ISR;
	asm ( " EDIS" );
	IER |= M_INT3;
	//  IER |= M_INT2;
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	//  PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
	EINT;
	ERTM;

	while ( 1 ) {
		/********
		*  显示  *
		********/
		/*
		if (lcdflag==1)
		{
		    lcdflag=0;
		    LCD12864_Clear();
		    char show1[12] = "";
		    num2char(show1, gM, 1, 4);
		    char show11[12]  = "占空比：";
		    strcat(show11, show1);
		    strcat(show11, "");
		    strDisp(1, 1, show11, 13);

		    char show2[12] = "";
		    num2char(show2, adc0, 1, 3);
		    char show21[12]  = "电压0：";
		    strcat(show21, show2);
		    strcat(show21, "V");
		    strDisp(2, 1, show21, 14);

		    char show3[12] = "";
		    num2char(show3, adc1, 1, 3);
		    char show31[12]  = "电压1：";
		    strcat(show31, show3);
		    strcat(show31, "V");
		    strDisp(3, 1, show31, 14);

		    char show4[12] = "";
		    num2char(show4, (adc0-adc1)/0.185, 1, 3);
		    char show41[12]  = "电流：";
		    strcat(show41, show4);
		    strcat(show41, "A");
		    strDisp(4, 1, show41, 14);
		    /*if (mode)
		    {
			    strcat(show4, "频率");
		    }
		    else
		    {
			    strcat(show4, "电压");
		    }
		    if (load)
		    {
			    strcat(show4, "*");
		    }
		    else
		    {
			    strcat(show4, " ");
		    }
		    strDisp(4, 1, show4, 14);
		}*/
		/*DELAY_US(1000000);*/
		/**********
		*  按键扫描  *
		**********/
		//if (load == 1) gVm = gVm * 2; ////
		/*while (input == '\0' || Scan_Button() != '\0')
		{
			input = Scan_Button();
		}*/
		/*if (matrixflag==1)
		{
		    matrixflag=0;
		    input = Scan_Button();
		    switch (input)
		    {
		        case '*':
		        //确认
		        if (mode)
		        {
		            gF = atof(disp);
		        }
		        else
		        {
		            gVm = atof(disp);
		        }
		        break;
		        case 'C':
		        //无论确认清空，都要将显示清空
		        dot = 0;
		        strcpy(disp, "");
		        break;
		        case '#':
		        if (strlen(disp))
		        {
		            if (disp[strlen(disp) - 1] == '.')
		            {
		                dot = 0;
		            }
		            disp[strlen(disp) - 1] = '\0';
		        }
		        break;
		        case 65:
		        num++;
		        adcflag=1;
		        //mode = !mode;
		        break;
		        case 'B':
		        num--;
		        adcflag=1;
		        //load = !load;
		        break;
		        case 'D':
		        if (dot == 0)
		        {
		            strcat(disp, ".");
		            dot = 1;
		        }
		        break;
		        default:
		        if (strlen(disp) < 10)
		        {
		            disp[strlen(disp) + 1] = '\0';
		            disp[strlen(disp)] = input;
		        }
		    }
		    input='\0';
		}*/
	}
}

extern interrupt void User_EPWM1_INT_ISR ( void ) {
	float Time, Omiga;
	unsigned int CMP;
	/*Time = gCnt * gTs;
	Omiga = 2.0 * mPI * gF * Time;
	gVoltage = gVm * 1.414 * cos( Omiga ) + 10;

	//gVoltage_Save[gCnt_Save] = gVoltage;
	gCnt++;
	if ( Omiga >= 2.0 * mPI )
	{
		gCnt = 0;
	}*/
	//gM = (float)(890e-3 + num*1e-3);
	//if (gM>1) gM=1;
	//if (gM<0) gM=0;
	gM = 0.5;
	CMP = gM * EPwm1Regs.TBPRD;
	//gCMP_Save[gCnt_Save] = CMP;
	//gCnt_Save++;
	//if( gCnt_Save >= 200 )
	{
		//	gCnt_Save = 0;
	}
	EPwm1Regs.CMPA.half.CMPA = CMP;
	GpioDataRegs.GPBTOGGLE.bit.GPIO60 = 1;
	////===================================
	EPwm1Regs.ETCLR.bit.INT = 1;
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
}

interrupt void cpu_timer0_isr ( void ) {
	asm ( " EALLOW" );

	if ( turn < 60 ) {
		GpioDataRegs.GPADAT.bit.GPIO3 = 0;
		GpioDataRegs.GPADAT.bit.GPIO5 = 1;
		GpioDataRegs.GPADAT.bit.GPIO7 = 1;
		GpioDataRegs.GPADAT.bit.GPIO9 = 1;

	} else {
		switch ( ( turn - 60 ) / CHARGE_PERIOD % 4 ) {
			case 0:
				GpioDataRegs.GPADAT.bit.GPIO3 = 1;
				GpioDataRegs.GPADAT.bit.GPIO5 = 0;
				GpioDataRegs.GPADAT.bit.GPIO7 = 1;
				GpioDataRegs.GPADAT.bit.GPIO9 = 1;
				break;

			case 1:
				GpioDataRegs.GPADAT.bit.GPIO3 = 1;
				GpioDataRegs.GPADAT.bit.GPIO5 = 1;
				GpioDataRegs.GPADAT.bit.GPIO7 = 0;
				GpioDataRegs.GPADAT.bit.GPIO9 = 1;
				break;

			case 2:
				GpioDataRegs.GPADAT.bit.GPIO3 = 1;
				GpioDataRegs.GPADAT.bit.GPIO5 = 1;
				GpioDataRegs.GPADAT.bit.GPIO7 = 1;
				GpioDataRegs.GPADAT.bit.GPIO9 = 0;
				break;

			case 3:
				GpioDataRegs.GPADAT.bit.GPIO3 = 0;
				GpioDataRegs.GPADAT.bit.GPIO5 = 1;
				GpioDataRegs.GPADAT.bit.GPIO7 = 1;
				GpioDataRegs.GPADAT.bit.GPIO9 = 1;
				break;
		}
	}

	asm ( " EDIS" );
	turn ++ ;
	/*
	 *    lcdcount++;
	 *
	 *    if ( lcdcount == 100 ) {
	 *        lcdflag = 1;
	 *        lcdcount = 0;
	 *    }
	 *
	 *    matrixflag = 1;
	 *    CpuTimer0.InterruptCount++;
	 *    samplevalue0 = Ad_Get ( 0 );
	 *    samplevalue1 = Ad_Get ( 4 );
	 *    samplesave0[adcindex] = samplevalue0;
	 *    samplesave1[adcindex] = samplevalue1;
	 *    adcindex++;
	 *
	 *    if ( adcindex == 10 ) {
	 *        adcindex = 0;
	 *        adc0 = ( float ) ( samplesave0[0] + samplesave0[1] + samplesave0[2] + samplesave0[3] + samplesave0[4] + samplesave0[5] + samplesave0[6] + samplesave0[7] + samplesave0[8] + samplesave0[9] ) * 3.0 / 4096.0 / 10.0;
	 *        adc1 = ( float ) ( samplesave1[0] + samplesave1[1] + samplesave1[2] + samplesave1[3] + samplesave1[4] + samplesave1[5] + samplesave1[6] + samplesave1[7] + samplesave1[8] + samplesave1[9] ) * 3.0 / 4096.0 / 10.0;
	 *    }
	 *
	 *    if ( adcflag == 1 ) {
	 *        adc0 = adc1;
	 *        adcflag = 0;
	 *
	 *    } else {
	 *        if ( abs ( adc1 - adc0 ) < 0.005 )
	 *            adc0 = adc1;
	 *    }
	 *
	 *    //Acknowledge this interrupt to receive more interrupts from group 1
	 *    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	 *    CpuTimer0Regs.TCR.bit.TIF = 1;
	 *    CpuTimer0Regs.TCR.bit.TRB = 1;
	 *    timer0count++;
	 */
}

// =====================================================================================================
// End of file
// =====================================================================================================
