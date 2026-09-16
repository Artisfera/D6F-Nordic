#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "adc.h"
#include "pwm.h"
LOG_MODULE_REGISTER(main);

int main(void)
{
        pwm_init();

        adc_channel_setup(adc, &channel_cfg);

        sequence.calibrate = true;
        adc_read(adc, &sequence);
        sequence.calibrate = false;

        while (1) {
                adc_read(adc, &sequence);

                int32_t voltage_mv = sample;

                adc_raw_to_millivolts(
                        600,
                        channel_cfg.gain,
                        sequence.resolution,
                        &voltage_mv
                );

                float flow_ml_min = (voltage_mv - 500) / 20;

                if (flow_ml_min > 100) {
                        LOG_ERR("Flow is too HIGH! - %0.f ml/min", flow_ml_min);
                        k_msleep(100);
                } else{
                        LOG_INF("Voltage: %d mV", voltage_mv);
                        LOG_INF("Flow: %.0f ml/min", flow_ml_min);
                        k_msleep(1000);
                }

        }
}
