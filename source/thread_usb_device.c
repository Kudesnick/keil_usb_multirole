/***************************************************************************************************
 *   Project:       usb mrd
 *   Author:        Stulov Tikhon
 ***************************************************************************************************
 *   Distribution:  
 *
 ***************************************************************************************************
 *   MCU Family:    STM32F
 *   Compiler:      ARMCC
 ***************************************************************************************************
 *   File:          thread_usb_host.c
 *   Description:   USB host thread
 *
 ***************************************************************************************************
 *   History:       22.09.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include <stdio.h>
#include <stdint.h>

#include "rl_usb.h"
#include "err_strings.h"
#include "thread_usb_device.h"

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

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

void thread_usb_device (void)
{
    
    uint8_t buf[1];

    volatile usbStatus usb_connect_status;
    
    usb_connect_status = USBD_Connect(0); /* USB Device 0 Connect */
    printf("<USBD> device connect status: %s\r\n", err_str_usb_status(usb_connect_status)); 
    if (usb_connect_status != usbOK)
    {
        osThreadExit();
    }
    
    osDelay(500);

    if (USBD_Configured(0))
    {
        printf("<USBD> Device is configured.\r\n");
    }
    
    for (;USBD_Configured(0); osDelay(100))
    {   /* Loop forever */
        USBD_HID_GetReportTrigger(0, 0, &buf[0], 1);
        USBD_HID_GetReportTrigger(1, 0, &buf[0], 1);
    }
    
    printf("<USBD> Device is not configured.\r\n");
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
