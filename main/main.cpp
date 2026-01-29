#include "main.h"

const char *Main::_TAG = "Main";

extern "C" void app_main(void) {
    Main::init();
}

void Main::init()
{
    printf("Watch me fly, mama!\n");
}
