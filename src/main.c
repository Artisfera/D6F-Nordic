#include <zephyr/kernel.h>
#include "adc.h"
#include "pwm.h"
#include "spi.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);


int main(void)
{
        printf("Hello World!\n");

        adc_init();
        spi_init();

        float psi = 0;
        float duty = 0;
        int ml_min = 0;
        int monitoring_duration = 120000;
        int64_t start_time = k_uptime_get();

        while(psi <= 0.3f) {
                duty = MIN(duty + 0.05f, 15.0f);
                pwm_set_percent(duty);

                spi_read_pressure(&psi);

                adc_read_flowrate(&ml_min);

                k_msleep(100);

        }

        duty = 0;
        pwm_set_percent(duty);
        LOG_INF("Vaccum Genereted!");
        
        while(k_uptime_get() - start_time <= monitoring_duration){
                spi_read_pressure(&psi);
                adc_read_flowrate(&ml_min);

                LOG_INF("Pressure (psi): %.3f", (double)psi);
                LOG_INF("Flowrate (ml/min): %d", ml_min);
                int64_t time_remaining = (monitoring_duration - (k_uptime_get() - start_time)) / 1000;
                LOG_INF("Time remaining (s): %d", time_remaining);

                k_msleep(10000);
        }

        return 0;
}