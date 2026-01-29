#ifndef CO2_DETECT_DHT22_H
#define CO2_DETECT_DHT22_H

#include "common_libs.h"
#include "shared_constants.h"

#include "Utils/utils.h"

class DHT22
{
public:
	static void init();
private:
	static const char *_TAG;
};
#endif //CO2_DETECT_DHT22_H