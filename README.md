# WaveClock

A bedside clock that is off until you wave your hand near it then it comes on to display the time. The idea being the clocks display will not disturn you at night.

This is an Arduino powered clock using an DS1307 RTC chip and a Max7219 LED controlle connected to a 4 digit 7-segment LED display.

There is an LED emitter and receiver (38KHz) being used as a proximity detector.

When the clock detects a hand being waved above it, the time will display for 2 seconds then go off.


