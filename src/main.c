#include <zephyr/kernel.h>
#include "adc.h"
#include "pwm.h"
#include "spi.h"

int main(void)
{
        printf("Hello World!\n");
        spi_init();
//        adc_init();
//        pwm_init();
//        adc_loop();
}
