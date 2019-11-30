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
 *   File:          hid_descriptor_decode.c
 *   Description:   decode HID descriptor for device detect and debug
 *
 ***************************************************************************************************
 *   History:       24.09.2019 - file created
 *
 **************************************************************************************************/

#pragma once

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include <stdint.h>

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

// Main Items
#define HID_Field_Input           0x81U                 
#define HID_Field_Output          0x91U                 
#define HID_Field_Feature         0xB1U                 
#define HID_Field_Collection      0xA1U                 
#define HID_Field_EndCollection   0xC0U                   
// Global Items                
#define HID_Field_UsagePage       0x05U                 
#define HID_Field_UsagePageVendor 0x06U                 
#define HID_Field_LogicalMin      0x15U                 
#define HID_Field_LogicalMinS     0x16U                 
#define HID_Field_LogicalMinL     0x17U                 
#define HID_Field_LogicalMax      0x25U                 
#define HID_Field_LogicalMaxS     0x26U                 
#define HID_Field_LogicalMaxL     0x27U                 
#define HID_Field_PhysicalMin     0x35U                 
#define HID_Field_PhysicalMinS    0x36U                 
#define HID_Field_PhysicalMinL    0x37U                 
#define HID_Field_PhysicalMax     0x45U                 
#define HID_Field_PhysicalMaxS    0x46U                 
#define HID_Field_PhysicalMaxL    0x47U                 
#define HID_Field_UnitExponent    0x55U                 
#define HID_Field_Unit            0x65U                 
#define HID_Field_UnitS           0x66U                 
#define HID_Field_UnitL           0x67U                 
#define HID_Field_ReportSize      0x75U                 
#define HID_Field_ReportSizeS     0x76U                 
#define HID_Field_ReportSizeL     0x77U                 
#define HID_Field_ReportID        0x85U                 
#define HID_Field_ReportCount     0x95U                 
#define HID_Field_ReportCountS    0x96U                 
#define HID_Field_ReportCountL    0x97U                 
#define HID_Field_Push            0xA4U                    
#define HID_Field_Pop             0xB4U                    
// Local Items                 
#define HID_Field_Usage           0x09U                 
#define HID_Field_UsageMin        0x19U                 
#define HID_Field_UsageMinS       0x1AU                 
#define HID_Field_UsageMinL       0x1BU                 
#define HID_Field_UsageMax        0x29U                 
#define HID_Field_UsageMaxS       0x2AU                 
#define HID_Field_UsageMaxL       0x2BU                 


/***************************************************************************************************
 *                                      PUBLIC TYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                                     GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 *                                PUBLIC FUNCTION PROTOTYPES
 **************************************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

void hid_desc_print(const uint8_t * _desc, uint32_t len);

uint8_t hid_desc_get_generic(const uint8_t * _desc, uint32_t len);

void hid_desc_usage_print(uint8_t _usage);

#ifdef __cplusplus
} // extern "C"
#endif

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
