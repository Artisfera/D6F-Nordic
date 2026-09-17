#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>
#include <zephyr/dt-bindings/adc/nrf-saadc.h>
#include "adc.h"

LOG_MODULE_REGISTER(adcin, LOG_LEVEL_DBG);

const struct device *adc = DEVICE_DT_GET(DT_NODELABEL(adc));

struct adc_channel_cfg channel_cfg = {
        .input_positive = NRF_SAADC_AIN7,
        .gain = ADC_GAIN_1_5,
        .reference = ADC_REF_INTERNAL,
        .acquisition_time = ADC_ACQ_TIME_DEFAULT,
        .channel_id = 0,
        .differential = 0,
};

int16_t sample;

struct adc_sequence sequence = {
        .channels = BIT(0),
        .buffer = &sample,
        .buffer_size = sizeof(sample),
        .resolution = 14,
        .oversampling = 6,
};


int adc_init(void) {
        adc_channel_setup(adc, &channel_cfg);

        sequence.calibrate = true;
        adc_read(adc, &sequence);
        sequence.calibrate = false;

        return 0;

}


int adc_loop(void) {
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