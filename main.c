#include "Peripheral_Setup.h"

/**
 * main.c
 */

uint32_t count = 0;

__interrupt void isr_cpu_timer0(void);

int main(void)
{
    InitSysCtrl();                                      // Initialize System Control
    DINT;                                               // Disable CPU interrupts
    InitPieCtrl();                                      // Initialize the PIE control registers to their default state

    IER = 0x0000;                                       // Disable CPU interrupts
    IFR = 0x0000;                                       // Clear all CPU interrupt flags

    InitPieVectTable();                                 // Initialize the PIE vector table

    Setup_GPIO();

    EALLOW;
    PieVectTable.TINT0 = &isr_cpu_timer0;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;                  // Timer 0 - Column (Table)
    EDIS;
    IER |= M_INT1;                                      // Line (Table)

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 60, 1000000);
    // CpuTimer0Regs.TCR.all = 0x4001;                  // Enable interruption
    StartCpuTimer0();                                   // Enable interruption

    EINT;                                               // Enable Global interrupt INTM
    ERTM;                                               // Enable Global real time interrupt DBGM

    // Configuring LEDs
    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    GpioDataRegs.GPASET.bit.GPIO1 = 1;
    GpioDataRegs.GPASET.bit.GPIO2 = 1;
    GpioDataRegs.GPASET.bit.GPIO3 = 1;

    while (1)
    {
        for (count = 0; count < 0x0007FFFF; count++)
        {
        }

        GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
    }

    return 0;
}

__interrupt void isr_cpu_timer0(void)
{
    GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
