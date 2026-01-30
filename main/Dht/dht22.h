#ifndef CO2_DETECT_DHT22_H
#define CO2_DETECT_DHT22_H

#include "common_libs.h"
#include "shared_constants.h"

#include "Utils/utils.h"
#include "dht.h"

#define DHT_PIN GPIO_NUM_5
#define DHT_TYPE DHT_TYPE_AM2301

#define DHT_RES_STRING "Temperature: %.1f C, Humidity: %.1f %%"

class DHT22
{
public:
	static void init();
	static bool read_data(float *temp, float *humidity);

private:
	static const char *_TAG;
};
#endif //CO2_DETECT_DHT22_H