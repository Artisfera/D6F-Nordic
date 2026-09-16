#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
LOG_MODULE_REGISTER(main);

int main(void)
{
        adc_channel_setup(adc, &channel_cfg);

        while (1) {
                adc_read(adc, &sequence);
                int32_t voltage_mv = sample * 3000 / 4095;
                LOG_INF("milli Voltage: %d mV", voltage_mv);

                k_msleep(100);
        }
}
