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

#include "cmsis_os.h"
#include "rl_usb.h"
#include "rl_fs.h"

#include "USBH_MSC.h"
#include "err_strings.h"

#include "thread_usb_host.h"

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

void thread_usb_host(void *arg) {
    int32_t   msc_status;                 // MSC status
    FILE     *f;                          // Pointer to stream object
    uint8_t   con_mcs = 0U;               // Connection status of MSC(s)

    usbStatus hid_status;                 // HID status
    uint8_t   con_hid = 0U;               // Connection status of MSC(s)
    int       ch;                         // Character
    
    (void)arg;
    
    for (;;)
    {
        // USB HID device
        hid_status = USBH_HID_GetDeviceStatus (0U); // Get HID device status
        
        if (hid_status == usbOK)
        {
            if (con_hid == 0U)                   // If keyboard was not connected previously
            {
                con_hid = 1U;                       // Keyboard got connected
                printf ("Keyboard connected.\r\n");
            }
        }
        
        while (con_hid != 0U)                     // If keyboard is active
        {
            ch = USBH_HID_GetKeyboardKey (0U);// Get pressed key
            if (ch != -1)                    // If valid key value
            {
                printf("Key code: 0x%08X\r\n", ch);
                
                if ((ch & 0x10000) != 0)            // Handle non-ASCII translated keys (Keypad 0 .. 9)
                {                                   // Bit  16:    non-ASCII bit (0 = ASCII, 1 = not ASCII)
                                                    // Bits 15..8: modifiers (SHIFT, ALT, CTRL, GUI)
                                                    // Bits  7..0: ASCII or HID key Usage ID if not ASCII
                                                    // HID Usage ID values can be found in following link:
                                                    // http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
                    ch &= 0xFF;                   // Remove non-ASCII bit and modifiers
                    if ((ch>=0x59)&&(ch<=0x61))  // Keypad 1 .. 9 key convert to
                    {
                        ch = (ch - 0x59) + '1';     // ASCII  1 .. 9
                    }
                    else if (ch == 0x62)       // Keypad 0 key convert to
                    {
                        ch = '0';                   // ASCII  0
                    } 
                    else                       // If not Keypad 0 .. 9
                    {
                        ch = -1;                    // invalidate the key
                    }
                }
                if ((ch > 0) && (ch < 128))    // Output ASCII 0 .. 127 range
                {
                    putchar (ch);
                    fflush (stdout);
                }
            }
            
            if (USBH_HID_GetDeviceStatus(0U) != usbOK) // Get HID device status
            {
                con_hid = 0U;                       // Keyboard got disconnected
                printf ("Keyboard disconnected.\r\n");
            }
            
            osDelay(10U);
        }
        
        // USB mass storage device
        msc_status = USBH_MSC_DriveGetMediaStatus ("U0:");  // Get MSC device status
        
        if (msc_status == USBH_MSC_OK) 
        {            
            if (con_mcs == 0U)                   // If stick was not connected previously
            {
                printf("Mass storage connected.\r\n");

                con_mcs = 1U;                       // Stick got connected
                msc_status = USBH_MSC_DriveMount ("U0:");
                if (msc_status != USBH_MSC_OK) 
                {
                    continue;                     // Handle U0: mount failure
                }
                f = fopen ("Test.txt", "w");    // Open/create file for writing
                if (f == NULL) 
                {
                    continue;                     // Handle file opening/creation failure
                }
                fprintf (f, "USB Host Mass Storage!\n");
                fclose (f);                     // Close file
                
                msc_status = USBH_MSC_DriveUnmount ("U0:");
                
                if (msc_status != USBH_MSC_OK) 
                {
                    continue;                     // Handle U0: dismount failure
                }
            }
        }
        else 
        {
            if (con_mcs == 1U)                   // If stick was connected previously
            {
                printf("Mass storage disconnected.\r\n");
                
                con_mcs = 0U;                       // Stick got disconnected
            }
        }
        osDelay(100U);
    }
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
