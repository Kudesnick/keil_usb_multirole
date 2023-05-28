/***************************************************************************************************
 *   Project:     
 *   Author:        Stulov Tikhon (kudesnick@inbox.ru)
 ***************************************************************************************************
 *   Distribution:  
 *
 ***************************************************************************************************
 *   MCU Family:    STM32F
 *   Compiler:      ARMCC
 ***************************************************************************************************
 *   File:          main.c
 *   Description:   
 *
 ***************************************************************************************************
 *   History:       13.04.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdio.h>

#include "cmsis_compiler.h"
#include "cmsis_os2.h"
#include "rtx_os.h"
#ifdef RTE_DEVICE_HAL_COMMON
    #include "stm32f4xx_hal.h" // Device header
#endif

#include "usb_observer.h"
#include "cmd_parse.h"

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

#ifdef RTE_DEVICE_HAL_COMMON

#ifdef RTE_CMSIS_RTOS2_RTX5
/**
  * @brief  Add HAL_GetTick FUNCTION
  * @param  None
  * @return System tick counter
  */
uint32_t HAL_GetTick(void) 
{
    static uint32_t ticks = 0U;
    
    if (osKernelGetState() == osKernelRunning)
    {
        return osKernelGetTickCount();
    }
    
    // If Kernel is not running wait approximately 1 ms then increment
    // and return auxiliary tick counter value
    for (uint32_t i = (SystemCoreClock >> 14U); i > 0U; i--)
    {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
    return ++ticks;
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @return None
  */
static void Error_Handler(void)
{
    printf("<main> Error_Handler!\r");
    
    for(;;);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @return None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    // User can add his own implementation to report the file name and line number
    printf("<main> Wrong parameters value: file %s on line %d\r", file, line);
    
    for(;;);
}
#endif

#endif

/// System hard fault
void HardFault_Handler(void)
{
    printf("<main> HardFault!\r");
    
    return;
}

#ifdef RTE_CMSIS_RTOS2_RTX5

/// OS Idle Thread (weakly function)
void osRtxIdleThread(void *argument)
{
    (void)argument;

    for (;;)
    {
#ifndef DEBUG
        __WFE();
#else
        __NOP();
#endif
    }
}
 
/// OS Error Callback function (weakly function)
uint32_t osRtxErrorNotify (uint32_t code, void *object_id)
{
    const char * ptr = "strange error";
    
    switch (code)
    {
        case osRtxErrorStackOverflow:
            // Stack overflow detected for thread (thread_id=object_id)
            ptr = "Stack overflow detected for thread";
            break;
        case osRtxErrorISRQueueOverflow:
            // ISR Queue overflow detected when inserting object (object_id)
            ptr = "ISR Queue overflow detected when inserting object";
            break;
        case osRtxErrorTimerQueueOverflow:
            // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
            ptr = "User Timer Callback Queue overflow detected for timer";
            break;
        case osRtxErrorClibSpace:
            // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
            ptr = "Standard C/C++ library libspace not available";
            break;
        case osRtxErrorClibMutex:
            // Standard C/C++ library mutex initialization failed
            ptr = "Standard C/C++ library mutex initialization failed";
            break;
        default:
            break;
    }
    
    if (object_id != NULL)
    {
        printf("<main> OS %s. ID = 0x%08X.", ptr, (uint32_t)object_id);

        const char *name = osThreadGetName(object_id);
        
        if (name != NULL)
        {
             printf(" Name = '%s'.", name);
        }
    }
    else
    {
        printf("\r");
    }
    
    return 0U;
}

#endif

/**
  * @brief  System Clock Configuration (core clock = 98 MHz)
  * @note   This funtion is generated from CubeMX project
  * @return None
  */
#if defined(STM32F413xx)
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 16; // If HSE = 16 MHz
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
#elif defined(STM32F411xE)
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25; // If HSE = 25 MHz
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
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
}

#endif

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

int main(void)
{
    #ifdef RTE_DEVICE_HAL_COMMON
        HAL_Init();
        SystemClock_Config();
    #endif
    SystemCoreClockUpdate();
    
    cmd_parse_init();

    printf("<main> \e[31mC\e[32mO\e[33mL\e[34mO\e[35mR\e[42m \e[0m\e[36mT\e[37mE\e[30m\e[47mS\e[0mT\r"); // Color test
    printf("<main> Runing main function.\r");

    printf("<main> Starting OS.\r");

    if (osKernelInitialize() != osOK) // initialize RTX
    {
        printf("<main> osKernelInitialize Error.\r");
        return -1;
    }

    cdc_thread_create();

    if (osKernelStart() != osOK) // start RTX kernel
    {
        printf("<main> osKernelStart Error.\r");
        return -1;
    }
    
    printf("<main> Main function terminated.\r");

    return -1;
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
