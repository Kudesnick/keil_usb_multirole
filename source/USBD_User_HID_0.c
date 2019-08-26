/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device:HID
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    USBD_User_HID_Mouse_n.c
 * Purpose: USB Device Human Interface Device class (HID) User module
 * Rev.:    V6.3.3
 *----------------------------------------------------------------------------*/
 
#include <stdint.h>
 
#include "rl_usb.h"
#include "usb_hid.h"
 
// User Provided HID Report Descriptor
// for standard mouse (size of this descriptor is 52 bytes)
extern 
const uint8_t usbd_hid0_report_descriptor[];
const uint8_t usbd_hid0_report_descriptor[] = {

    // Keyboard Top-Level Collection (TLC)

    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION

    // Mouse TLC

    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};
/*    HID_UsagePage(HID_USAGE_PAGE_GENERIC),
  HID_Usage(HID_USAGE_GENERIC_KEYBOARD),
  HID_Collection(HID_Application),
    HID_UsagePage(HID_USAGE_PAGE_KEYBOARD),
    HID_UsageMin(HID_USAGE_KEYBOARD_LCTRL),
    HID_UsageMax(HID_USAGE_KEYBOARD_RGUI),
    HID_LogicalMin(0),
    HID_LogicalMax(1),
    HID_ReportSize(1),
    HID_ReportCount(8),
    HID_Input(HID_Data | HID_Variable | HID_Absolute),
    HID_ReportCount(1),
    HID_ReportSize(8),
    HID_Input(HID_Data | HID_Variable | HID_Absolute),
    HID_ReportCount(5),
    HID_ReportSize(1),
    HID_UsagePage(HID_USAGE_PAGE_LED),
    HID_UsageMin(HID_USAGE_LED_NUM_LOCK),
    HID_UsageMax(HID_USAGE_LED_KANA),
    HID_Output(HID_Data | HID_Variable | HID_Absolute),
    HID_ReportCount(1),
    HID_ReportSize(3),
    HID_Output(HID_Data | HID_Variable | HID_Absolute),
    HID_ReportCount(6),
    HID_ReportSize(8),
    HID_LogicalMin(0),
    HID_LogicalMax(101),
    HID_UsagePage(HID_USAGE_PAGE_KEYBOARD),
    HID_UsageMin(0),
    HID_UsageMax(101),
    HID_Input(HID_Data | HID_Variable | HID_Absolute),
  HID_EndCollection,
};*/
 
// Called during USBD_Initialize to initialize the USB HID class instance.
void USBD_HID0_Initialize (void) {
  // Add code for initialization
}
 
 
// Called during USBD_Uninitialize to de-initialize the USB HID class instance.
void USBD_HID0_Uninitialize (void) {
  // Add code for de-initialization
}
 
 
// \brief Prepare HID Report data to send.
// \param[in]   rtype   report type:
//                - HID_REPORT_INPUT           = input report requested
//                - HID_REPORT_FEATURE         = feature report requested
// \param[in]   req     request type:
//                - USBD_HID_REQ_EP_CTRL       = control endpoint request
//                - USBD_HID_REQ_PERIOD_UPDATE = idle period expiration request
//                - USBD_HID_REQ_EP_INT        = previously sent report on interrupt endpoint request
// \param[in]   rid     report ID (0 if only one report exists).
// \param[out]  buf     buffer containing report data to send.
// \return              number of report data bytes prepared to send or invalid report requested.
//              - value >= 0: number of report data bytes prepared to send
//              - value = -1: invalid report requested
int32_t USBD_HID0_GetReport (uint8_t rtype, uint8_t req, uint8_t rid, uint8_t *buf) {
 
  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
        case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:        // Explicit USB Host request via Control OUT Endpoint
            case USBD_HID_REQ_PERIOD_UPDATE:  // Periodic USB Host request via Interrupt OUT Endpoint
              // Update buffer for report data, example:
              // buf[0] = 0;       // Data Value = 0
              // buf[1] = 5;       // Data Value = 5
              // return (2);       // Data Size  = 2 bytes
              break;
 
            case USBD_HID_REQ_EP_INT:         // Called after USBD_HID_GetReportTrigger to signal
                                              // data obtained.
              break;
          }
          break;
      }
      break;
 
    case HID_REPORT_FEATURE:
      break;
  }
  return (0);
}
 
 
// \brief Process received HID Report data.
// \param[in]   rtype   report type:
//                - HID_REPORT_OUTPUT    = output report received
//                - HID_REPORT_FEATURE   = feature report received
// \param[in]   req     request type:
//                - USBD_HID_REQ_EP_CTRL = report received on control endpoint
//                - USBD_HID_REQ_EP_INT  = report received on interrupt endpoint
// \param[in]   rid     report ID (0 if only one report exists).
// \param[in]   buf     buffer that receives report data.
// \param[in]   len     length of received report data.
// \return      true    received report data processed.
// \return      false   received report data not processed or request not supported.
bool USBD_HID0_SetReport (uint8_t rtype, uint8_t req, uint8_t rid, const uint8_t *buf, int32_t len) {
 
  switch (rtype) {
    case HID_REPORT_OUTPUT:
      /*
        buf: Received Data
        len: Received Data Length
      */
      break;
    case HID_REPORT_FEATURE:
      break;
  }
  return true;
}
