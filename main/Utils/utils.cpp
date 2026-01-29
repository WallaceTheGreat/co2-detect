#include "utils.h"

const char *Utils::_TAG = "Utils";

void Utils::printf_color(const char* color, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf("%s", color);
	vprintf(fmt, args);
	printf("%s", COLOR_RESET);

	va_end(args);
}