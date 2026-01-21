#include "rotary_encoder.h"

RotaryEncoder rotaryEncoder( DI_ENCODER_A, DI_ENCODER_B, DI_ENCODER_SW);
/*
void knobCallback( long value )
{
	Serial.printf( "Value: %ld\n", value );
}

void buttonCallback( unsigned long duration )
{
	Serial.printf( "boop! button was down for %lu ms\n", duration );
}
*/
void encoder_init(void (*knobCallback)(long value), void (*buttonCallback)(unsigned long duration))
{
  // This tells the library that the encoder has its own pull-up resistors
	rotaryEncoder.setEncoderType( EncoderType::FLOATING );

	// Range of values to be returned by the encoder: minimum is 1, maximum is 10
	// The third argument specifies whether turning past the minimum/maximum will
	// wrap around to the other side:
	//  - true  = turn past 10, wrap to 1; turn past 1, wrap to 10
	//  - false = turn past 10, stay on 10; turn past 1, stay on 1
	rotaryEncoder.setBoundaries( ENCODER_MIN, ENCODER_MAX, true );

	// The function specified here will be called every time the knob is turned
	// and the current value will be passed to it
	rotaryEncoder.onTurned( knobCallback );

	// The function specified here will be called every time the button is pushed and
	// the duration (in milliseconds) that the button was down will be passed to it
	rotaryEncoder.onPressed( buttonCallback );

	// This is where the inputs are configured and the interrupts get attached
	rotaryEncoder.begin();
}
