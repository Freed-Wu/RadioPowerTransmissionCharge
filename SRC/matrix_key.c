/*
 * matrix_key.c
 *
 *  Created on: 2019年5月18日
 *      Author: 10786
 */
#include"DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "matrix_key.h"
#include <string.h>
#define  L_O_V_I()     EALLOW;                             \
                       GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO2 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO4 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO4 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO6 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO6 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO8 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO8 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO10 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO10 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO12 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO12 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO14 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO14 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO16 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO16 = 0; DELAY_US(1);\
                       EDIS;
#define  L_I_V_O()     EALLOW;                             \
                       GpioCtrlRegs.GPADIR.bit.GPIO10 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO10 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO12 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO12 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO14 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO14 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO16 = 1; \
                       GpioDataRegs.GPADAT.bit.GPIO16 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO2 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO2 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO4 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO4 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO6 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO6 = 0; DELAY_US(1);\
                       GpioCtrlRegs.GPADIR.bit.GPIO8 = 0; \
                       GpioDataRegs.GPADAT.bit.GPIO8 = 0; DELAY_US(1);\
                       EDIS;
#define   V1    GpioDataRegs.GPADAT.bit.GPIO16
#define   V2    GpioDataRegs.GPADAT.bit.GPIO14
#define   V3    GpioDataRegs.GPADAT.bit.GPIO12
#define   V4    GpioDataRegs.GPADAT.bit.GPIO10
#define   L1    GpioDataRegs.GPADAT.bit.GPIO8
#define   L2    GpioDataRegs.GPADAT.bit.GPIO6
#define   L3    GpioDataRegs.GPADAT.bit.GPIO4
#define   L4    GpioDataRegs.GPADAT.bit.GPIO2
#define   uchar     unsigned char
void delay(Uint16 t)
{
	Uint16 i;
	while (t--)
	{
		for (i = 0; i < 125; i++);
	}
}
char Scan_Button(void)
{
    static char keystate=0;
	unsigned char i = 0, x = 0, y = 0;
	char str = '\0';
	i = V1 & V2 & V3 & V4; //如果列读取到有低电平说明有按键按下
	if (i == 0)
	{
		DELAY_US(50);//延时消抖
		i = V1 & V2 & V3 & V4;
		if (i == 0)    //如果列读取到仍然有低电平说明有按键按下
		{
			if (!V1) x = 0x10;
			if (!V2) x = 0x20;
			if (!V3) x = 0x30;
			if (!V4) x = 0x40;
			L_I_V_O();
			DELAY_US(50); //行与列输入输出反转
			if (!L2) y = 0x02;
			if (!L3) y = 0x03;
			if (!L4) y = 0x04;
			if (!L1) y = 0x01;
			x |= y;
			L_O_V_I();
		}
		else x=0x00;
	}
	switch (keystate)
	{
	    case 0:
	    if (x!=0x00) keystate=1;
	    break;

	    case 1:
	    if (x==0x00)
	        keystate=0;
	    else
	    {
	        keystate=2;
	        switch (x)
	        {
	            case 0x11:
	            str = '1';
	            break;
	            case 0x12:
	            str = '2';
	            break;
	            case 0x13:
	            str = '3';
	            break;
	            case 0x14:
	            str = 'A';
	            break;
	            case 0x21:
	            str = '4';
	            break;
	            case 0x22:
	            str = '5';
	            break;
	            case 0x23:
	            str = '6';
	            break;
	            case 0x24:
	            str = 'B';
	            break;
	            case 0x31:
	            str = '7';
	            break;
	            case 0x32:
	            str = '8';
	            break;
	            case 0x33:
	            str = '9';
	            break;
	            case 0x34:
	            str = 'C';
	            break;
	            case 0x41:
	            str = '*';
	            break;
	            case 0x42:
	            str = '0';
	            break;
	            case 0x43:
	            str = '#';
	            break;
	            case 0x44:
	            str = 'D';
	            break;
	            default:
	            str = '\0';
	       }
	    }
	    break;

	    case 2:
	    if (x==0x00) keystate=0;
	    break;

	}

	return str;
}
void Configio_Button(void)
{
	EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
    //    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO49 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO52 = 0;10
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO53 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;
    //  GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;
    GpioCtrlRegs.GPAPUD.all = 0;
    EDIS;
	L_O_V_I();  //行设置为输出（输出低电平）列设置为输入
}




