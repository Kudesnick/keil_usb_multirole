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
#include "hid_descriptor_decode.h"

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

/// \brief Callback function called for parsing of the Human Interface Device report descriptor
/// \details see rl_usb.h
/// \param[in]     instance             instance index.
/// \param[in]     ptr_hid_report_desc  pointer to HID report descriptor.
/// \param[in]     len                  length of HID report descriptor.
/// \return                             none.
void USBH_HID_ParseReportDescriptor (uint8_t instance, const uint8_t *ptr_hid_report_desc, uint32_t len)
{
    printf("Hid descriptor parse:\r\n");
    
    hid_desc_print(ptr_hid_report_desc, len);
    
    uint8_t id = hid_desc_get_generic(ptr_hid_report_desc, len);
    while(id)
    {
        hid_desc_usage_print(id);
        id = hid_desc_get_generic(NULL, 0);
    }
};

static __INLINE void usb_hid(void)
{
    // USB HID device
    usbStatus hid_status = USBH_HID_GetDeviceStatus (0U); // Get HID device status
    
    if (hid_status == usbOK)
    {
        printf ("USBH HID connected.\r\n");
        
        for (; hid_status == usbOK; hid_status = USBH_HID_GetDeviceStatus(0U), osDelay(10U))
        {
//          static uint8_t prev_btn;
//          usbHID_MouseState mouse_state;
//          
//            usbStatus mouse_status = USBH_HID_GetMouseState(0U, &mouse_state);
//            
//            if (mouse_status == usbOK)
//            {
//                if (prev_btn != mouse_state.button)
//                {
//                    printf("Mouse btn code: 0x%02X\r\n", mouse_state.button);
//                
//                    prev_btn = mouse_state.button;
//                }
//            }
            
            int ch = USBH_HID_GetKeyboardKey (0U);// Get pressed key
            
            if (ch != -1)                    // If valid key value
            {
                printf("Key code: 0x%08X\r\n", ch);
            }
        }
        
        printf ("USBH HID disconnected.\r\n");
    }
}

static __INLINE void usb_masstorage(void)
{
    // USB mass storage device
    int32_t msc_status = USBH_MSC_DriveGetMediaStatus("U0:");  // Get MSC device status
        
    if (msc_status == USBH_MSC_OK) 
    {
        printf("USBH mass storage connected.\r\n");
        
        for (bool first_iter = true;
             msc_status == USBH_MSC_OK;
             msc_status = USBH_MSC_DriveGetMediaStatus("U0:"), osDelay(1000U)
             )
        {
            if (first_iter) // If stick was not connected previously
            {
                msc_status = USBH_MSC_DriveMount("U0:");
                printf("USBH_MSC_DriveMount U0: %s\r\n", err_str_usbh_msc_status(msc_status));
                if (msc_status != USBH_MSC_OK) continue; // Handle U0: mount failure

                FILE * f = fopen ("Test.txt", "w"); // Open/create file for writing
                printf("Fopen 'Test.txt': %s\r\n", (f != NULL) ? "Ok" : "failure");
                if (f == NULL) continue; // Handle file opening/creation failure

                fprintf (f, "USB Host Mass Storage!\n");
                fclose (f); // Close file
                
                msc_status = USBH_MSC_DriveUnmount ("U0:");
                printf("USBH_MSC_DriveUnmount U0: %s\r\n", err_str_usbh_msc_status(msc_status));
                if (msc_status != USBH_MSC_OK) continue; // Handle U0: dismount failure
                
                first_iter = false;
            }
        }
        printf("USBH mass storage disconnected.\r\n");
    }
}

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

void thread_usb_host(void *arg)
{
    (void)arg;
    
    for (;;)
    {
        usb_hid();
        usb_masstorage();
    }
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
