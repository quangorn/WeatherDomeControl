
#include "definitions.h"
#include "motor/motor.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void ledOn() {
	PORTC |= 1 << PORTC2;
}

void ledOff() {
	PORTC &= ~(1 << PORTC2);
}

void ledToggle() {
	PORTC ^= 1 << PORTC2;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma ide diagnostic ignored "OCDFAInspection"
int main (void) {
	//C2 is output
	PORTC |= 1 << DDC2;

	// PD2 and PD3 are buttons with pull up
	PORTD |= (1 << PORTD2) | (1 << PORTD3);

	motorInit();

	sei();

	uint8_t lastButtonsState = 0;
	while (1) {
		ledOn();
		_delay_ms(40);
		ledOff();

		uint8_t buttonsState = PIND & ((1 << PIND2) | (1 << PIND3));
		if (buttonsState != lastButtonsState) {
			if (!(PIND & (1 << PIND2))) {
				ledToggle();
				motorToggle(true);
			} else if (!(PIND & (1 << PIND3))) {
				ledToggle();
				motorToggle(false);
			}
			lastButtonsState = buttonsState;
		}

		motorProceed();
	}
}

