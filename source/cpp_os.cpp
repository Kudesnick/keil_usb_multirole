/***************************************************************************************************
 *   Project:       
 *   Author:        
 ***************************************************************************************************
 *   Distribution:  
 *
 ***************************************************************************************************
 *   MCU Family:    STM32F
 *   Compiler:      ARMCC
 ***************************************************************************************************
 *   File:          cpp_os.cpp
 *   Description:   see cpp_os.h
 *
 ***************************************************************************************************
 *   History:       27.05.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include "RTE_Components.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifdef RTE_Compiler_EventRecorder_DAP
    #include "EventRecorder.h"
#endif

#include "cpp_list.h"
#include "cpp_os.h"

#ifdef __cplusplus
    using namespace std;
#endif

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                    PRIVATE FUNCTIONS
 **************************************************************************************************/

//-- os_elements

void cpp_os::all_elements_create(void)
{
    enumerate([](cpp_os *& _el_os)
    {
        _el_os->create();
        
        return true;
    });
};

osStatus_t cpp_os::os_chck(osStatus_t _status)
{
    if (_status != osOK)
    {
        printf("<cpp_os> os_chck not complete.");
    };
    
    return _status;
};

void * cpp_os::os_chck(void * _ptr)
{
    if (_ptr == NULL)
    {
        printf("<cpp_os> os_chck not complete.");
    };
    
    return _ptr;
};

void thread_run(void * argument)
{
    static_cast<cpp_os_thread<> *>(argument)->thread_func();
}

void timer_run(void * argument)
{
    static_cast<cpp_os_timer *>(argument)->timer_func();
}

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

//-- os_elements

void cpp_os::create_os(void)
{
#if defined(RTE_Compiler_EventRecorder_DAP)
    EventRecorderInitialize (EventRecordError, 1);
#endif

    os_chck(osKernelInitialize()); // initialize RTX

    all_elements_create();

    os_chck(osKernelStart()); // start RTX kernel
};

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
