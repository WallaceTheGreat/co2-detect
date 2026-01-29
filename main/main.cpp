#include "main.h"

const char *Main::_TAG = "Main";

extern "C" void app_main(void) {
    Main::init();
}

void Main::init()
{
    print_header();
}

void Main::print_header()
{
    printf("\n=========================================\n");
    printf("\t%s\n", CO2_PROJECT_NAME);
    printf("\tVersion: %s (%s)\n", CO2_VER_STRING, CO2_VER_VERBOSE);
    printf("=========================================\n\n");
}