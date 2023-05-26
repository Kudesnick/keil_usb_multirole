
#include "gpio.h"

void gpio_set(gpio_t gpio)
{
    HAL_RCC_GPIO_CLK_ENABLE(gpio.pin);
    HAL_GPIO_WritePin(GPIO_PORT(gpio.pin), GPIO_PIN(gpio.pin), gpio.off_state ^ gpio.state);
    
    GPIO_InitTypeDef GPIO_Init =
    {
        .Mode      = (gpio.mode == GPIO_MODE_PP) ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_OUTPUT_OD,
        .Pin       = GPIO_PIN(gpio.pin),
        .Pull      = (gpio.mode == GPIO_MODE_PU) ? GPIO_PULLUP : GPIO_NOPULL,
        .Speed     = GPIO_SPEED_FREQ_LOW,
        .Alternate = 0,
    };
    
    HAL_GPIO_Init(GPIO_PORT(gpio.pin), &GPIO_Init);
};
