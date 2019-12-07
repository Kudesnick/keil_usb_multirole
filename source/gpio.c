/***************************************************************************************************
 *   Project:       
 *   Author:        
 ***************************************************************************************************
 *   Distribution:  
 *
 ***************************************************************************************************
 *   MCU Family:    STM32F
 *   Compiler:      ARMCC
 ***************************************************************************************************
 *   File:          filename.c
 *   Description:   
 *
 ***************************************************************************************************
 *   History:       01.12.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

#if defined (STM32F10X_LD)    || defined (STM32F10X_LD_VL) || defined (STM32F10X_MD)    || \
    defined (STM32F10X_MD_VL) || defined (STM32F10X_HD)    || defined (STM32F10X_HD_VL) || \
    defined (STM32F10X_XL)    || defined (STM32F10X_CL)
    
    #ifdef  __cplusplus
    extern "C"
    {
    #endif
        #include "GPIO_STM32F10x.h"
    #ifdef  __cplusplus
    };
    #endif
#else 
    #include "stm32f4xx_hal.h"
#endif

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

#define GPIO_GET_SOURCE_PIN(GPIO) ( GPIO == GPIO_PIN_0  ? 0  : \
                                    GPIO == GPIO_PIN_1  ? 1  : \
                                    GPIO == GPIO_PIN_2  ? 2  : \
                                    GPIO == GPIO_PIN_3  ? 3  : \
                                    GPIO == GPIO_PIN_4  ? 4  : \
                                    GPIO == GPIO_PIN_5  ? 5  : \
                                    GPIO == GPIO_PIN_6  ? 6  : \
                                    GPIO == GPIO_PIN_7  ? 7  : \
                                    GPIO == GPIO_PIN_8  ? 8  : \
                                    GPIO == GPIO_PIN_9  ? 9  : \
                                    GPIO == GPIO_PIN_10 ? 10 : \
                                    GPIO == GPIO_PIN_11 ? 11 : \
                                    GPIO == GPIO_PIN_12 ? 12 : \
                                    GPIO == GPIO_PIN_13 ? 13 : \
                                    GPIO == GPIO_PIN_14 ? 14 : \
                                    GPIO == GPIO_PIN_15 ? 15 : \
                                    16)
#define GPIO_GET_SOURCE_PORT(GPIO) (GPIO == GPIOA ? 0 << 4 : \
                                    GPIO == GPIOB ? 1 << 4 : \
                                    GPIO == GPIOC ? 2 << 4 : \
                                    GPIO == GPIOD ? 3 << 4 : \
                                    GPIO == GPIOE ? 4 << 4 : \
                                    GPIO == GPIOF ? 5 << 4 : \
                                    GPIO == GPIOG ? 6 << 4 : \
                                    7)

#define GPIO_IRQ_CHANNEL(GPIO)   \
(                                                     \
    (GPIO_PIN_SOURCE(GPIO) == 0x0) ? EXTI0_IRQn     : \
    (GPIO_PIN_SOURCE(GPIO) == 0x1) ? EXTI1_IRQn     : \
    (GPIO_PIN_SOURCE(GPIO) == 0x2) ? EXTI2_IRQn     : \
    (GPIO_PIN_SOURCE(GPIO) == 0x3) ? EXTI3_IRQn     : \
    (GPIO_PIN_SOURCE(GPIO) == 0x4) ? EXTI4_IRQn     : \
    (GPIO_PIN_SOURCE(GPIO) == 0x5) ? EXTI9_5_IRQn   : \
    (GPIO_PIN_SOURCE(GPIO) == 0x6) ? EXTI9_5_IRQn   : \
    (GPIO_PIN_SOURCE(GPIO) == 0x7) ? EXTI9_5_IRQn   : \
    (GPIO_PIN_SOURCE(GPIO) == 0x8) ? EXTI9_5_IRQn   : \
    (GPIO_PIN_SOURCE(GPIO) == 0x9) ? EXTI9_5_IRQn   : \
    (GPIO_PIN_SOURCE(GPIO) == 0xA) ? EXTI15_10_IRQn : \
    (GPIO_PIN_SOURCE(GPIO) == 0xB) ? EXTI15_10_IRQn : \
    (GPIO_PIN_SOURCE(GPIO) == 0xC) ? EXTI15_10_IRQn : \
    (GPIO_PIN_SOURCE(GPIO) == 0xD) ? EXTI15_10_IRQn : \
    (GPIO_PIN_SOURCE(GPIO) == 0xE) ? EXTI15_10_IRQn : \
                                     EXTI15_10_IRQn   \
)

#define GPIO_PORT_SOURCE(GPIO) (GPIO >> 4)
#define GPIO_PIN_SOURCE(GPIO) (GPIO & 0xF)

#define GPIO_PORT(GPIO) ((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * GPIO_PORT_SOURCE(GPIO)))
#define GPIO_PIN(GPIO) ((uint16_t)1 << GPIO_PIN_SOURCE(GPIO))

#define GPIO_PORT_RCC(GPIO) ((uint32_t)0x00000004 << GPIO_PORT_SOURCE(GPIO))

/***************************************************************************************************
 *                                      PRIVATE TYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                               PRIVATE FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                                       PRIVATE DATA
 **************************************************************************************************/

