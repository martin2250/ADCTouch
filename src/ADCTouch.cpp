/*
  ADCTouch.cpp - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
#include "Arduino.h"
#include "ADCTouch.h"

int ADCTouchClass::read(byte ADCChannel, int samples)
{
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	uint8_t digitalPin = pgm_read_byte(analog_pin_to_digital_pin + ADCChannel);
	if (digitalPin == NOT_A_PIN) return;
#endif
	long _value = 0;
	for(int _counter = 0; _counter < samples; _counter ++)
	{
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		// set the digital pin corresponding to the analog pin as an input pin with a pullup resistor
		// this will start charging the capacitive element attached to that pin
		pinMode(digitalPin, INPUT_PULLUP);
		
		// connect the ADC input and the internal sample and hold capacitor to ground to discharge it
		ADMUX = (ADMUX & 0xF0) | 0b1101;
#else
		// set the analog pin as an input pin with a pullup resistor
		// this will start charging the capacitive element attached to that pin
		pinMode(ADCChannel, INPUT_PULLUP);
		
		// connect the ADC input and the internal sample and hold capacitor to ground to discharge it
		ADMUX |=   0b11111;
#endif
		// start the conversion
		ADCSRA |= (1 << ADSC);

		// ADSC is cleared when the conversion finishes
		while((ADCSRA & (1 << ADSC)));

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		pinMode(digitalPin, INPUT);
#else
		pinMode(ADCChannel, INPUT);
#endif
		_value += analogRead(ADCChannel);
	}
	return _value / samples;
}

ADCTouchClass ADCTouch;
