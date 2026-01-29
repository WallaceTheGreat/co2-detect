#ifndef CO2_DETECT_MAIN_H
#define CO2_DETECT_MAIN_H

#include "common_libs.h"
#include "shared_constants.h"

class Main
{
public:
	static void init();
	static void print_header();

private:
	static const char *_TAG;

};
#endif //CO2_DETECT_MAIN_H