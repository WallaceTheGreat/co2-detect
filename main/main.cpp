#include "main.h"

const char *Main::_TAG = "Main";

extern "C" void app_main(void) {
    Main::init();
}

void Main::init()
{
    print_header();

    I2c::init();
}

void Main::print_header()
{
    PRINTF_COLOR(COLOR_BLUE, "\n=========================================\n");
    PRINTF_COLOR(COLOR_BLUE, "\t%s\n", CO2_PROJECT_NAME);
    PRINTF_COLOR(COLOR_BLUE, "\tVersion: %s (%s)\n", CO2_VER_STRING, CO2_VER_VERBOSE);
    PRINTF_COLOR(COLOR_BLUE, "=========================================\n\n");
}