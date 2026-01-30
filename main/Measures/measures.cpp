#include "measures.h"

const char *Measures::_TAG = "Measures";

void Measures::init()
{
	DEFAULT_INIT_TAG;

	xTaskCreatePinnedToCore(task_measures, "measures", MEASURE_TASK_STACK, nullptr, 1, nullptr, 1);
	xTaskCreatePinnedToCore(task_write_env, "write_env", WRITE_ENV_DATA_TASK_STACK, nullptr, 1, nullptr, 1);
}

void Measures::task_measures(void *pvParameters)
{
	PRINTF_COLOR(COLOR_BLUE, "Starting measures task...\n");

	while (true)
	{
		uint16_t eco2, tvoc = 0;
		float temp, humidity = 0;

		if (!I2c::read_ccs811(&eco2, &tvoc))
			PRINTF_COLOR(COLOR_RED, "ccs811 Read operation failed!\n");

		if (!DHT22::read_data(&temp, &humidity))
			PRINTF_COLOR(COLOR_RED, "dht22 Read operation failed!\n");

		PRINTF_COLOR(COLOR_GREEN, "\nMeasure report:\n");
		PRINTF_COLOR(COLOR_GREEN, "================\n\n");
		PRINTF_COLOR(COLOR_GREEN, "\t" CCS811_RES_STRING "\n", eco2, tvoc);
		PRINTF_COLOR(COLOR_GREEN, "\t" DHT_RES_STRING "\n", temp, humidity);

		vTaskDelay(pdMS_TO_TICKS(MEASURE_DELAY_MS));
	}

	vTaskDelete(nullptr);
}

void Measures::task_write_env(void *pvParameters)
{
	PRINTF_COLOR(COLOR_BLUE, "Starting write_env task...\n");

	while (true)
	{
		float temp, humidity = 0;

		if (!DHT22::read_data(&temp, &humidity))
			PRINTF_COLOR(COLOR_RED, "dht22 Read operation failed!\n");

		if (!I2c::write_env_data(temp, humidity))
			PRINTF_COLOR(COLOR_RED, "Couldn't write environmental data!\n");

		vTaskDelay(pdMS_TO_TICKS(WRITE_ENV_DATA_MS));
	}
}
