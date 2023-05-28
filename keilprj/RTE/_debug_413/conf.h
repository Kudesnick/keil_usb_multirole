
#pragma once

// Igla-x M95 (stm32f413) nRF USART2, GSM USART3, GNSS UART4
#define LED_PIN             PORTA_10, GPIO_MODE_OD, HI // green
// #define PWR_PIN             PORTB_07, GPIO_MODE_PP, LOW // not used
#define PWRKEY_PIN          PORTC_08, GPIO_MODE_PP, LOW // pulse
#define RESET_PIN           PORTD_03, GPIO_MODE_PP, LOW
#define GNSS_PWR_PIN        PORTB_00, GPIO_MODE_PP, LOW
#define GNSS_RESET_PIN      PORTC_09, GPIO_MODE_PP, LOW
#define GNSS_STB_PIN        PORTD_07, GPIO_MODE_PP, LOW
#define GNSS_ANT_PIN        PORTB_07, GPIO_MODE_PP, LOW // internal
#define NRF_RES_PIN         PORTC_10, GPIO_MODE_OD, HI
