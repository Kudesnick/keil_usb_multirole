#include <stdio.h>
#include <string.h>

#include "RTE_Components.h"
#include CMSIS_device_header

#ifdef RTE_DEVICE_HAL_COMMON
    #include "stm32f4xx_hal.h"                  // Device header
#endif

#include "cmsis_os.h"
#include "rl_fs.h"                      // Keil.MDK-Pro::File System:CORE
#include "rl_usb.h"                     // Keil.MDK-Pro::USB:CORE

#include "USBH_MSC.h"

#include "err_strings.h"

#include "thread_usb_manager.h"

#ifdef RTE_DEVICE_HAL_COMMON
#ifdef RTE_CMSIS_RTOS2_RTX5
/**
  * Override default HAL_GetTick function
  */
uint32_t HAL_GetTick (void)
{
    static uint32_t ticks = 0U;
           uint32_t i;
    
    if (osKernelGetState () == osKernelRunning) 
    {
        return ((uint32_t)osKernelGetTickCount ());
    }
    
    /* If Kernel is not running wait approximately 1 ms then increment 
       and return auxiliary tick counter value */
    for (i = (SystemCoreClock >> 14U); i > 0U; i--)
    {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
    return ++ticks;
}
#endif
#endif



/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device Human Interface Device example program
 *----------------------------------------------------------------------------*/
__NO_RETURN void usbd_handle (void *arg)
{
    uint8_t buf[1];

    volatile usbStatus usb_connect_status;
    
    usb_connect_status = USBD_Connect(0); /* USB Device 0 Connect */
    printf("USB device connect status: %s\r\n", err_str_usb_status(usb_connect_status)); 
    if (usb_connect_status != usbOK) for(;;);

    for (;;)
    {   /* Loop forever */
        USBD_HID_GetReportTrigger(0, 0, &buf[0], 1);
        osDelay(100); /* 100 ms delay for sampling buttons  */
    }
}

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Host
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    MassStorage.c
 * Purpose: USB Host - Mass Storage example
 *----------------------------------------------------------------------------*/


__NO_RETURN void usbh_handle (void *arg) {
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

#ifdef RTE_DEVICE_HAL_COMMON

static void SystemClock_Config(void);
static void Error_Handler(void);

#endif

#if defined(__ARMCC_VERSION) && !defined(__OPTIMIZE__)
    /*
    Without this directive, it does not start if -o0 optimization is used and the "main"
    function without parameters.
    see http://www.keil.com/support/man/docs/armclang_mig/armclang_mig_udb1499267612612.htm
    */
    __asm (".global __ARM_use_no_argv\n\t" "__ARM_use_no_argv:\n\t");
#endif

int main(void)
{
    printf("main runing.\r\n");
    
#ifdef RTE_DEVICE_HAL_COMMON
    HAL_Init();

    /* Configure the system clock to 96 MHz */
    SystemClock_Config();
#endif
    // System Initialization
    SystemCoreClockUpdate();

    osKernelInitialize();
    
    thread_usb_manager_init(usbd_handle, usbh_handle);
    
    printf("Kernel start.\r\n");

    osKernelStart();
    
    for (;;);
}

#ifdef RTE_DEVICE_HAL_COMMON

/**
  * @brief System Clock Configuration
  * @note This funtion is generated from CubeMX project
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    
    /** Configure the main internal regulator output voltage 
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 384;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    RCC_OscInitStruct.PLL.PLLR = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* User may add here some code to deal with this error */
    
    for(;;){__BKPT(0);}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number*/
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    
    /* Infinite loop */
    for(;;){__BKPT(0);}
}

#endif

#endif /* RTE_DEVICE_HAL_COMMON */
