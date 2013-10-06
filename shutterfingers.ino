// shutterfingers
//
// Positions a servo to depress the shutter button of a camera when triggered
// from a remote shutter release cable. See URL below for pinouts:
// http://www.doc-diy.net/photo/eos_wired_remote/

#include <Servo.h>

// trigger inputs, corresponding servo positions, and states
const int shutter_pin = 2;  // pin on which the shutter signal is received
const int shutter_pos = 20; // position at which the camera's shutter is released
int shutter_state;          // status of the shutter signal
const int focus_pin   = 4;
const int focus_pos   = 30;
int focus_state;
const int safety_pos  = 60;  // safety position for the servo, away from the shutter button

// shutter servo details
Servo shutter_servo;              // create servo object to control a servo
const int shutter_servo_pin = 8;  // pin on which the servo is attached
int shutter_servo_state;          // current state/position of the shutter servo

const int led_pin_external     = 6;   // to help with debugging
const int led_pin_internal     = 13;  // to help with debugging

void setup()
{
  shutter_servo.attach( shutter_servo_pin );  // attaches the servo on the defined pin to the servo object
  pinMode( shutter_pin, INPUT_PULLUP );
  pinMode( focus_pin, INPUT_PULLUP );
  pinMode( led_pin_internal, OUTPUT );
  pinMode( led_pin_external, OUTPUT );
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
      digitalWrite( led_pin_internal, HIGH );
      digitalWrite( led_pin_external, HIGH );
      shutter_servo_state = shutter_pos;
    }
  }
  else if ( LOW == focus_state )
  {
    if ( shutter_servo_state != focus_pos )
    {
      shutter_servo.write( focus_pos );
      digitalWrite( led_pin_internal, HIGH );
      digitalWrite( led_pin_external, HIGH );
      shutter_servo_state = focus_pos;
    }
  }
  else
  {
    if ( shutter_servo_state != safety_pos )
    {
      shutter_servo.write( safety_pos );
      digitalWrite( led_pin_internal, LOW );
      digitalWrite( led_pin_external, LOW );
      shutter_servo_state = safety_pos;
    }
  }

  // sleep for some number of milliseconds, have some patience
  delay( 15 );
}
