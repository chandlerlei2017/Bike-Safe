/* DESCRIPTION
 * Sample code showing keypad's response to pushing buttons 1 and 2
 * Include pressedKey on debugger's "Expressions" to see the hexaKeys' value when you alternate between the two keys
 * Did not include button debouncer in this (releasing the button does not set pressedKey back to Value 0 '\x00')
 */


#include <msp430.h>
#include "driverlib.h"
#include "Board.h"


char hexaKeys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void Key();
char pressedKey;

void main (void)
{
    WDT_A_hold(WDT_A_BASE);     // Stop watchdog timer

    // ROWS ARE OUTPUTS
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);                  // Row 1: Output direction
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);


    // COLUMNS ARE ISR TRIGGERS

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);     // Column 1: Input direction
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN5);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);     // Column 2: Input direction
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN3);

    _EINT();        // Start interrupt

    PMM_unlockLPM5();           // Need this for LED to turn on- in case of "abnormal off state"
    __bis_SR_register(LPM4_bits + GIE);     // Need this for interrupts or else "abnormal termination"
    __no_operation();           //For debugger
}

void Key()
{

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)     // Column 1 to GND
            pressedKey = hexaKeys[0][0];        // Shows "1"
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_LOW)     // Column 2
            pressedKey = hexaKeys[0][1];       // Shows "2"
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW


}

#pragma vector = PORT1_VECTOR       // Using PORT1_VECTOR interrupt because P1.4 and P1.5 are in port 1
__interrupt void PORT1_ISR(void)
{
    Key();

    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5);
}
