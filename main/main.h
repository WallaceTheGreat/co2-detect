#ifndef CO2_DETECT_MAIN_H
#define CO2_DETECT_MAIN_H

#include "common_libs.h"
#include "shared_constants.h"

#include "Utils/utils.h"
#include "I2c/i2c.h"
#include "Dht//dht22.h"
#include "Measures/measures.h"

class Main
{
public:
	static void init();
	static void print_header();

private:
	static const char *_TAG;

};
#endif //CO2_DETECT_MAIN_H