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
 *   File:          cpp_os.h
 *   Description:   Object wrapper for RTX-5 primitives.
 *                  Classes provide static memory allocation for stacks, queues and so on.
 *                  Сlasses provide an interface for using OS functions within the C ++ paradigm. 
 *
 ***************************************************************************************************
 *   History:       27.05.2019 - file created
 *                  08.12.2019 - v0.1.0 - added all primitives and Debug Access Port (DAP) support
 *
 **************************************************************************************************/

#pragma once

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include <stdlib.h>
#include <stdint.h>

#include "cpp_list.h"
#include "rtx_os.h"
#include "RTX_Config.h"

#ifdef __cplusplus
    using namespace std;
#endif

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

/**
 *  @brief      Location of control block memory into object structure
 *
 *  @warning    If STATIC_CBM is undefined, you must up size of dynamic memory in RTX_Config.h,
 *              because control blocks located in it.
 */
#define STATIC_CBM EventRecordError

/***************************************************************************************************
 *                                      PUBLIC TYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                                PUBLIC FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                                EXTERNAL FUNCTION PROTOTYPES
 **************************************************************************************************/

void thread_run(void * argument);
void timer_run(void * argument);

/***************************************************************************************************
 *                               PUBLIC CLASS
 **************************************************************************************************/

//-- os_elements

// Абстрактный класс, реализующий общие функции примитивов ОС и обертки над глобальными функциями ОС
class cpp_os : public cpp_list<cpp_os>
{
private:
    // Произвести инициализацию всех примитивов ОС
    static void all_elements_create(void);

protected:
    // Проверить возвращаемое значение (используется для отладки)
    static osStatus_t os_chck(osStatus_t);
    static void * os_chck(void *);
    
public:
    using cpp_list::cpp_list;

    // Инициализация примитива ОС
    virtual void * create(void) = 0;

    // Первичная инициализация ОС
    static void create_os(void);

    // for other functions see https://www.keil.com/pack/doc/CMSIS/RTOS2/html/index.html

    static osStatus_t delay(const uint32_t _ticks)
    {
        return osDelay(_ticks);
    };
    
    static osStatus_t delay_until(const uint32_t _ticks)
    {
        return osDelayUntil(_ticks);
    };

    static uint32_t get_tick_count(void)
    {
        return osKernelGetTickCount();
    };
    
    static uint32_t get_tick_freq(void)
    {
        return osKernelGetTickFreq();
    }
    
    // see https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__Definitions.html
    static constexpr uint32_t flags_error_unknown   = osFlagsErrorUnknown;
    static constexpr uint32_t flags_error_timeout   = osFlagsErrorTimeout;
    static constexpr uint32_t flags_error_resource  = osFlagsErrorResource;
    static constexpr uint32_t flags_error_parameter = osFlagsErrorParameter;
    static constexpr uint32_t flags_error_ISR       = osFlagsErrorISR;
    
    static constexpr uint32_t wait_forever   = osWaitForever;
    
    static constexpr uint32_t flags_wait_any = osFlagsWaitAny;
    static constexpr uint32_t flags_wait_all = osFlagsWaitAll;
    static constexpr uint32_t flags_no_clear = osFlagsNoClear;
    static constexpr uint32_t flags_wait_any_no_clr = osFlagsWaitAny | osFlagsNoClear;
    static constexpr uint32_t flags_wait_all_no_clr = osFlagsWaitAll | osFlagsNoClear;

    static constexpr uint32_t mutex_recursive    = osMutexRecursive;
    static constexpr uint32_t mutex_prio_inherit = osMutexPrioInherit;
    static constexpr uint32_t mutex_robust       = osMutexRobust;
    
    static constexpr  int32_t sts_ok              = osOK;
    static constexpr  int32_t sts_error           = osError;
    static constexpr  int32_t sts_error_timeout   = osErrorTimeout;
    static constexpr  int32_t sts_error_resource  = osErrorResource;
    static constexpr  int32_t sts_error_parameter = osErrorParameter;
    static constexpr  int32_t sts_error_nomemory  = osErrorNoMemory;
    static constexpr  int32_t sts_error_isr       = osErrorISR;
};

//-- thread

// Абстрактный класс потока
/* Параметризация шаблона:
 * - T_stack_size - размер стека
 */
