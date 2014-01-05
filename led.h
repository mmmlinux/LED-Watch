/*
 * led.h
 *
 *  Created on: Dec 4, 2013
 *      Author: elliott
 */

#ifndef LED_H_
#define LED_H_




#endif /* LED_H_ */


#define LED_DATA   GPIO_PIN_5
#define LED_EN GPIO_PIN_0
#define NOP __asm("\tNOP\n\t")

/*------------------------------------------------------------------------
  WS2811 control functions
------------------------------------------------------------------------*/
class WS2811 {
public:
  static void init() {
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	  GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_DATA | LED_EN);
	  GPIOPinWrite(GPIO_PORTB_BASE, LED_DATA|LED_EN, 0);
  }

  static void sendByte(int b) {
    int i=8;
    do {
      if ((b&0x80)==0) {
        // Send a '0'
    	  GPIOPinWrite(GPIO_PORTB_BASE, LED_DATA, LED_DATA);NOP;// Hi (start)
          NOP;NOP;
          NOP;NOP;
          NOP;// Hi
          GPIOPinWrite(GPIO_PORTB_BASE, LED_DATA, 0);NOP;// Lo (250ns)
          NOP;NOP;            // Lo
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          NOP;
          b=b+b;

          // Lo (500ns)
      }
      else {
        // Send a '1'
    	  GPIOPinWrite(GPIO_PORTB_BASE, LED_DATA, LED_DATA);NOP;// Hi (start)
          NOP;NOP;            // Hi
          NOP;NOP;            // Hi (250ns)
          NOP;NOP;            // Hi
          NOP;NOP;
          NOP;NOP;            // Hi
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          NOP;NOP;
          b = b+b;
          // Hi (500ns)
          GPIOPinWrite(GPIO_PORTB_BASE, LED_DATA, 0);    // Lo (625ns)
      }

    } while (--i!=0);
  }
};
WS2811 ws2811;

/*------------------------------------------------------------------------
  LED object
------------------------------------------------------------------------*/
class LED {
  int r_,g_,b_;
public:
  LED& setColor(int r, int g, int b) {
    r_ = r;
    g_ = g;
    b_ = b;
  }
  LED& setColor() {
      r_ = 0;
      g_ = 0;
      b_ = 0;
    }
  // nb. Interrupts must be disabled when you use this
  void send() const {
	_disable_interrupts( ) ;      // Lest we forget...
    ws2811.sendByte(g_);
    ws2811.sendByte(r_);
    ws2811.sendByte(b_);
  }
};


