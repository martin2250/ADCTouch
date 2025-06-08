/*
  ADCTouch.h - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
#ifndef ADCT_h
#define ADCT_h

#include "Arduino.h"

#ifndef __AVR_ARCH__
  #error "This library only supports AVR microcontrollers; this does not appear to be one."
#else
  #if (__AVR_ARCH__ < 102 ) && !defined(ADCSRA)
    #error "This is not a modern AVR, but if it is a classic AVR, it doesn't have an ADC."
  #endif
#endif

#if (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__ATtiny13__))&& !defined(ADC_GROUND)
const uint8_t PROGMEM analog_pin_to_digital_pin[] = {
	5, /* 0 */
	2,
	4,
	3,
	NOT_A_PIN,
	NOT_A_PIN
};
#endif

class ADCTouchClass
{
	public:
	int read(byte ADCChannel, int samples = 64); //Using powers of two allows us to avoid having to use division, which is painful, especially on classic tinyAVR parts.
};

extern ADCTouchClass ADCTouch;

#endif