template<uint32_t T_stack_size = OS_STACK_SIZE> class cpp_os_thread : public cpp_os
{
private:
    // Выравнивание по uint64_t обязательно
    uint64_t stack[(T_stack_size + sizeof(uint64_t) - 1) / sizeof(uint64_t)]; 
#ifdef STATIC_CBM
    osRtxThread_t tcb;
#endif
    bool must_create;

    friend void thread_run(void * argument);
    
    // Виртуальная функция тела потока. Должна быть реализована в дочернем классе.
    virtual void thread_func(void) = 0;
    
protected:
    osThreadId_t id_;
    
    osStatus_t yeld(void)
    {        
        return osThreadYield();
    };
    
    void exit(void)
    {
        osThreadExit();
    };
    
    uint32_t flags_clear(uint32_t _flags)
    {
        return osThreadFlagsClear(_flags);	
    };
    
    uint32_t flags_get(void)
    {
        return osThreadFlagsGet();	
    };
    
    uint32_t flags_wait(uint32_t _flags, uint32_t _options, uint32_t _timeout)
    {
        return osThreadFlagsWait(_flags, _options, _timeout);
    };

public:
    // если true - инициализация и запуск потока выполняется на этапе инициализации ОС
    cpp_os_thread(bool _must_create = true):
        cpp_os(),
        must_create(_must_create)
    {};

    // Инициализация и запуск потока
    osThreadId_t create(void)
    {
        if (must_create)
        {
            id_ = run();
        }
        
        return id_;
    };
    
    osThreadId_t run(void)
    {
        const osThreadAttr_t attr =
        {
        #ifdef STATIC_CBM
            .cb_mem     = &tcb,
            .cb_size    = sizeof(tcb),
        #endif
            .stack_mem  = stack,
            .stack_size = sizeof(stack),
        };

        id_ = os_chck(osThreadNew(thread_run, this, &attr));
        
        return id_;
    };
    
    const char * get_name(void)
    {
        return osThreadGetName(id_);
    };
    
    osThreadState_t get_state(void)
    {
        return osThreadGetState(id_);
    };
    
    osStatus_t set_priority(osPriority_t _priority)
    {
        return osThreadSetPriority(id_, _priority);
    };
    
    osPriority_t get_priority(void)
    {        
        return osThreadGetPriority(id_);
    };

    osStatus_t suspend(void)
    {        
        return osThreadSuspend(id_);
    };
    
    osStatus_t resume(void)
    {        
        return osThreadResume(id_);
    };
    
    osStatus_t detach(void)
    {        
        return osThreadDetach(id_);
    };
    
    osStatus_t join(void)
    {        
        return osThreadJoin(id_);
    };
    
    osStatus_t terminate(void)
    {        
        return osThreadTerminate(id_);
    };
    
    uint32_t get_stack_size(void)
    {
        return osThreadGetStackSize(id_);
    };

    uint32_t get_stack_space(void)
    {
        // Stack watermark recording during execution needs to be enabled.
        return osThreadGetStackSize(id_);
    };
    
    static uint32_t get_count(void)	
    {
        return osThreadGetCount();
    };
    
    static uint32_t enumerate(osThreadId_t *& _thread_array, uint32_t _array_items)
    {
        return osThreadEnumerate(_thread_array, _array_items);
    };

    uint32_t flags_set(uint32_t _flags)
    {
        return osThreadFlagsSet(id_, _flags);
    };
};

//-- Timer

// Класс таймера
class cpp_os_timer : public cpp_os
{
private:

#ifdef STATIC_CBM
    osRtxTimer_t  tcb;
#endif
    bool          repeat;
    uint32_t      interval;

    friend void timer_run(void * argument);
    
    // Виртуальная функция тела таймера. Должна быть реализована в дочернем классе.
    virtual void timer_func(void) = 0;

protected:
    osTimerId_t id_;

public:
    // Constructor
    cpp_os_timer(const uint32_t _interval, const bool _repeat):
        cpp_os(),
        interval(_interval),
        repeat(_repeat)
    {};

    // Инициализация таймера
    osTimerId_t create(void)
    {
        const osTimerAttr_t attr =
        {
#ifdef STATIC_CBM
            .cb_mem     = &tcb,
            .cb_size    = sizeof(tcb),
#endif
        };
        
        const osTimerType_t type = (repeat) ? osTimerPeriodic : osTimerOnce;
        
        id_ = osTimerNew(timer_run, type, this, &attr);
        
        return id_;
    };
    
    const char * get_name(void)
    {
        return osTimerGetName(id_);
    };
    
    osStatus_t start(void)
    {
        return osTimerStart(id_, interval);	
    };
    
    osStatus_t start(const uint32_t _interval)
    {
        return osTimerStart(id_, _interval);	
    };
    
    osStatus_t stop(void)
    {
        return osTimerStop(id_);
    };
    
    bool is_runing(void)
    {
        return static_cast<bool>(osTimerIsRunning(id_));
    };
};

//-- queue

