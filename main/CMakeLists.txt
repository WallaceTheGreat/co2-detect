idf_component_register(SRCS
        "main.cpp"
        REQUIRES mqtt
        REQUIRES esp_driver_i2c
        INCLUDE_DIRS ".")
