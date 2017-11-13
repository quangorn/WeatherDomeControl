#ifndef MLX90614_H_
#define MLX90614_H_

#include <stdint.h>

struct mlx90614_data {
	//internal sensor temperature
	int32_t temperature;
	
	//infra red temperature
	int32_t irTemperature;
};

int8_t mlxGetCurrentData(struct mlx90614_data* dataPtr);

#endif /* MLX90614_H_ */