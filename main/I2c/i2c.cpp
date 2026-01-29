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

	if (!ccs811_check_app_valid())
	{
		vTaskDelay(pdMS_TO_TICKS(2000));
		esp_restart();
	}

	PRINTF_COLOR(COLOR_BLUE, "ccs811 app is valid\n");

	start_app();

	uint8_t meas_mode[2] = { 0x01, 0x10 };
	i2c_master_transmit(_ccs811_handle, meas_mode, 2, pdMS_TO_TICKS(100));

	vTaskDelay(pdMS_TO_TICKS(1000));

	PRINTF_COLOR(COLOR_BLUE, "Waiting for first DATA_READY...\n");
	uint8_t status;
	constexpr uint8_t status_reg = STATUS_REG;
	do {
		i2c_master_transmit_receive(_ccs811_handle, &status_reg, 1, &status, 1, pdMS_TO_TICKS(100));
		PRINTF_COLOR(COLOR_RED, "status register: 0x%02x (DATA_READY = 0x08)\n", status);
		vTaskDelay(pdMS_TO_TICKS(100));
	} while (!(status & 0x08));
	PRINTF_COLOR(COLOR_GREEN, "First measurement ready!\n");

	xTaskCreatePinnedToCore(task_i2c, "i2c_task", 4096, nullptr, 1, nullptr, 1);
}

bool I2c::read_ccs811(uint16_t *eco2, uint16_t *tvoc)
{
	if (!eco2 || !tvoc) return false;

	uint8_t status_reg = 0x00;
	uint8_t status = 0;
	if (i2c_master_transmit_receive(_ccs811_handle, &status_reg, 1, &status, 1, pdMS_TO_TICKS(100)) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Couldn't read status register!\n");
		return false;
	}

	if (!(status & 0x08))
	{
		PRINTF_COLOR(COLOR_RED, "status register: 0x%02x (DATA_READY = 0x08)\n", status);

		uint8_t error_reg = 0xe0;
		uint8_t error_id = 0;
		i2c_master_transmit_receive(_ccs811_handle, &error_reg, 1, &error_id, 1, pdMS_TO_TICKS(100));

		PRINTF_COLOR(COLOR_RED, "error register: 0x%02x | error id: 0x%02x\n", error_reg, error_id);

		return false;
	}

	uint8_t reg = 0x02;
	uint8_t buf[8] = {};
	if (i2c_master_transmit_receive(_ccs811_handle, &reg, 1, buf, 8, pdMS_TO_TICKS(100)) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Couldn't read data register!\n");
		return false;
	}

	*eco2 = (buf[0] << 8) | buf[1];
	*tvoc = (buf[2] << 8) | buf[3];

	return true;
}


bool I2c::ccs811_check_app_valid()
{
	constexpr uint8_t status_reg = STATUS_REG;
	uint8_t status = 0;
	if (i2c_master_transmit_receive(_ccs811_handle, &status_reg, 1, &status, 1, pdMS_TO_TICKS(100)) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Couldn't read status register!\n");
		return false;
	}

	if (!(status & APP_VALID_BIT))
	{
		PRINTF_COLOR(COLOR_RED, "app not valid!: 0x%02x (DATA_READY = 0x10)\n", status);
		return false;
	}

	return true;
}

bool I2c::start_app()
{
	uint8_t app_start = APP_START_REG;
	i2c_master_transmit(_ccs811_handle, &app_start, 1, pdMS_TO_TICKS(100));

	vTaskDelay(pdMS_TO_TICKS(100));

	constexpr uint8_t status_reg = STATUS_REG;
	uint8_t status = 0;
	if (i2c_master_transmit_receive(_ccs811_handle, &status_reg, 1, &status, 1, pdMS_TO_TICKS(100)) != ESP_OK)
	{
		PRINTF_COLOR(COLOR_RED, "Couldn't read status register!\n");
		return false;
	}

	if (!(status & FW_MODE_BIT))
	{
		PRINTF_COLOR(COLOR_RED, "status FW_MODE is set!: 0x%02x (FW_MODE = 0x40 bit 7)\n", status);
		return false;
	}

	return true;
}

void I2c::task_i2c(void *pvParameters)
{
	PRINTF_COLOR(COLOR_BLUE, "Starting i2c task...\n");

	while (true)
	{
		PRINTF_COLOR(COLOR_BLACK, "Reading CCS811...\n");

		uint16_t eco2, tvoc = 0;

		if (!read_ccs811(&eco2, &tvoc))
		{
			PRINTF_COLOR(COLOR_RED, "Read operation failed!\n");
		}
		else
		{
			PRINTF_COLOR(COLOR_GREEN, "eCO2: %u ppm | TVOC: %u ppb\n", eco2, tvoc);
		}

		vTaskDelay(pdMS_TO_TICKS(3000));
	}

	vTaskDelete(nullptr);
}