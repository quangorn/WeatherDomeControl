#include "motor.h"
#include <avr/io.h>

#define MOTOR_SPEED_STEP_UP 2
#define MOTOR_SPEED_STEP_DOWN 3
#define MOTOR_START_SPEED 5
#define MOTOR_MAX_SPEED 128

uint8_t motorTargetSpeed = MOTOR_START_SPEED;
bool motorDirection = false;

//совпадает ли текущее направление вращения мотора с установленным
bool motorIsDirectionRight() {
	return !motorDirection == !(PORTD & (1 << PORTD5)); //convert to bool
}

void motorSetDirection() {
	if (motorDirection) {
		PORTD |= (1 << PORTD5);
	} else {
		PORTD &= ~(1 << PORTD5);
	}
}

void motorInit() {
	//PD5 is now an output
	DDRD |= (1 << DDD5);
	
	//PB1 is now an output
	DDRB |= (1 << DDB1);

	//set none-inverting mode and fast PWM Mode
	TCCR1A |= (1 << WGM10) | (1 << WGM12);

	//set prescaler to 256
	TCCR1B |= (1 << CS12);

	OCR1A = MOTOR_START_SPEED;
}

void motorStart(bool direction) {
	motorTargetSpeed = MOTOR_MAX_SPEED;
	motorDirection = direction;
	if (OCR1A <= MOTOR_START_SPEED) {
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
	if (OCR1A < targetSpeed) {
		if (OCR1A <= MOTOR_START_SPEED) {
			//enable PWM Mode
			TCCR1A |= 1 << COM1A1;
		}
		if (targetSpeed - OCR1A < MOTOR_SPEED_STEP_UP) {
			OCR1A = targetSpeed;
		} else {
			OCR1A += MOTOR_SPEED_STEP_UP;
		}
	} else if (OCR1A > targetSpeed) {
		if (OCR1A - targetSpeed < MOTOR_SPEED_STEP_DOWN) {
			OCR1A = targetSpeed;
		} else {
			OCR1A -= MOTOR_SPEED_STEP_DOWN;
		}
		if (OCR1A <= MOTOR_START_SPEED) {
			if (motorIsDirectionRight()) {
				//disable PWM Mode
				TCCR1A &= ~(1 << COM1A1);
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