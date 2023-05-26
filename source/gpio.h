/**
 * @file       bsp_gpio.h
 * @brief      Файл макросов преобразования переменной содержащей порт и пин в формат определенный
 *             библиотекой от STM
 *
 * @author     Zelenin Alex
 * @date       2019/07/17
 */

#pragma once

/***************************************************************************************************
 *                                         INCLUDED FILES
 **************************************************************************************************/

#include "RTE_Components.h"
#include CMSIS_device_header

/***************************************************************************************************
 *                                     GLOBAL VARIABLES
 **************************************************************************************************/

/// Соответствие номера пина и номера прерывания EXTI
extern const IRQn_Type gpio_exti_irq[0x10];
/// Соответствие номера пина и номера линии EXTI
extern const uint32_t gpio_exti_line[0x10];

/***************************************************************************************************
 *                                           DEFINITIONS                                           *
 **************************************************************************************************/

/** @defgroup PORTS Псевдонимы пинов всех GPIO пинов
 *  @{
 */

#define PORT_NA  0xFF ///< Порт и пин не определены

#define PORTA_00 0x00 ///< Порт A пин 00
#define PORTA_01 0x01 ///< Порт A пин 01
#define PORTA_02 0x02 ///< Порт A пин 02
#define PORTA_03 0x03 ///< Порт A пин 03
#define PORTA_04 0x04 ///< Порт A пин 04
#define PORTA_05 0x05 ///< Порт A пин 05
#define PORTA_06 0x06 ///< Порт A пин 06
#define PORTA_07 0x07 ///< Порт A пин 07
#define PORTA_08 0x08 ///< Порт A пин 08
#define PORTA_09 0x09 ///< Порт A пин 09
#define PORTA_10 0x0A ///< Порт A пин 10
#define PORTA_11 0x0B ///< Порт A пин 11
#define PORTA_12 0x0C ///< Порт A пин 12
#define PORTA_13 0x0D ///< Порт A пин 13
#define PORTA_14 0x0E ///< Порт A пин 14
#define PORTA_15 0x0F ///< Порт A пин 15

#define PORTB_00 0x10 ///< Порт B пин 00
#define PORTB_01 0x11 ///< Порт B пин 01
#define PORTB_02 0x12 ///< Порт B пин 02
#define PORTB_03 0x13 ///< Порт B пин 03
#define PORTB_04 0x14 ///< Порт B пин 04
#define PORTB_05 0x15 ///< Порт B пин 05
#define PORTB_06 0x16 ///< Порт B пин 06
#define PORTB_07 0x17 ///< Порт B пин 07
#define PORTB_08 0x18 ///< Порт B пин 08
#define PORTB_09 0x19 ///< Порт B пин 09
#define PORTB_10 0x1A ///< Порт B пин 10
#define PORTB_11 0x1B ///< Порт B пин 11
#define PORTB_12 0x1C ///< Порт B пин 12
#define PORTB_13 0x1D ///< Порт B пин 13
#define PORTB_14 0x1E ///< Порт B пин 14
#define PORTB_15 0x1F ///< Порт B пин 15

#define PORTC_00 0x20 ///< Порт C пин 00
#define PORTC_01 0x21 ///< Порт C пин 01
#define PORTC_02 0x22 ///< Порт C пин 02
#define PORTC_03 0x23 ///< Порт C пин 03
#define PORTC_04 0x24 ///< Порт C пин 04
#define PORTC_05 0x25 ///< Порт C пин 05
#define PORTC_06 0x26 ///< Порт C пин 06
#define PORTC_07 0x27 ///< Порт C пин 07
#define PORTC_08 0x28 ///< Порт C пин 08
#define PORTC_09 0x29 ///< Порт C пин 09
#define PORTC_10 0x2A ///< Порт C пин 10
#define PORTC_11 0x2B ///< Порт C пин 11
#define PORTC_12 0x2C ///< Порт C пин 12
#define PORTC_13 0x2D ///< Порт C пин 13
#define PORTC_14 0x2E ///< Порт C пин 14
#define PORTC_15 0x2F ///< Порт C пин 15

#define PORTD_00 0x30 ///< Порт D пин 00
#define PORTD_01 0x31 ///< Порт D пин 01
#define PORTD_02 0x32 ///< Порт D пин 02
#define PORTD_03 0x33 ///< Порт D пин 03
#define PORTD_04 0x34 ///< Порт D пин 04
#define PORTD_05 0x35 ///< Порт D пин 05
#define PORTD_06 0x36 ///< Порт D пин 06
#define PORTD_07 0x37 ///< Порт D пин 07
#define PORTD_08 0x38 ///< Порт D пин 08
#define PORTD_09 0x39 ///< Порт D пин 09
#define PORTD_10 0x3A ///< Порт D пин 10
#define PORTD_11 0x3B ///< Порт D пин 11
#define PORTD_12 0x3C ///< Порт D пин 12
#define PORTD_13 0x3D ///< Порт D пин 13
#define PORTD_14 0x3E ///< Порт D пин 14
#define PORTD_15 0x3F ///< Порт D пин 15

#define PORTE_00 0x40 ///< Порт E пин 00
#define PORTE_01 0x41 ///< Порт E пин 01
#define PORTE_02 0x42 ///< Порт E пин 02
#define PORTE_03 0x43 ///< Порт E пин 03
#define PORTE_04 0x44 ///< Порт E пин 04
#define PORTE_05 0x45 ///< Порт E пин 05
#define PORTE_06 0x46 ///< Порт E пин 06
#define PORTE_07 0x47 ///< Порт E пин 07
#define PORTE_08 0x48 ///< Порт E пин 08
#define PORTE_09 0x49 ///< Порт E пин 09
#define PORTE_10 0x4A ///< Порт E пин 10
#define PORTE_11 0x4B ///< Порт E пин 11
#define PORTE_12 0x4C ///< Порт E пин 12
#define PORTE_13 0x4D ///< Порт E пин 13
#define PORTE_14 0x4E ///< Порт E пин 14
#define PORTE_15 0x4F ///< Порт E пин 15

