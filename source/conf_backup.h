
#pragma once

// Black pill (stm32f411) J-Link USART2
#define LED_PIN             PORTC_13, GPIO_MODE_OD, HI
// #define PWR_PIN
// #define PWRKEY_PIN
// #define RESET_PIN
// #define GNSS_EN_PIN
// #define GNSS_ANT_PIN

// Compass P8 (stm32f103) GSM USART2, GNSS USART3
#define LED_PIN             PORTB_13, GPIO_MODE_OD, HI // green
#define PWR_PIN             PORTD_02, GPIO_MODE_PP, LO
#define PWRKEY_PIN          PORTC_07, GPIO_MODE_OD, HI // pulse
// #define RESET_PIN
#define GNSS_PWR_PIN        PORTB_06, GPIO_MODE_PP, LO
// #define GNSS_RESET_PIN
// #define GNSS_STB_PIN
#define GNSS_ANT_PIN        PORTB_07, GPIO_MODE_PP, LO // internal
// #define NRF_RES_PIN

// Compass P9 (stm32f103) GSM+GNSS USART2
#define LED_PIN             PORTB_13, GPIO_MODE_OD, HI // green
#define PWR_PIN             PORTD_02, GPIO_MODE_PP, LO
#define PWRKEY_PIN          PORTC_07, GPIO_MODE_PP, LO // pulse
#define RESET_PIN           PORTB_06, GPIO_MODE_PP, LO
// #define GNSS_PWR_PIN
// #define GNSS_RESET_PIN
// #define GNSS_STB_PIN
#define GNSS_ANT_PIN        PORTB_07, GPIO_MODE_PP, LO // internal
// #define NRF_RES_PIN

// Igla-x M95 (stm32f413) nRF USART2, GSM USART3, GNSS UART4
#define LED_PIN             PORTA_10, GPIO_MODE_OD, HI // green
// #define PWR_PIN             PORTB_07, GPIO_MODE_PP, LO // not used
#define PWRKEY_PIN          PORTC_08, GPIO_MODE_PP, LO // pulse
#define RESET_PIN           PORTD_03, GPIO_MODE_PP, LO
#define GNSS_PWR_PIN        PORTB_00, GPIO_MODE_PP, LO
#define GNSS_RESET_PIN      PORTC_09, GPIO_MODE_PP, LO
#define GNSS_STB_PIN        PORTD_07, GPIO_MODE_PP, LO
#define GNSS_ANT_PIN        PORTB_07, GPIO_MODE_PP, LO // internal
#define NRF_RES_PIN         PORTC_10, GPIO_MODE_OD, HI
