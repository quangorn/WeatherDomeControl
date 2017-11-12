#include "../definitions.h"
#include "usart.h"
#include <avr/io.h>

#define UART_PRESCALER (F_CPU / 16 / UART_BAUD_RATE - 1)

void usartInit () {
	//Baud Rate speed setting
	UBRR0H = UART_PRESCALER >> 8;
	UBRR0L = UART_PRESCALER;

	//Enable transmitter and receiver, enable receive interrupt
	UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0);

	//Async mode, 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

uint8_t usartReceiveByte() {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void usartTransmitByte(uint8_t data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void usartTransmitString(char* str) {
	for (uint16_t i = 0; str[i]; i++) {
		usartTransmitByte(str[i]);
	}
}