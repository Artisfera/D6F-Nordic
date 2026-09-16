#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

static const struct device *pwm0_motor = DEVICE_DT_GET(DT_NODELABEL(pwm0));
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

int pwm_set_percent(int duty)
{
    uint32_t period = PWM_KHZ(50);
    uint32_t pulse = period / 100 * duty;

    return pwm_set(
        pwm0_motor,
        0,
        period,
        pulse,
        PWM_POLARITY_NORMAL
    );
}


int pwm_init(void)
{
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    gpio_pin_set(led1.port, led1.pin, 1);
    int duty = 0;
    pwm_set_percent(duty);

    while (1){
        for (int duty = 5; duty <=20; duty++)
        {
            pwm_set_percent(duty);
            k_msleep(100);
        }
        for (int duty = 20; duty >=5; duty--)
        {
            pwm_set_percent(duty);
            k_msleep(100);
        }
    }
}