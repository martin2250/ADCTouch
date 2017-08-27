/*
  ADCTouch.h - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
#ifndef ADCT_h
#define ADCT_h

#include "Arduino.h"

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
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
	int read(byte ADCChannel, int samples = 100);
};

extern ADCTouchClass ADCTouch;

#endif
