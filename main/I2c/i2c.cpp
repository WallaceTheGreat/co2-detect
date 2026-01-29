#include "i2c.h"

const char *I2c::_TAG = "I2c";
i2c_master_bus_handle_t I2c::_bus_handle = nullptr;
i2c_master_dev_handle_t I2c::_ccs811_handle = nullptr;

void I2c::init()
{
	DEFAULT_INIT_TAG;

	i2c_master_bus_config_t i2c_conf = {};
	i2c_conf.clk_source = I2C_CLK_SRC_DEFAULT;
	i2c_conf.i2c_port = I2C_PORT;
	i2c_conf.scl_io_num = I2C_MASTER_SCL_IO;
	i2c_conf.sda_io_num = I2C_MASTER_SDA_IO;
	i2c_conf.glitch_ignore_cnt = 7;
	i2c_conf.flags.enable_internal_pullup = true;

	if (i2c_new_master_bus(&i2c_conf, &_bus_handle) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Failed to initialize I2C bus.\n");
		vTaskDelay(pdMS_TO_TICKS(2000));
		esp_restart();
	}

	i2c_device_config_t dev_cfg = {};
	dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
	dev_cfg.device_address = CCS811_ADDR;
	dev_cfg.scl_speed_hz = 100000;

	if (i2c_master_bus_add_device(_bus_handle, &dev_cfg, &_ccs811_handle) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Failed to initialize CCS811.\n");
		vTaskDelay(pdMS_TO_TICKS(2000));
		esp_restart();
	}
}

bool I2c::read_ccs811(uint16_t& eco2, uint16_t& tvoc)
{
	uint8_t reg = 0x02;
	uint8_t buf[8] = {};

	esp_err_t err = i2c_master_transmit_receive(
		_ccs811_handle,
		&reg,
		1,
		buf,
		sizeof(buf),
		pdMS_TO_TICKS(100)
	);

	if (err != ESP_OK) {
		return false;
	}

	eco2 = (buf[0] << 8) | buf[1];
	tvoc = (buf[2] << 8) | buf[3];

	return true;
}
