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
 *   File:          thread_usb_manager.c
 *   Description:   management multirole USB device
 *
 ***************************************************************************************************
 *   History:       22.09.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/


#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cpp_os.h"
#include "err_strings.h"
#include "thread_usb_device.h"
#include "thread_usb_host.h"
#include "pin_config.h"

using namespace std;

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

#define PIN_DISCONNECT PIN_USB_DB

#define USBM_STK_SZ (2048U)

#define TERMINATE_TOUT 2000

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

class: public cpp_os_thread<USBM_STK_SZ>
{
private:
    void(* device_thread)(void) = thread_usb_device;
    void(* host_thread)(void) = thread_usb_host;
    
#ifndef PIN_USB_ID
    bool id_simulate = true;
    
    bool otg_detect(void)
    {
        id_simulate != id_simulate;
        
        return id_simulate;
    }
#else
    bool otg_detect(void)
    {
        gpio_in_pup_config(PIN_USB_ID);
        
        return (gpio_read(PIN_USB_ID) == false);
    }
#endif

    /// Emulate USB disconnect (USB_DP)
    void usb_disconnect_emulate(void)
    {
        gpio_out_od_config(PIN_DISCONNECT);
        gpio_write(PIN_DISCONNECT, false);
        cpp_os::delay(10);
    }

    void thread_func(void)
    {
        for(;; osDelay(TERMINATE_TOUT))
        {
            usbStatus usb_status;
            
            // Init usb host role
            if (otg_detect())
            {
                usb_status = USBH_Initialize (0U);
                printf("<USBH> Initialize: %s\r\n", err_str_usb_status(usb_status));
                
                if (usb_status == usbOK)
                {
                    uint32_t time = osKernelGetTickCount();
    
                    while (osKernelGetTickCount() - time < 4000)
                    {
                        host_thread();
                    }
                }
                
                usb_status = USBH_Uninitialize(0);
                printf("<USBH> Uninitialize: %s\r\n", err_str_usb_status(usb_status));
            }
            
            // init usbdevice device role
            else
            {
                usb_status = USBD_Initialize (0U);
                printf("<USBD> Initialize: %s\r\n", err_str_usb_status(usb_status));
                
                if (usb_status == usbOK)
                {
                    device_thread();
                }
                
                usb_status = USBD_Uninitialize(0);
                printf("<USBD> Uninitialize: %s\r\n", err_str_usb_status(USBD_Uninitialize(0)));
            }
        }
    };
    
public:
    using cpp_os_thread::cpp_os_thread;
    
} thread_usb_manager = {};

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