#define PORTF_00 0x50 ///< Порт F пин 00
#define PORTF_01 0x51 ///< Порт F пин 01
#define PORTF_02 0x52 ///< Порт F пин 02
#define PORTF_03 0x53 ///< Порт F пин 03
#define PORTF_04 0x54 ///< Порт F пин 04
#define PORTF_05 0x55 ///< Порт F пин 05
#define PORTF_06 0x56 ///< Порт F пин 06
#define PORTF_07 0x57 ///< Порт F пин 07
#define PORTF_08 0x58 ///< Порт F пин 08
#define PORTF_09 0x59 ///< Порт F пин 09
#define PORTF_10 0x5A ///< Порт F пин 10
#define PORTF_11 0x5B ///< Порт F пин 11
#define PORTF_12 0x5C ///< Порт F пин 12
#define PORTF_13 0x5D ///< Порт F пин 13
#define PORTF_14 0x5E ///< Порт F пин 14
#define PORTF_15 0x5F ///< Порт F пин 15

#define PORTG_00 0x60 ///< Порт G пин 00
#define PORTG_01 0x61 ///< Порт G пин 01
#define PORTG_02 0x62 ///< Порт G пин 02
#define PORTG_03 0x63 ///< Порт G пин 03
#define PORTG_04 0x64 ///< Порт G пин 04
#define PORTG_05 0x65 ///< Порт G пин 05
#define PORTG_06 0x66 ///< Порт G пин 06
#define PORTG_07 0x67 ///< Порт G пин 07
#define PORTG_08 0x68 ///< Порт G пин 08
#define PORTG_09 0x69 ///< Порт G пин 09
#define PORTG_10 0x6A ///< Порт G пин 10
#define PORTG_11 0x6B ///< Порт G пин 11
#define PORTG_12 0x6C ///< Порт G пин 12
#define PORTG_13 0x6D ///< Порт G пин 13
#define PORTG_14 0x6E ///< Порт G пин 14
#define PORTG_15 0x6F ///< Порт G пин 15

/** @}*/  // PORTS

/**
 * @brief Получить номер порта
 * @param _GPIO GPIO пин
 * @return номер порта
 */
#define GPIO_PORT_SOURCE(_GPIO) (_GPIO >> 4)

/**
 * @brief Получить номер пина
 * @param _GPIO GPIO пин
 * @return номер пина
 */
#define GPIO_PIN_SOURCE(_GPIO) (_GPIO & 0xF)

/**
 * @brief Получить адрес порта
 * @param _GPIO GPIO пин
 * @return адрес порта
 */
#define GPIO_PORT(_GPIO) ((GPIO_TypeDef *)(GPIOA_BASE + 0x400 * GPIO_PORT_SOURCE(_GPIO)))

/**
 * @brief Получить позицию пина в регистре порта
 * @param _GPIO GPIO пин
 * @return позиция пина в регистре порта
 */
#define GPIO_PIN(_GPIO) ((uint16_t)1 << GPIO_PIN_SOURCE(_GPIO))

/**
 * @brief Сбросить пин в "0"
 * @param _GPIO GPIO пин
 */
#define GPIO_RESET(_GPIO) HAL_GPIO_WritePin(GPIO_PORT(_GPIO), GPIO_PIN(_GPIO), GPIO_PIN_RESET)

/**
 * @brief Установить пин в "1"
 * @param _GPIO GPIO пин
 */
#define GPIO_SET(_GPIO)   HAL_GPIO_WritePin(GPIO_PORT(_GPIO), GPIO_PIN(_GPIO), GPIO_PIN_SET)

/**
 * @brief Прочитать состояние пина
 * @param _GPIO GPIO пин
 *
 * @return GPIO_PIN_SET - пин установлен в "1"
 * @return GPIO_PIN_RESET - пин сброшен в "0"
 */
#define GPIO_READ(_GPIO)  HAL_GPIO_ReadPin(GPIO_PORT(_GPIO), GPIO_PIN(_GPIO))

/**
 * @brief Включение тактирования периферии порта
 * @param _GPIO GPIO пин
 */
#define HAL_RCC_GPIO_CLK_ENABLE(_GPIO)                  \
    do {                                                    \
        __IO uint32_t tmpreg = 0x00U;                       \
        uint32_t rcc_bit = (1 << GPIO_PORT_SOURCE(_GPIO));  \
        SET_BIT(RCC->AHB1ENR, rcc_bit);                     \
        /* Delay after an RCC peripheral clock enabling  */ \
        tmpreg = READ_BIT(RCC->AHB1ENR, rcc_bit);           \
        UNUSED(tmpreg);                                     \
    } while(0U)

typedef enum
{
    GPIO_MODE_PP = 0,
    GPIO_MODE_OD = 1,
    GPIO_MODE_PU = 2,
} GPIO_MODE_t;

typedef struct
{
    uint32_t pin      :8;
    uint32_t mode     :8;
    uint32_t off_state:8;
    uint32_t state    :8;
} gpio_t;

#define LOW 0
#define HI  1

#define OFF 0
#define ON  1

void gpio_set(gpio_t arg);

/***************************************************************************************************
 *                                        END OF FILE
 **************************************************************************************************/
