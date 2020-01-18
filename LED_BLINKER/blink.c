//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  Texas Instruments, Inc
//  July 2013
//***************************************************************************************

#include <msp430.h>
#include "driverlib\MSP430FR2xx_4xx\driverlib.h"

unsigned int up_counter;
unsigned int distance_cm;
unsigned int flag;

const char digit[10] =
{
    0xFC,                                                      // "0"
    0x60,                                                      // "1"
    0xDB,                                                      // "2"
    0xF3,                                                      // "3"
    0x67,                                                      // "4"
    0xB7,                                                      // "5"
    0xBF,                                                      // "6"
    0xE4,                                                      // "7"
    0xFF,                                                      // "8"
    0xF7                                                       // "9"
};

Timer_A_initUpModeParam initUpParam_A0 = {
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    30000,
    TIMER_A_TAIE_INTERRUPT_DISABLE,
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
    TIMER_A_DO_CLEAR,
    true
};


void main(void) {

    //Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0); //Set Up the Timer

    SYSCFG2 |= LCDPCTL;

    LCDPCTL0 = 0xFFFF;
    LCDPCTL1 = 0x07FF;
    LCDPCTL2 = 0x00F0;

    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;

    LCDVCTL = LCDCPEN | LCDREFEN | VLCD_6 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);

    LCDMEMCTL |= LCDCLRM;
    LCDCSSEL0 = 0x000F;
    LCDCSSEL1 = 0x0000;
    LCDCSSEL2 = 0x0000;

    LCDM0 = 0x21;
    LCDM1 = 0x84;


    WDTCTL = WDTPW | WDTHOLD; //WatchDog DISABLED

    PM5CTL0 &= ~LOCKLPM5; //




//    P8DIR |= BIT3; //1.6 is output for trigger pulse
//    P8SEL0 &= ~BIT3;

    P1DIR |= BIT6;


    P8DIR |= BIT1; //8.1
    P8SEL0 &= ~BIT1;
    P8OUT |= BIT1;

    P8DIR |= BIT0; //8.0
    P8SEL0 &= ~BIT0;
    P8OUT |= BIT0;

    P5DIR |= BIT2; //5.2
    P5SEL0 &= ~BIT2;
    P5OUT |= BIT2;

    P5DIR |= BIT3; //5.3
    P5SEL0 &= ~BIT3;
    P5OUT |= BIT3;


    P1SEL0 &= (~BIT7); // Set P1.7 SEL as GPIO
    P1DIR &= (~BIT7); // Set P1.7 SEL as Input
    P1IES &= (~BIT7); // Falling Edge 1 -> 0
    P1IFG &= (~BIT7); // Clear interrupt flag for P1.7
    P1IE |= (BIT7); // Enable interrupt for P1.7

//    P1SEL0 &= (~BIT1); // Set P1.1 SEL as GPIO
//    P1DIR &= (~BIT1); // Set P1.1 SEL as Input
//    P1IES |= (BIT1); // Rising Edge 1 -> 0
//    P1IFG &= (~BIT1); // Clear interrupt flag for P1.1
//    P1IE |= (BIT1); // Enable interrupt for P1.1


    __enable_interrupt(); // Enable Global Interrupts

    flag = 0;


    for(;;)
    {
        //P8OUT |= BIT3;              // assert

        P1OUT |= BIT6;

        __delay_cycles(4);         // 10us wide

        //P8OUT &= ~BIT3;              // deassert

        P1OUT &= ~BIT6;

        __delay_cycles(24000);      // 60ms measurement cycle
    }
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
//    if (P1IN & BIT7){
//        //Stop timer and calculate
//        distance_cm = (Timer_A_getCounterValue(TIMER_A0_BASE))/58;
//
//        if (distance_cm > 2){
//            P8OUT &= ~BIT1;
//        }
//        else{
//            P8OUT |= BIT1;
//        }
//        flag = 0;
//
//        Timer_A_clear(TIMER_A0_BASE); //Clear the Timer Value
//
//        P1IFG &= (~BIT7);
//    }
//    else {
//        //Start timer
//        Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0); //Reset Timer
//        flag = 1;
//
//        P1IFG &= (~BIT1);
//    }
    P1IFG &= (~BIT7);
    CM_1
    //P1IFG &= (~BIT1);
}

