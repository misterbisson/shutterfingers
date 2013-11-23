/*
shutterfingers

Positions a servo to depress the shutter button of a camera when triggered
from a remote shutter release cable. See URL below for pinouts:
http://www.doc-diy.net/photo/eos_wired_remote/

This code is written for ATtiny85 running on a Digispark board.
See http://digistump.com/products/1

Version 1 of the project can was written for a more common ATmega328
or ATmega32U4-powered Arduino environment. It's still available from:
https://github.com/misterbisson/shutterfingers/releases

The ATtiny85 differs in a number if ways from ATmega328 and ATmega32U4 environments.
These two pages offer detail some of those differences:
https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
http://digistump.com/wiki/digispark/tutorials/basics

Regarding pins usage and pull-up (pullup) resistors on the Digispark and ATtiny85:

Overview of pins, fromhttp://digistump.com/board/index.php/topic,986.0.html

	"P5 has a weak drive, current is limited to a few mA, high level voltage is 3V @1mA and 2.5V @2mA
	P4 is connected to a zener diode for USB, the internal pullup is too weak to set the pin to HIGH
	P3 has a 1.5KΩ pullup
	P0 or P1 (depending on model) is connected to the onboard led, which interferes with the internal pullup"

More about pins to avoid, from http://digistump.com/board/index.php/topic,1105.0.html

	"I was saying that P5 is weak and useless as an output because 
	it's the reset pin and it's said in the datasheet you can't get a lot 
	of current from it, voltage drops a lot when you drain even a few mAs."

	"if you want to keep USB feature, P3/P4 must be kept"

And quoting from http://digistump.com/wiki/digispark/tutorials/basics :

	"Pin 3 (P3) has a 1.5 kΩ pull-up resistor attached to it which is required for
	when P3 and P4 are used for USB communication (including programming). Your design
	may need to take into account that you'd have to overpower this to pull this pin low."

	"The internal pull-up resistor are much weaker (about 25 kohm) on an ATtiny
	than on an Arduino, so the onboard LED interferes with them. If you need them,
	you can use a different port. Change your circuit to not need the internal
	pull-up, or cut the LED trace."

And because I think there are some typos in the above description about using
the the internal pull-ups, I used the example in http://digistump.com/board/index.php/topic,1086.msg4553.html#msg4553
when implementing them in this project.
*/

/*
Trigger inputs, corresponding servo positions, and states
*/
const int shutter_pin = 5;  // pin on which the shutter signal is received
const int shutter_pos = 65; // position at which the camera's shutter is released
int shutter_state;          // status of the shutter signal
const int focus_pin   = 2;
const int focus_pos   = 55;
int focus_state;
const int safety_pos  = 45;  // safety position for the servo, away from the shutter button

/*
Shutter servo details
Using Simple Servo here:
http://digistump.com/board/index.php/topic,444.0.html
https://github.com/digistump/DigisparkArduinoIntegration/tree/master/libraries/DigisparkSimpleServo

Alternatives (untested) include:
https://github.com/digistump/DigisparkArduinoIntegration/tree/master/libraries/DigisparkSoftRcPulseOut
https://github.com/fri000/Servo8Bit
*/
#include <SimpleServo.h>
SimpleServo shutter_servo;        // create servo object to control a servo
const int shutter_servo_pin = 0;  // pin on which the servo is attached
int shutter_servo_state;          // current state/position of the shutter servo

/*
Gotta have some LEDs
*/
const int led_pin = 1;   // to help with debugging

void setup()
{
	shutter_servo.attach( shutter_servo_pin );  // attaches the servo on the defined pin to the servo object
	pinMode( shutter_pin, INPUT );
	digitalWrite( shutter_pin, HIGH ); // activate internal pull-up
	pinMode( focus_pin, INPUT );
	digitalWrite( shutter_pin, HIGH ); // activate internal pull-up
	pinMode( led_pin, OUTPUT );
	digitalWrite( led_pin, LOW );
}

void loop()
{
	// read the inputs
	shutter_state = digitalRead( shutter_pin );
	focus_state   = digitalRead( focus_pin );

	// determine where to send the servo
	// only reposition the servo if the new position is different from the previous
	if ( LOW == shutter_state )
	{
		if ( shutter_servo_state != shutter_pos )
		{
			shutter_servo.write( shutter_pos );
			digitalWrite( led_pin, HIGH );
			shutter_servo_state = shutter_pos;
		}
	}
	else if ( LOW == focus_state )
	{
		if ( shutter_servo_state != focus_pos )
		{
			shutter_servo.write( focus_pos );
			digitalWrite( led_pin, HIGH );
			shutter_servo_state = focus_pos;
		}
	}
	else
	{
		if ( shutter_servo_state != safety_pos )
		{
			shutter_servo.write( safety_pos );
			digitalWrite( led_pin, LOW );
			shutter_servo_state = safety_pos;
		}
	}

	// sleep for some number of milliseconds, have some patience
	delay( 15 );
}
