#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "led.h"

#define LED_DATA   GPIO_PIN_5
#define LED_EN GPIO_PIN_0
#define NOP __asm("\tNOP\n\t")

LED led1;
LED led2;
LED led3;
void sendLEDs(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, LED_EN, LED_EN);

	for (int i = 0; i < 4; ++i) {
		led1.send();
		led2.send();
		led3.send();
	}

	_enable_interrupts();
	GPIOPinWrite(GPIO_PORTB_BASE, LED_EN | LED_DATA, 0);
}
void blankLEDs(int leds)
{
	LED blank;
	for (int i = 0; i < leds; ++i) {
		blank.send();
	}
}
int main(void) {
	//
	// Setup the system clock to run at 50 Mhz from PLL with crystal reference
	//
	SysCtlClockSet(
			SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
					| SYSCTL_OSC_MAIN);

	//
	// Enable and configure the GPIO port for the LED operation.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_DATA | LED_EN);
	GPIOPinWrite(GPIO_PORTB_BASE, LED_DATA, 0);


	blankLEDs(12);

	SysCtlDelay(100000);
	//
	// Loop Forever
	//
	while (1) {
		for (int i = 0; i < 64; ++i) {
			int ii = 64 - i;
			led1.setColor(i, ii, 0);
			led3.setColor(0, i, ii);
			led2.setColor(ii, 0, i);
			sendLEDs();

			SysCtlDelay(200000);
		}
		for (int i = 0; i < 64; ++i) {
			int ii = 64 - i;
			led3.setColor(i, ii, 0);
			led2.setColor(0, i, ii);
			led1.setColor(ii, 0, i);
			sendLEDs();

			SysCtlDelay(200000);
		}
		for (int i = 0; i < 64; ++i) {
			int ii = 64 - i;
			led2.setColor(i, ii, 0);
			led1.setColor(0, i, ii);
			led3.setColor(ii, 0, i);
			sendLEDs();

			SysCtlDelay(200000);
		}

//		SysCtlDelay(10000000);
//
//		led2.setColor(64,0,0);
//		led3.setColor(0,64,0);
//		led1.setColor(0,0,64);
//		sendLEDs();
//
//		SysCtlDelay(10000000);
//
//		led3.setColor(64,0,0);
//		led1.setColor(0,64,0);
//		led2.setColor(0,0,64);
//		sendLEDs();
//
//		SysCtlDelay(10000000);

	}
}
