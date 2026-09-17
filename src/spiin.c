#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(spi, LOG_LEVEL_DBG);

#define SPIOP SPI_WORD_SET(8) | SPI_TRANSFER_MSB
struct spi_dt_spec honeywell_pressure = SPI_DT_SPEC_GET(DT_NODELABEL(honeywell_pressure), SPIOP);



int spi_init(void)
{
    k_msleep(10000);
    if (spi_is_ready_dt(&honeywell_pressure)) {
        LOG_INF("SPI interface: READY");

    } else {
        LOG_ERR("SPI interface: NOT READY");
        return -1;
        
    }
    //LOG_INF("SPI read data: %02X %02X\n", rx_data[0], rx_data[1]);

    while (1) {
        uint8_t rx_data[2] = {0};

        struct spi_buf rx_buf = {
            .buf = rx_data,
            .len = sizeof(rx_data)
        };
        struct spi_buf_set rx = {
            .buffers = &rx_buf,
            .count = 1
        };

        int err = spi_read_dt(&honeywell_pressure, &rx);
        uint8_t status = rx_data[0] >> 6;

        LOG_DBG("SPI read error: %d", err);
        LOG_DBG("SPI read data: %02X %02X", rx_data[0], rx_data[1]);
        LOG_DBG("SPI read status: %d\n", status);
        
        if (err == 0 && status == 0) {
            int output = (rx_data[0] & 0x3F) * 256 + rx_data[1];
            float psi = ((output - 1638.0f) * 2.0f) / 13108.0f - 1.0f;

            LOG_INF("Pressure: %0.3f psi", psi);

            k_msleep(100);

        } else {
            LOG_ERR("SPI read failed with error: %d", err);
            k_msleep(1000);

        }
    }

    return 0;
}