
#pragma once

// Compass P9 (stm32f103) GSM+GNSS USART2
#define UART_PORT 2

#define LED_PIN             PORTB_13, GPIO_MODE_OD, HI // green
#define PWR_PIN             PORTD_02, GPIO_MODE_PP, LOW
#define PWRKEY_PIN          PORTC_07, GPIO_MODE_PP, LOW // pulse
#define RESET_PIN           PORTB_06, GPIO_MODE_PP, LOW
// #define GNSS_PWR_PIN
// #define GNSS_RESET_PIN
// #define GNSS_STB_PIN
#define GNSS_ANT_PIN        PORTB_07, GPIO_MODE_PP, LOW // internal
// #define NRF_RES_PIN
