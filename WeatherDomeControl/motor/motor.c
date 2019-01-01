#include "motor.h"
#include <avr/io.h>

#define MOTOR_SPEED_STEP 2

uint8_t motorTargetSpeed = 0;
//TODO: поддержать запрос на изменение направления если мотор уже работает

void motorInit() {
	//PD5 is now an output
	DDRD |= 1 << DDD5;

	//set none-inverting mode and fast PWM Mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);

	//set prescaler to 1024
	TCCR0B |= (1 << CS00) | (1 << CS02);

	OCR0B = 0;
}

void motorStart(bool direction) {
	motorTargetSpeed = 0xFF;
}

void motorStop() {
	motorTargetSpeed = 0;
}

bool motorIsStarted() {
	return motorTargetSpeed != 0;
}

void motorProceed() {
	if (OCR0B < motorTargetSpeed) {
		if (OCR0B == 0) {
			//enable PWM Mode
			TCCR0A |= 1 << COM0B1;
		}
		if (motorTargetSpeed - OCR0B < MOTOR_SPEED_STEP) {
			OCR0B = motorTargetSpeed;
		} else {
			OCR0B += MOTOR_SPEED_STEP;
		}
	} else if (OCR0B > motorTargetSpeed) {
		if (OCR0B - motorTargetSpeed < MOTOR_SPEED_STEP) {
			OCR0B = motorTargetSpeed;
		} else {
			OCR0B -= MOTOR_SPEED_STEP;
		}
		if (OCR0B == 0) {
			//disable PWM Mode
			TCCR0A &= ~(1 << COM0B1);
		}
	}
}

void motorToggle(bool direction) {
	if (motorIsStarted()) {
		motorStop();
	} else {
		motorStart(direction);
	}
}