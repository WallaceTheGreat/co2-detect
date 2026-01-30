#ifndef CO2_DETECT_MEASURES_H
#define CO2_DETECT_MEASURES_H

#include "common_libs.h"
#include "shared_constants.h"

#include "Utils/utils.h"
#include "I2c/i2c.h"
#include "Dht/dht22.h"

#define MEASURE_DELAY_MS 2000
#define MEASURE_TASK_STACK 4096

#define WRITE_ENV_DATA_MS 60000
#define WRITE_ENV_DATA_TASK_STACK 2048

class Measures
{
public:
	static void init();
	static void task_measures(void *pvParameters);
	static void task_write_env(void *pvParameters);
private:
	static const char *_TAG;
};
#endif //CO2_DETECT_MEASURES_H