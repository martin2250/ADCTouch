/*
  ADCTouch.cpp - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
#include "Arduino.h"
#include "ADCTouch.h"

int ADCTouchClass::read(byte analogPin, int samples)
{
  #ifndef ADC_GROUND
  /* this is the old error checking for pre 2.0.0 versions of ATTinyCore and other cores on the where you can't use analog and digital pin numbers interchangibly.
   * This dreadful behavior is finally fixed in ATTinyCore 2.0.0 - and now will work for ALL parts. Checking for ADC_GROUND is a good quick test, since it's defined
   * there, as well as megaTinyCore and DxCore - but nowhere else */
    #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__ATtiny13__)
      uint8_t digitalPin = pgm_read_byte(analog_pin_to_digital_pin + analogPin);
      if (digitalPin == NOT_A_PIN) return -1;
    #endif
  #endif // End t85/t13-specific check.

  long _value = 0;
  for(int _counter = 0; _counter < samples; _counter++) {
    #if defined(ADC_GROUND)
      /* Same implementation ought to work anywhere that we have the ADC_GROUND define, because that
       * implies that there's no need to use direct register manipulation to set this up - we can
       * use analogRead to read the ground channel, and be done with it. We don't need to do any '
       * direct register manipulation. (though it c)
      pinMode(analogPin,INPUT_PULLUP);
      // connect the ADC input and the internal sample and hold capacitor to ground to discharge it
      // We do this by reading the internal ground channel which the core exposes as ADC_GROUND.*/
      uint8_t oldSREG=SREG;                 // this part is timing-critical, so we will want to
      cli();                                // turn off interrupts briefly. savea SREG (making this
                                            // safe to call when interrupts already disabled. )
      analogRead(ADC_GROUND);               // Now turn interrupts off...
      pinMode(analogPin,INPUT);             // turn off pullup too...
      int16_t val = analogRead(analogPin);  // connect the discharged S&H cap to the pim and read
      SREG=oldSREG;                         // Restore SREG (setting interrupts to what they were).
      _value += val;                        // Add to running total of readings
                                            // There, that was easy wasn't it?
      /* The source of my concern around interrupts is that we would really prefer that the time
       * between discharging the S&H cap, after which charge will begin leaking back into it
       * (other end is connected to Vdd/2) and turn off the pullup, after which the pin and
       * sensing element will start picking up noise and when we start the second conversion
       * to be as close to 0 as possible. A slow interrupt would generate a bad reading, and 2 ADC
       * readings, while on a classic AVR, that is slow by microprocessor timescales, are still
       * well within typical safe lengths of time to keep interrupts off for. */
    #else
      #if (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__ATtiny13__))
        // set the digital pin corresponding to the analog pin as an input pin with a pullup resistor
        // this will start charging the capacitive element attached to that pin
        pinMode(digitalPin, INPUT_PULLUP);

        // connect the ADC input and the internal sample and hold capacitor to ground to discharge it
        ADMUX = (ADMUX & 0xF0) | 0b1101;
      #else
        // set the analog pin as an input pin with a pullup resistor
        // this will start charging the capacitive element attached to that pin
        pinMode(analogPin, INPUT_PULLUP);

        // connect the ADC input and the internal sample and hold capacitor to ground to discharge it
        ADMUX |=   0b11111;
      #endif
      // In order for it to actually be connected, we have to take a dummy reading of ground.
      // start the conversion (just to connect S&H cap to ground).
      ADCSRA |= (1 << ADSC);
      // ADSC is cleared when the conversion finishes
      while((ADCSRA & (1 << ADSC)));

      #if (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__ATtiny13__))
        pinMode(digitalPin, INPUT);
      #else
        pinMode(analogPin, INPUT);
      #endif
        _value += analogRead(analogPin);
    #endif /* end non ADC_GROUND defined case */
    }
  if ((samples & (samples - 1)) != 0) {
    /* oof, gotta use a software implementation of division on a 32-bit number by a 16-bit number on an 8-bit processor!
     * note that if LTO is enabled (it probably is), and samples is a compile-time known constant power of 2, this
     * branch will be optimized away entirely, which is what we want, because division operators are fairly large...*/
    return _value / samples;
  } else {
    /* Great, samples is a power of 2 - we can do this the fast, easy way:
     * Rightshift samples 1 bit at a time along with _value until samples = 1
     * at which point _value(current) = _value(original)/samples(original)
     * Probably around a dozen instrunction words or less (4 + 2 plus rjmp, compare, and conditional branch, plus maybe
     * a few movw's to set up the return) and worst case runtime of under 200 clocks. */
    while (samples > 1) {
      _value >>= 1;
      samples >>= 1;
    }
    return _value;
  }
}

ADCTouchClass ADCTouch;
