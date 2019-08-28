#include <stdio.h>

#include "RTE_Components.h"
#include CMSIS_device_header

#include "cmsis_os.h"
#include "rl_usb.h"

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device Human Interface Device example program
 *----------------------------------------------------------------------------*/
void usb_handle (void const *argument)
{
    uint8_t buf[1];

    USBD_Initialize    (0);               /* USB Device 0 Initialization        */
    USBD_Connect       (0);               /* USB Device 0 Connect               */

    for (;;)
    {                           /* Loop forever                       */
        USBD_HID_GetReportTrigger(0, 0, &buf[0], 1);
        osDelay(100);                       /* 100 ms delay for sampling buttons  */
    }
}

osThreadDef(usb_handle, osPriorityNormal, 1, 0);

int main(void)
{
    // System Initialization
    SystemCoreClockUpdate();

    // printf("main runing..\r\n");
    
    osKernelInitialize();
    osThreadCreate(osThread(usb_handle), NULL);
    osKernelStart();
    
    for (;;);
}
