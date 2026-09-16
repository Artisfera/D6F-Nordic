#include <zephyr/drivers/adc.h>
#include <zephyr/dt-bindings/adc/nrf-saadc.h>

const struct device *adc = DEVICE_DT_GET(DT_NODELABEL(adc));

struct adc_channel_cfg channel_cfg = {
        .input_positive = NRF_SAADC_AIN1,
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
        .resolution = 12,
        .oversampling = 0,
        .calibrate = false,
};