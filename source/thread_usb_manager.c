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

#include "thread_usb_manager.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "RTE_Components.h"
#include CMSIS_device_header

#ifdef RTE_DEVICE_HAL_COMMON
    #include "stm32f4xx_hal.h"                  // Device header
#endif

#include "cmsis_os.h"
#include "rl_usb.h"

#include "err_strings.h"

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

#define USBM_STK_SZ (2048U * 16)

#define TERMINATE_TOUT 2000

/***************************************************************************************************
 *                                      PRIVATE TYPES
 **************************************************************************************************/

typedef struct
{
    void(* device_thread)(void);
    void(* host_thread)(void);
} arg_t;

/***************************************************************************************************
 *                               PRIVATE FUNCTION PROTOTYPES
 **************************************************************************************************/

static void thread_usb_manager (void *argument); // thread function

/***************************************************************************************************
 *                                       PRIVATE DATA
 **************************************************************************************************/

osThreadId_t device_id, host_id;

// USB multirole manager thread static environment
uint64_t usbm_stk[(USBM_STK_SZ + sizeof(uint64_t) - 1) / sizeof(uint64_t)];
uint32_t usbm_tcb[osRtxThreadCbSize / sizeof(uint32_t)];
const osThreadAttr_t usbm_attr =
{
    .name       = "USB multirole manager thread",
    .cb_mem     = &usbm_tcb[0],
    .cb_size    = sizeof(usbm_tcb),
    .stack_mem  = &usbm_stk[0],
    .stack_size = sizeof(usbm_stk),
};

/***************************************************************************************************
 *                                       PUBLIC DATA
 **************************************************************************************************/

/***************************************************************************************************
 *                                      EXTERNAL DATA
 **************************************************************************************************/

extern const uint8_t usbd_hid0_report_descriptor[];

/***************************************************************************************************
 *                              EXTERNAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                                    PRIVATE FUNCTIONS
 **************************************************************************************************/

/// Emulate USB disconnect (USB_DP)
static void usb_disconnect_emulate(void)
{
#ifdef RTE_DEVICE_HAL_COMMON
    static GPIO_InitTypeDef GPIO_InitStruct = 
    {
        .Pin   = GPIO_PIN_12        ,
        .Mode  = GPIO_MODE_OUTPUT_OD,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Pull  = GPIO_NOPULL        ,
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_InitStruct.Pin, GPIO_PIN_RESET);
	
	HAL_Delay(10);
#elif
    #error Function not implemented for SPL 
#endif
}

static void thread_usb_manager(void *argument)
{
    for(uint8_t otg = 1; true; osDelay(TERMINATE_TOUT), otg ^=1)
    {
        usbStatus usb_status;
        
        // Init usb host role
        if (otg)
        {
            usb_status = USBH_Initialize (0U);
            printf("<USBH> Initialize: %s\r\n", err_str_usb_status(usb_status));
            
            if (usb_status == usbOK)
            {
                uint32_t time = osKernelGetTickCount();

                while (osKernelGetTickCount() - time < 4000)
                {
                    ((arg_t *)argument)->host_thread();
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
                ((arg_t *)argument)->device_thread();
            }
            
            usb_status = USBD_Uninitialize(0);
            printf("<USBD> Uninitialize: %s\r\n", err_str_usb_status(USBD_Uninitialize(0)));
        }
    }
}

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/

int thread_usb_manager_init(void(* _host_thread)(void), void(* _device_thread)(void))
{
    static arg_t args;
    
    args = (arg_t){_device_thread, _host_thread};
    
    osThreadId_t id = osThreadNew(thread_usb_manager, &args, &usbm_attr);
    
    return (id == NULL) ? -1 : 0;
}
