#ifndef ADC_H_
#define ADC_H_

#include <zephyr/drivers/adc.h>

extern const struct device *adc;
extern struct adc_channel_cfg channel_cfg;
extern int16_t sample;
extern struct adc_sequence sequence;

#endif
