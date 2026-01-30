#include "dht22.h"

const char *DHT22::_TAG = "DHT22";

void DHT22::init()
{
	DEFAULT_INIT_TAG;
}

bool DHT22::read_data(float *temp, float *humidity)
{
	int16_t i_temp, i_humidity = 0;

	if (dht_read_data(DHT_TYPE, DHT_PIN, &i_humidity, &i_temp) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Failed to read data from DHT sensor!\n");
		return false;
	}

	*temp = i_temp / 10.0f;
	*humidity = i_humidity / 10.0f;

	return true;
}
