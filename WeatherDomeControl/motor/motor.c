#include "motor.h"
#include <avr/io.h>

#define MOTOR_SPEED_STEP_UP 3
#define MOTOR_SPEED_STEP_DOWN 6
#define MOTOR_START_SPEED 0

uint8_t motorTargetSpeed = MOTOR_START_SPEED;
bool motorDirection = false;

//совпадает ли текущее направление вращения мотора с установленным
bool motorIsDirectionRight() {
	return !motorDirection == !(PORTD & (1 << PORTD7)); //convert to bool
}

void motorSetDirection() {
	if (motorDirection) {
		PORTD |= (1 << PORTD7);
	} else {
		PORTD &= ~(1 << PORTD7);
	}
}

void motorInit() {
	//PD5 is now an output
	DDRD |= (1 << DDD5) | (1 << DDD7);

	//set none-inverting mode and fast PWM Mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);

	//set prescaler to 1024
	TCCR0B |= (1 << CS00) | (1 << CS02);

	OCR0B = MOTOR_START_SPEED;
}

void motorStart(bool direction) {
	motorTargetSpeed = 0xFF;
	motorDirection = direction;
	if (OCR0B <= MOTOR_START_SPEED) {
		motorSetDirection();
	}
}

void motorStop() {
	motorTargetSpeed = MOTOR_START_SPEED;
}

bool motorIsStarted() {
	return motorTargetSpeed > MOTOR_START_SPEED;
}

void motorProceed() {
	uint8_t targetSpeed = motorTargetSpeed;
	if (!motorIsDirectionRight()) {
		targetSpeed = MOTOR_START_SPEED;
	}
	if (OCR0B < targetSpeed) {
		if (OCR0B <= MOTOR_START_SPEED) {
			//enable PWM Mode
			TCCR0A |= 1 << COM0B1;
		}
		if (targetSpeed - OCR0B < MOTOR_SPEED_STEP_UP) {
			OCR0B = targetSpeed;
		} else {
			OCR0B += MOTOR_SPEED_STEP_UP;
		}
	} else if (OCR0B > targetSpeed) {
		if (OCR0B - targetSpeed < MOTOR_SPEED_STEP_DOWN) {
			OCR0B = targetSpeed;
		} else {
			OCR0B -= MOTOR_SPEED_STEP_DOWN;
		}
		if (OCR0B <= MOTOR_START_SPEED) {
			if (motorIsDirectionRight()) {
				//disable PWM Mode
				TCCR0A &= ~(1 << COM0B1);
				//disable direction relay if motor is stopped
				motorDirection = false;
				motorSetDirection();
			} else {
				motorSetDirection();
			}
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