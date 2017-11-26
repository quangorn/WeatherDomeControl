#include "../definitions.h"
#include "mlx90614.h"
#include "../i2c/i2c.h"
#include <util/delay.h>

//TODO: check PEC

#define MLX90614_I2C_DEV_ID 0x5a

int8_t readRegister(int8_t addr, int16_t* regPtr) {
	int8_t result = 0;
	int16_t buf[2];
	if (i2cStart() &&
			i2cWriteSlaveAddr(MLX90614_I2C_DEV_ID, 0) &&
			i2cWriteDataByte(addr) &&
			i2cStart() &&
			i2cWriteSlaveAddr(MLX90614_I2C_DEV_ID, 1) &&
			i2cRead((uint8_t*)buf, 3)) {
		*regPtr = buf[0];
		result = 1;
	}
	i2cStop();
	return result;
}

int8_t writeRegister(int8_t addr, int16_t regValue, int8_t nPEC) {
	int8_t* buf = (int8_t*)&regValue;
	int8_t result = 0;
	if (i2cStart() &&
			i2cWriteSlaveAddr(MLX90614_I2C_DEV_ID, 0) &&
			i2cWriteDataByte(addr) &&
			i2cWriteDataByte(buf[0]) &&
			i2cWriteDataByte(buf[1]) &&
			i2cWriteDataByte(nPEC)) {
		result = 1;
	}
	i2cStop();
	return result;
}

void calculateTemp(int16_t regValue, int32_t* temperaturePtr) {
	*temperaturePtr = regValue;
	*temperaturePtr <<= 1;
	*temperaturePtr -= 27315;
}

int8_t mlxGetCurrentData(struct mlx90614_data* dataPtr) {
	int16_t regValue;
	if (!readRegister(0x06, &regValue)) {
		return 0;
	}
	calculateTemp(regValue, &dataPtr->temperature);

	//Без этой задержки датчик случайным образом отваливается
	_delay_ms(2);

	if (!readRegister(0x07, &regValue)) {
		return 0;
	}
	calculateTemp(regValue, &dataPtr->irTemperature);
	return 1;
}

int8_t mlxGetPWM(int16_t* dataPtr) {
	if (!readRegister(0x22, dataPtr)) {
		return 0;
	}
	return 1;
}

int8_t mlxWritePWM(int16_t nPWM, int8_t nPEC) {
	if (!writeRegister(0x22, nPWM, nPEC)) {
		return 0;
	}
	return 1;
}