/***************************************************************************************************
 *                                       PUBLIC DATA
 **************************************************************************************************/

/***************************************************************************************************
 *                                      EXTERNAL DATA
 **************************************************************************************************/

/***************************************************************************************************
 *                              EXTERNAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                                    PRIVATE FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

#ifdef RTE_DEVICE_HAL_COMMON

void gpio_out_pp_config(const uint8_t _port_pin)
{
    GPIO_InitTypeDef GPIO_Init =
    {
        .Pin   = GPIO_PIN(_port_pin),
        .Mode  = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_FREQ_MEDIUM,
        .Pull  = GPIO_NOPULL,
    };

    HAL_GPIO_Init(GPIO_PORT(_port_pin), &GPIO_Init);
}

void gpio_out_od_config(const uint8_t _port_pin)
{
    GPIO_InitTypeDef GPIO_Init =
    {
        .Pin   = GPIO_PIN(_port_pin),
        .Mode  = GPIO_MODE_OUTPUT_OD,
        .Speed = GPIO_SPEED_FREQ_MEDIUM,
        .Pull  = GPIO_PULLUP,
    };

    HAL_GPIO_Init(GPIO_PORT(_port_pin), &GPIO_Init);
}

void gpio_in_flt_config(const uint8_t _port_pin)
{
    GPIO_InitTypeDef GPIO_Init =
    {
        .Pin   = GPIO_PIN(_port_pin),
        .Mode  = GPIO_MODE_INPUT,
        .Pull  = GPIO_NOPULL,   
    };

    HAL_GPIO_Init(GPIO_PORT(_port_pin), &GPIO_Init);
}

void gpio_in_pup_config(const uint8_t _port_pin)
{
    GPIO_InitTypeDef GPIO_Init =
    {
        .Pin   = GPIO_PIN(_port_pin),
        .Mode  = GPIO_MODE_INPUT,
        .Pull  = GPIO_PULLUP,   
    };

    HAL_GPIO_Init(GPIO_PORT(_port_pin), &GPIO_Init);
}

__INLINE void gpio_write(const uint8_t _port_pin, const bool _val)
{
    HAL_GPIO_WritePin(GPIO_PORT(_port_pin), GPIO_PIN(_port_pin), _val);
}

__INLINE bool gpio_read(const uint8_t _port_pin)
{
    return HAL_GPIO_ReadPin(GPIO_PORT(_port_pin), GPIO_PIN(_port_pin));
}

#else

__INLINE void gpio_out_pp_config(const uint8_t _port_pin)
{
    GPIO_PinConfigure(GPIO_PORT(_port_pin),
                      GPIO_PIN_SOURCE(_port_pin),
                      GPIO_OUT_PUSH_PULL,
                      GPIO_MODE_OUT2MHZ);
}

__INLINE void gpio_out_od_config(const uint8_t _port_pin)
{
    GPIO_PinConfigure(GPIO_PORT(_port_pin),
                      GPIO_PIN_SOURCE(_port_pin),
                      GPIO_OUT_OPENDRAIN,
                      GPIO_MODE_OUT2MHZ);
}

__INLINE void gpio_in_flt_config(const uint8_t _port_pin)
{
    GPIO_PinConfigure(GPIO_PORT(_port_pin),
                      GPIO_PIN_SOURCE(_port_pin),
                      GPIO_IN_FLOATING,
                      GPIO_MODE_INPUT);
}

__INLINE void gpio_in_pup_config(const uint8_t _port_pin)
{
    GPIO_PinConfigure(GPIO_PORT(_port_pin),
                      GPIO_PIN_SOURCE(_port_pin),
                      GPIO_IN_PULL_UP,
                      GPIO_MODE_INPUT);
}

__INLINE void gpio_write(const uint8_t _port_pin, const bool _val)
{
    GPIO_PinWrite(GPIO_PORT(_port_pin), GPIO_PIN_SOURCE(_port_pin), _val);
}

__INLINE bool gpio_read(const uint8_t _port_pin)
{
    return GPIO_PinRead(GPIO_PORT(_port_pin), GPIO_PIN_SOURCE(_port_pin));
}

#endif

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
