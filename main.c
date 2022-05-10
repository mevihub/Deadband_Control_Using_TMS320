/*
 * main.c
 *
 *  Created on: 05-Apr-2022
 *      Author: Devilal
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "F2837xD_device.h"
#include "F28x_Project.h"
#include "F2837xD_Examples.h"

#include "device.h"
#include "driverlib.h"

void PinMux_init_1();
void initEPWM1();


void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();
    PinMux_init_1();
    initEPWM1();


    while(1)
    {
        EPwm1Regs.CMPA.bit.CMPA = 200;    // Set compare A value

    }
}


void PinMux_init_1()
{
    EALLOW;
    //EPWM1 -> myEPWM1 Pinmux
    GpioCtrlRegs.GPAPUD.bit.GPIO0=0; //
    GpioCtrlRegs.GPAPUD.bit.GPIO1=0;

    GpioCtrlRegs.GPAMUX1.bit.GPIO0=1; // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO1=1; // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv

    EDIS;

}


void initEPWM1()
{
    // Disable sync(Freeze clock to PWM as well). GTBCLKSYNC is applicable
    // only for multiple core devices. Uncomment the below statement if
    // applicable.
    //
    // SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_GTBCLKSYNC);

    // Set Compare values

    //        // ------------ EPWM1A ------------------                            BUCK //

    EPwm1Regs.TBPRD = 1000;       // Set timer period 801 TBCLKs
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;

    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;


    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;           // load on CTR = Zero
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;

    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;

    EPwm1Regs.DBRED.all = 100;
    EPwm1Regs.DBFED.all = 100;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC=1;
    EDIS;
}


