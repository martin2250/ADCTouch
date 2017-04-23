## ADCTouch  
ADCTouch is a library that allows users to create a capacitive sensor without ANY external hardware.

## Purpose  
Most capacitive touch libraries require two pins and a large resistor to acquire precise readings. This library makes use of the AVRs internal wiring to get decent resolution with just a single pin.

## Usage  
`int val = ADCTouch.read(byte analogChannel, int samples = 100);`  
* analogChannel: the pin to use, you can just use A0 - A5  
* samples: (optional) the amount of samples to take, defaults to 100  
...range 1 - 65535
...(value does not change with different amounts of samples)

## How it works    
To Acquire a reading, the library does the following:

* charge the test pin to 5V via pullup resistor (not directly to prevent short circuits)
* discharge the internal ~14pF capacitor
* set the pin to tristate
* connect the 14pF capacitor with the pin so that charge distributes evenly
* measure the voltage of the internal cap with analogRead()  
...if the pin has a low capacitance, the stored charge will be small as will the resulting voltage, if the external capacitance is equal to 14pF, the volatage should be ~2.5V. Even higher capacitances will result in volatges > 2.5V. The chip and arduino board already have stray capacitances that will produce an output of ~390 and just a single external wire can boost that up to 500, so you really need offset compensation.

The accuracy is really good, most often even the LSB/smalles digit can still yield usable data and just vary by a single unit between readings (at only 100 samples, more will mean even less variation). The sensitivity is phenomenal, with a large enough surface, it can sense a person in more than 2 feet distance.

A more accurate explanation can be found [here](http://tuomasnylund.fi/drupal6/content/capacitive-touch-sensing-avr-and-single-adc-pin)
