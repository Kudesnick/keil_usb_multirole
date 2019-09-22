#include <stdio.h>
#include <string.h>

#include "RTE_Components.h"
#include CMSIS_device_header

#ifdef RTE_DEVICE_HAL_COMMON
    #include "stm32f4xx_hal.h"                  // Device header
#endif

#include "cmsis_os.h"

#include "thread_usb_manager.h"
#include "thread_usb_host.h"
#include "thread_usb_device.h"

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
    
    thread_usb_manager_init(thread_usb_host, thread_usb_device);
    
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
