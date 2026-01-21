#pragma once 
/**
 * ESP32RotaryEncoder: BasicRotaryEncoder.ino
 * 
 * This is a basic example of how to instantiate a single Rotary Encoder.
 * 
 * Turning the knob will increment/decrement a value between 1 and 10 and
 * print it to the serial console.
 * 
 * Pressing the button will output "boop!" to the serial console.
 * 
 * Created 3 October 2023
 * Updated 1 November 2023
 * By Matthew Clark
 */

#include <ESP32RotaryEncoder.h>

// Change these to the actual pin numbers that
// you've connected your rotary encoder to
#define DI_ENCODER_A 27
#define DI_ENCODER_B 12
#define DI_ENCODER_SW 14


#define ENCODER_MIN 1
#define ENCODER_MAX 10

void encoder_init(void (*knobCallback)(long value), void (*buttonCallback)(unsigned long duration));