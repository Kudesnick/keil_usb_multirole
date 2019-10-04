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

#define USBM_STK_SZ (2048U)
#define USB_STK_SZ (2048U)

#define TERMINATE_TOUT 2000

/***************************************************************************************************
 *                                      PRIVATE TYPES
 **************************************************************************************************/

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

// USB host and device common environment
uint64_t usb_stk[(USB_STK_SZ + sizeof(uint64_t) - 1) / sizeof(uint64_t)];
uint32_t usb_tcb[osRtxThreadCbSize / sizeof(uint32_t)];

// USB device thread static environment
const osThreadAttr_t usbd_attr =
{
    .name       = "USB device thread",
    .cb_mem     = &usb_tcb[0],
    .cb_size    = sizeof(usb_tcb),
    .stack_mem  = &usb_stk[0],
    .stack_size = sizeof(usb_stk),
};

// USB host thread static environment
const osThreadAttr_t usbh_attr =
{
    .name       = "USB host thread",
    .cb_mem     = &usb_tcb[0],
    .cb_size    = sizeof(usb_tcb),
    .stack_mem  = &usb_stk[0],
    .stack_size = sizeof(usb_stk),
};

void(* device_thread)(void *);
void(* host_thread)(void *);

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

/// USB OTG detect
static bool usb_otg_detect(void)
{
#ifdef RTE_DEVICE_HAL_COMMON
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    static GPIO_InitTypeDef GPIO_InitStruct = 
    {
        .Pin   = GPIO_PIN_10        ,
        .Mode  = GPIO_MODE_INPUT    ,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Pull  = GPIO_PULLUP        ,
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    return (HAL_GPIO_ReadPin(GPIOA, GPIO_InitStruct.Pin) == GPIO_PIN_RESET);
#elif
    #error Function not implemented for SPL 
#endif
}



static bool thread_kill(osThreadId_t * _id, uint32_t _tout)
{
    if (*_id != NULL)
    {
        if (osThreadGetState(*_id) != osThreadError)
        {
            osDelay(_tout);
            
            if (osThreadGetState(*_id) != osThreadError)
            {
                osThreadTerminate(*_id);
            }
        }
        
        *_id = NULL;
        
        return true;
    }
    else
    {
        return false;
    }
}


static void thread_usb_manager(void *argument)
{
    for(;; osDelay(1000))
    {
        // Init usb host role
        if (usb_otg_detect())
        {
            if (host_id != NULL) continue;
            
            // Kill device thread
            if (thread_kill(&device_id, TERMINATE_TOUT))
            {
                usbStatus usb_status = USBD_Uninitialize(0);
                printf("USBD_Uninitialize: %s\r\n", err_str_usb_status(USBD_Uninitialize(0)));
                
                if (usb_status != usbOK) continue;
            }
            
            // Start host thread
            usbStatus usb_status = USBH_Initialize (0U);
            printf("USBH_Initialize: %s\r\n", err_str_usb_status(usb_status));
            
            if (usb_status == usbOK)
            {
                host_id = osThreadNew(host_thread, NULL, &usbh_attr);
                
                if (host_id != NULL)
                {
                    printf("Thread '%s' is created.\r\n", usbh_attr.name);
                    
                    while (usb_otg_detect()) osDelay(100);
                }
                else
                {
                    printf("Thread '%s' is not created.\r\n", usbh_attr.name);
                }
            }
        }
        
        // init usbdevice device role
        else
        {
            if (device_id != NULL) continue;
            
            // Kill host thread
            if (thread_kill(&host_id, TERMINATE_TOUT))
            {
                usbStatus usb_status = USBH_Uninitialize(0);
                printf("USBH_Uninitialize: %s\r\n", err_str_usb_status(usb_status));
                
                if (usb_status != usbOK) continue;
            }
            
            // Start host thread
            usbStatus usb_status = USBD_Initialize (0U);
            printf("USBD_Initialize: %s\r\n", err_str_usb_status(usb_status));
            
            if (usb_status == usbOK)
            {
                device_id = osThreadNew(device_thread, NULL, &usbd_attr);
                
                if (device_id != NULL)
                {
                    printf("Thread '%s' is created.\r\n", usbd_attr.name);
                    
                    while (!usb_otg_detect()) osDelay(100);
                }
                else
                {
                    printf("Thread '%s' is not created.\r\n", usbd_attr.name);
                }
            }
            
        }
    }
}

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/

int thread_usb_manager_init(void(* _host_thread)(void *), void(* _device_thread)(void *))
{
    device_thread = _device_thread;
    host_thread = _host_thread;
    
    osThreadId_t id = osThreadNew(thread_usb_manager, NULL, &usbm_attr);
    
    return (id == NULL) ? -1 : 0;
}
