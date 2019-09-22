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
 *   File:          err_strings.c
 *   Description:   convert error code to string for print in debug console
 *
 ***************************************************************************************************
 *   History:       22.09.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include "err_strings.h"
#include "USBH_MSC.h"

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

const char * err_str_usb_status(const usbStatus _state)
{
    switch(_state)
    {
        case usbOK              : return "Ok";

        case usbTimeout         : return "Function completed; time-out occurred";
        case usbInvalidParameter: return "Invalid Parameter error: a mandatory parameter was missing or specified an incorrect object";

        case usbThreadError     : return "CMSIS-RTOS Thread creation/termination failed";
        case usbTimerError      : return "CMSIS-RTOS Timer creation/deletion failed";
        case usbSemaphoreError  : return "CMSIS-RTOS Semaphore creation failed";
        case usbMutexError      : return "CMSIS-RTOS Mutex creation failed";

        case usbControllerError : return "Controller does not exist";
        case usbDeviceError     : return "Device does not exist";
        case usbDriverError     : return "Driver function produced error";
        case usbDriverBusy      : return "Driver function is busy";
        case usbMemoryError     : return "Memory management function produced error";
        case usbNotConfigured   : return "Device is not configured (is connected)";
        case usbClassErrorADC   : return "Audio Device Class (ADC) error (no device or device produced error)";
        case usbClassErrorCDC   : return "Communication Device Class (CDC) error (no device or device produced error)";
        case usbClassErrorHID   : return "Human Interface Device (HID) error (no device or device produced error)";
        case usbClassErrorMSC   : return "Mass Storage Device (MSC) error (no device or device produced error)";
        case usbClassErrorCustom: return "Custom device Class (Class) error (no device or device produced error)";
        case usbUnsupportedClass: return "Unsupported Class";

        case usbTransferStall   : return "Transfer handshake was stall";
        case usbTransferError   : return "Transfer error";

        case usbUnknownError    : return "Unspecified USB error";
        
        default: return "Unkhown USB error code";
    }
};

const char * err_str_usbh_msc_status(const int32_t _state)
{
    switch(_state)
    {
        case USBH_MSC_OK          : return "Ok";
        case USBH_MSC_ERROR       : return "Unspecified error";
        case USBH_MSC_ERROR_DRIVE : return "USB storage device not connected";
        case USBH_MSC_ERROR_FORMAT: return "USB drive mounted, but unformatted";
        default: return "Unkhown USB storage device error code";
    }
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
