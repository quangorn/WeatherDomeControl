
#include "definitions.h"
#include "usart/usart.h"
#include "i2c/i2c.h"
#include "bme280/bme280_user.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void toggleLED() {
	PORTD ^= 1 << PORTD2;
}

char buf[256];

int main (void) {
	//D2 is output
	DDRD |= 1 << DDD2;

	usartInit ();
	i2cInit();

	sei();

	int8_t bmeStatus = bmeInit();
	if (bmeStatus != BME280_OK) {
		sprintf(buf, "BME init failed: %d\r\n", bmeStatus);
		usartTransmitString(buf);
	}

	bmeStatus = bmeStartInNormalMode();
	if (bmeStatus != BME280_OK) {
		sprintf(buf, "BME start failed: %d\r\n", bmeStatus);
		usartTransmitString(buf);
	}
	
	struct bme280_data sensorData;	
	while (1) {
		_delay_ms(1050);
		toggleLED();
		bmeStatus = bmeGetCurrentData(&sensorData);
		if (bmeStatus == BME280_OK) {
			sprintf(buf, "Temperature: %ld; Pressure: %ld; Humidity: %ld;\r\n", sensorData.temperature, sensorData.pressure, sensorData.humidity);
			usartTransmitString(buf);
		} else {
			sprintf(buf, "BME get data failed: %d\r\n", bmeStatus);
			usartTransmitString(buf);
		}
	}
}

ISR (USART_RX_vect) {
	//int8_t received = usartReceiveByte();
	//if (received == 'a') {
		//toggleLED();
	//}
	//usartTransmitByte(received);
}

