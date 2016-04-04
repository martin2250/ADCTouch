/*
  ADCTouch.cpp - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
#include "Arduino.h"
#include "ADCTouch.h"

int ADCTouchClass::read(byte ADCChannel, int samples)
{
	long _value = 0;
	for(int _counter = 0; _counter < samples; _counter ++)
	{
		pinMode(ADCChannel, INPUT_PULLUP);
		
		ADMUX |=   0b11111;
		ADCSRA |= (1<<ADSC); //start conversion
		while(!(ADCSRA & (1<<ADIF))); //wait for conversion to finish
		ADCSRA |= (1<<ADIF); //reset the flag
		
		pinMode(ADCChannel, INPUT);
		_value += analogRead(ADCChannel);
	}
	return _value / samples;
}

ADCTouchClass ADCTouch;