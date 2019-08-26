/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2016 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    VirtualCOM.c
 * Purpose: USB Device Virtual COM Port demonstration
 *----------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "rl_usb.h"

int main (void) {

  USBD_Initialize         (0U);         // USB Device 0 Initialization
  USBD_Connect            (0U);         // USB Device 0 Connect

  while (1) {
    osSignalWait (0U, osWaitForever);
  }  
}
