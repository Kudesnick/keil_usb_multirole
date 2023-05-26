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

#ifdef RTE_DEVICE_HAL_COMMON
    #include "stm32f4xx_hal.h" // Device header
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "err_strings.h"

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

#define USBM_STK_SZ (512U)

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

uint64_t stack[(USBM_STK_SZ + sizeof(uint64_t) - 1) / sizeof(uint64_t)]; 
osRtxThread_t tcb;

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

void thread_usb_device (void)
{
    
    volatile usbStatus usb_connect_status  = USBD_Connect(0); /* USB Device 0 Connect */
    printf("<USBD> device connect status: %s\n", err_str_usb_status(usb_connect_status)); 
    if (usb_connect_status != usbOK)
    {
        osThreadExit();
    }
    
    osDelay(500);

    if (USBD_Configured(0))
    {
        printf("<USBD> Device is configured.\n");
    }
    
    for (;USBD_Configured(0); osDelay(100));
    
    printf("<USBD> Device is not configured.\n");
}

/// Emulate USB disconnect (USB_DP)
void usb_disconnect_emulate(void)
{
#ifdef RTE_DEVICE_HAL_COMMON
    static GPIO_InitTypeDef GPIO_InitStruct = 
    {
        .Pin   = GPIO_PIN_12        ,
        .Mode  = GPIO_MODE_OUTPUT_OD,
        .Pull  = GPIO_NOPULL        ,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_InitStruct.Pin, GPIO_PIN_RESET);
    
    HAL_Delay(10);
#elif
    #error Function not implemented for SPL
#endif
}

void thread_func(void *arg)
{
    (void)arg;
    
    for(;; osDelay(TERMINATE_TOUT))
    {
        usbStatus usb_status;
        
        usb_disconnect_emulate();
        
        usb_status = USBD_Initialize (0U);
        printf("<USBD> Initialize: %s\n", err_str_usb_status(usb_status));
        
        if (usb_status == usbOK)
        {
            thread_usb_device();
        }
        
        usb_status = USBD_Uninitialize(0);
        printf("<USBD> Uninitialize: %s\n", err_str_usb_status(USBD_Uninitialize(0)));
    }
};


const osThreadAttr_t attr =
{
    .cb_mem     = &tcb,
    .cb_size    = sizeof(tcb),
    .stack_mem  = stack,
    .stack_size = sizeof(stack),
};

void cdc_thread_create(void)
{
    osThreadNew(thread_func, NULL, &attr);
}


/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
