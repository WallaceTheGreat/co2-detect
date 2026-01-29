#ifndef CO2_DETECT_UTILS_H
#define CO2_DETECT_UTILS_H

#include "common_libs.h"
#include "shared_constants.h"

#define COLOR_RESET     "\033[0m"

#define COLOR_BLACK     "\033[30m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_DARK_BLUE "\033[34;2m"

#define PRINTF_COLOR(color, fmt, ...)					\
	do {												\
		Utils::printf_color(color, fmt, ##__VA_ARGS__); \
	} while (0)

#define PRINTF_DEBUG(fmt, ...)													\
	do {																		\
		Utils::printf_color(COLOR_DARK_BLUE, "%s: " fmt, _TAG, ##__VA_ARGS__);	\
	} while (0)

class Utils
{
public:
	static void printf_color(const char *color, const char *fmt, ...);

private:
	static const char *_TAG;
};
#endif //CO2_DETECT_UTILS_H
