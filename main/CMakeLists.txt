idf_component_register(SRCS
        "main.cpp"
        "Utils/utils.cpp"
        "I2c/i2c.cpp"
        REQUIRES mqtt
        REQUIRES esp_driver_i2c
        INCLUDE_DIRS ".")