// Абстрактный класс очереди
/* Параметризация шаблона:
 * - T_queue_elment_t - тип элемента очереди
 * - T_queue_count - количество элементов в очереди
 */
template<typename T_queue_elment_t, uint32_t T_queue_count> class cpp_os_queue : public cpp_os
{
private:
    uint32_t qdata[osRtxMessageQueueMemSize(T_queue_count, sizeof(T_queue_elment_t)) / sizeof(uint32_t)];
#ifdef STATIC_CBM
    osRtxMessageQueue_t qcb;
#endif

protected:
    osMessageQueueId_t id_;

public:
    using cpp_os::cpp_os;

    // Инициализация очереди
    osMessageQueueId_t create(void)
    {
        const osMessageQueueAttr_t attr =
        {
#ifdef STATIC_CBM
            .cb_mem  = &qcb,
            .cb_size = sizeof(qcb),
#endif
            .mq_mem  = &qdata,
            .mq_size = sizeof(qdata),
        };

        id_ = os_chck(osMessageQueueNew(T_queue_count, sizeof(T_queue_elment_t), &attr));
        return id_;
    };
    
    // for other functions see https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__Message.html

    osStatus_t put(const void * _msg_ptr, uint8_t _msg_prio, uint32_t _timeout)
    {
        return osMessageQueuePut(id_, _msg_ptr, _msg_prio, _timeout);
    };

    osStatus_t get(void * _msg_ptr, uint8_t * _msg_prio, uint32_t _timeout)
    {
        return osMessageQueueGet(id_, _msg_ptr, _msg_prio, _timeout);
    };

    uint32_t get_capacity(void)
    {
        return osMessageQueueGetCapacity(id_);
    };

    uint32_t get_msg_size(void)
    {
        return osMessageQueueGetMsgSize(id_);
    };

    uint32_t get_count(void)
    {
        return osMessageQueueGetCount(id_);
    };  

    uint32_t get_space(void)
    {
        return osMessageQueueGetSpace(id_);
    };

    osStatus_t reset(void)   
    {
        return osMessageQueueReset(id_);
    };
};

//-- event_flags

// Класс флаговых событий
class cpp_os_event : public cpp_os
{
private:
#ifdef STATIC_CBM
    osRtxEventFlags_t ecb;
#endif
protected:
    osEventFlagsId_t id_;

public:
    using cpp_os::cpp_os;

    // Первичная инициализация
    osEventFlagsId_t create(void)
    {
        const osEventFlagsAttr_t attr =
        {
#ifdef STATIC_CBM
            .cb_mem  = &ecb,
            .cb_size = sizeof(ecb),
#endif
        };

        id_ = os_chck(osEventFlagsNew(&attr));
        return id_;
    };
    
    // for other functions see https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__EventFlags.html

    uint32_t set(uint32_t _flags)
    {
        return osEventFlagsSet(id_, _flags);
    };

    uint32_t clear(uint32_t _flags)
    {
        return osEventFlagsClear(id_, _flags);
    };

    uint32_t get(void)
    {
        return osEventFlagsGet(id_);
    };

    uint32_t wait(uint32_t _flags, uint32_t _options, uint32_t _timeout)
    {
        return osEventFlagsWait(id_, _flags, _options, _timeout);
    };
};

//-- mutex

// - T_flags - see https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__MutexMgmt.html#a6e93b49cb79c12f768d72580c7731f30 
class cpp_os_mutex : public cpp_os
{
private:
#ifdef STATIC_CBM
    osRtxMutex_t mcb;
#endif

protected:
    osMutexId_t id_;

    const uint8_t flags;

public:
    cpp_os_mutex(uint32_t _flags = cpp_os::mutex_recursive | cpp_os::mutex_robust | cpp_os::mutex_prio_inherit):
    cpp_os(),
    flags(_flags)
    {};

    // Первичная инициализация
    osMutexId_t create(void)
    {
        const osMutexAttr_t attr =
        {
            .attr_bits = flags,
        #ifdef STATIC_CBM
            .cb_mem    = &mcb,
            .cb_size   = sizeof(mcb),
        #endif
        };
        
        id_ = os_chck(osMutexNew(&attr));
        return id_;
    };
    
    // for other functions see https://www.keil.com/pack/doc/CMSIS/RTOS2/html/group__CMSIS__RTOS__MutexMgmt.html

    osStatus_t acquire(uint32_t _timeout)
    {
        return osMutexAcquire(id_, _timeout);
    };

    osStatus_t release(void)
    {
        return osMutexRelease(id_);
    };

    osThreadId_t get_owner(void)
    {
        return osMutexGetOwner(id_);
    };
};

/***************************************************************************************************
 *                                     GLOBAL VARIABLES
 **************************************************************************************************/
    
/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
