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
#include "cmsis_compiler.h"

#ifdef RTE_DEVICE_HAL_COMMON
    #include "stm32f4xx_hal.h" // Device header
#endif

#include <stdio.h>

#include "cpp_os.h"

using namespace std;

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

extern "C"
{

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
    printf("<main> Error_Handler!");
    
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
    printf("<main> Wrong parameters value: file %s on line %d\r\n", file, line);
    
    for(;;);
}
#endif

#endif

/// System hard fault
void HardFault_Handler(void)
{
    printf("<main> HardFault!\r\n");
    
    return;
}

#ifdef RTE_CMSIS_RTOS2_RTX5

/// OS Idle Thread (weakly function)
void osRtxIdleThread(void *argument)
{
    (void)argument;

    for (;;)
    {
        __WFE();
    }
}
 
/// OS Error Callback function (weakly function)
uint32_t osRtxErrorNotify (uint32_t code, void *object_id)
{
    const char * ptr = "strange error";
    
    switch (code)
    {
        case osRtxErrorStackUnderflow:
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
        printf("\r\n");
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
    RCC_OscInitStruct.PLL.PLLM = 25;
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
#else
    #error Invalid hardware!
#endif

} // extern "C"

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

#if defined(__ARMCC_VERSION) && !defined(__OPTIMIZE__)
    /*
    Without this directive, it does not start if -o0 optimization is used and the "main"
    function without parameters.
    see http://www.keil.com/support/man/docs/armclang_mig/armclang_mig_udb1499267612612.htm
    */
    __asm (".global __ARM_use_no_argv\n\t" "__ARM_use_no_argv:\n\t");
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
extern "C"
{
int cpp_main(void)
#else
int main(void)
#endif
{
    #ifdef RTE_DEVICE_HAL_COMMON
        HAL_Init();
        // Configure the system clock
        SystemClock_Config();
    #endif
    SystemCoreClockUpdate();

    printf("<main> \033[31mC\033[32mO\033[33mL\033[34mO\033[35mR\033[42m \033[0m"
       "\033[36mT\033[37mE\033[30m\033[47mS\033[0mT\r\n"); // Color test
    printf("<main> Runing main function.\r\n");
    
    printf("<main> Starting OS.\r\n");

    cpp_os::create_os();
    
    printf("<main> Main function terminated.");
    
    for(;;);

    return 0;
}
#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
} // extern "C"
#endif

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
