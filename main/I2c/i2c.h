#ifndef CO2_DETECT_I2C_H
#define CO2_DETECT_I2C_H

#include "common_libs.h"
#include "shared_constants.h"

#include "Utils/utils.h"

#define I2C_PORT I2C_NUM_0
#define I2C_MASTER_SDA_IO GPIO_NUM_22
#define I2C_MASTER_SCL_IO GPIO_NUM_23

#define CCS811_ADDR 0x5A

class I2c
{
public:
	static void init();
	static bool read_ccs811(uint16_t &eco2, uint16_t &tvoc);

private:
	static const char *_TAG;
	static i2c_master_bus_handle_t _bus_handle;
	static i2c_master_dev_handle_t _ccs811_handle;
};
#endif //CO2_DETECT_I2C_H
