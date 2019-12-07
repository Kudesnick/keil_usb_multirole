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
 *   File:          pin_config.h
 *   Description:  
 *
 ***************************************************************************************************
 *   History:       07.12.2019 - file created
 *
 **************************************************************************************************/
 
#pragma once

/*

Control pannel pinout

NC   + +  NC
NC   + +  NC
NC   + +  NC
RST  + +  NC
SET  + +  NC
MID  + +  NC
RGHT + +  NC
LFT  + +  NC
DWN  + + SCK
UP   + + SDA
GND  + + GND
MISO + + CLK
MOSI + + CS1
Vcc  + + CS2

"Nucleo shield (CN12 for 'nucleo 144' or CN10 for 'nucleo 64')

     | C9  x +   C8 |
SCK  | B8  + +   C6 |
SDA  | B9  + +   C5 |
Vcc  | Av  + + U5.5 |
GND  | GND + +   D8 |
     | A5  + +  A12 | MISO
     | A6  + +  A11 |
     | A7  + +  B12 | RST
     | B6  + +  B11 | SET
     | C7  + +  GND |
CLK  | A9  + +   B2 | MID
     | A8  + +   B1 | RGHT
     | B10 + +  B15 | LFT
     | B4  + +  B14 | DWN
     | B5  + +  B13 | UP
     | B3  + + AGND |
MOSI | A10 + +   C4 |
CS1  | A2  + +   F5 |
CS2  | A3  + +   F4 |

"Black Pill" (We Act) shield

 RST - B12 + + 5.0
 SET - B13 + + GND - GND
 MID - B14 + + 3.3 - Vcc
RGHT - B15 + + B10 - LFT
       A8  + +  B2 - BOOT1*
       A9  + +  B1 - DWN
       A10 + +  B0 - UP
DN** - A11 + +  A7 - MOSI
DP** - A12 + +  A6 - MISO
       A15 + +  A5 - CLK
       B3  + +  A4 - CS0***
       B4  + +  A3 - CS1
       B5  + +  A2 - CS2
 SCK - B6  + +  A1
 SDA - B7  + +  A0 - BTN****
       B8  + +   R
       B9  + + C15
       5.0 + + C14
       GND + + C13 - LED*****
       3.3 + +  VB

*     - boot config pin (hardware pull down)
**    - usb connector
***   - chip select pin for onboard SPI Flash memory
****  - onboard btn (push to ground)
***** - onboard led indicator (cathode)

*/
 
/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include "gpio.h"

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

#if defined(BRD_NUC413)

#define PIN_BTN_RST  PORTB_12
#define PIN_BTN_SET  PORTB_11
#define PIN_BTN_MID  PORTB_02
#define PIN_BTN_RGHT PORTB_01
#define PIN_BTN_LFT  PORTB_15
#define PIN_BTN_DWN  PORTB_14
#define PIN_BTN_UP   PORTB_13

//#define PIN_SPI_CS0  
#define PIN_SPI_CS1  PORTA_02
#define PIN_SPI_CS2  PORTA_03

#define PIN_USB_DB   PORTA_12
#define PIN_USB_ID   PORTA_10

#elif defined(BRD_NUC103)

#define PIN_BTN_RST  PORTB_12
#define PIN_BTN_SET  PORTB_11
#define PIN_BTN_MID  PORTB_02
#define PIN_BTN_RGHT PORTB_01
#define PIN_BTN_LFT  PORTB_15
#define PIN_BTN_DWN  PORTB_14
#define PIN_BTN_UP   PORTB_13

//#define PIN_SPI_CS0  
#define PIN_SPI_CS1  PORTA_02
#define PIN_SPI_CS2  PORTA_03

#define PIN_USB_DB   PORTA_12
//#define PIN_USB_ID

#elif defined(BRD_NUC476)

#define PIN_BTN_RST  PORTB_12
#define PIN_BTN_SET  PORTB_11
#define PIN_BTN_MID  PORTB_02
#define PIN_BTN_RGHT PORTB_01
#define PIN_BTN_LFT  PORTB_15
#define PIN_BTN_DWN  PORTB_14
#define PIN_BTN_UP   PORTB_13

//#define PIN_SPI_CS0  
#define PIN_SPI_CS1  PORTA_02
#define PIN_SPI_CS2  PORTA_03

#define PIN_USB_DB   PORTA_12
//#define PIN_USB_ID

#elif defined(BRD_PIL411)

#define PIN_BTN_RST  PORTB_12
#define PIN_BTN_SET  PORTB_13
#define PIN_BTN_MID  PORTB_14
#define PIN_BTN_RGHT PORTB_15
#define PIN_BTN_LFT  PORTB_10
#define PIN_BTN_DWN  PORTB_01
#define PIN_BTN_UP   PORTB_00

#define PIN_SPI_CS0  PORTA_04
#define PIN_SPI_CS1  PORTA_03
#define PIN_SPI_CS2  PORTA_02

#define PIN_USB_DB   PORTA_12
//#define PIN_USB_ID

#elif defined(BRD_PIL103)

#define PIN_BTN_RST  PORTB_12
#define PIN_BTN_SET  PORTB_13
#define PIN_BTN_MID  PORTB_14
#define PIN_BTN_RGHT PORTB_15
#define PIN_BTN_LFT  PORTB_10
#define PIN_BTN_DWN  PORTB_01
#define PIN_BTN_UP   PORTB_00

#define PIN_SPI_CS0  PORTA_04
#define PIN_SPI_CS1  PORTA_03
#define PIN_SPI_CS2  PORTA_02

#define PIN_USB_DB   PORTA_12
//#define PIN_USB_ID

#else
    #error Uncnown device board
#endif

/***************************************************************************************************
 *                                      PUBLIC TYPES
 **************************************************************************************************/
 
/***************************************************************************************************
 *                                     GLOBAL VARIABLES
 **************************************************************************************************/
 
/***************************************************************************************************
 *                                PUBLIC FUNCTION PROTOTYPES
 **************************************************************************************************/
 
/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/