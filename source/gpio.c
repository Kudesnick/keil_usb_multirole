
#include "RTE_Components.h"
#include CMSIS_device_header

#include "gpio.h"
#ifndef RTE_DEVICE_HAL_COMMON
    #include "GPIO_STM32F10x.h"
#endif

void gpio_set(gpio_t gpio)
{
#ifdef RTE_DEVICE_HAL_COMMON
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
#else

    GPIO_PortClock(GPIO_PORT(gpio.pin), ON);
    GPIO_PinWrite(GPIO_PORT(gpio.pin), GPIO_PIN_SOURCE(gpio.pin), gpio.off_state ^ gpio.state);
    GPIO_PinConfigure(
        GPIO_PORT(gpio.pin),
        GPIO_PIN_SOURCE(gpio.pin),
        (gpio.mode == GPIO_MODE_PP) ? GPIO_OUT_PUSH_PULL : GPIO_OUT_OPENDRAIN,
        GPIO_MODE_OUT2MHZ
        );
#endif
};
