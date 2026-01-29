#ifndef CO2_DETECT_I2C_H
#define CO2_DETECT_I2C_H

#include "common_libs.h"
#include "shared_constants.h"

#include "Utils/utils.h"

class I2c
{
public:
	static void init();
private:
	static const char *_TAG;
};
#endif //CO2_DETECT_I2C_H