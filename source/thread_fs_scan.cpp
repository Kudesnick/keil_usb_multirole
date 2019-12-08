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
 *   File:          test_sdcard.cpp
 *   Description:   
 *
 ***************************************************************************************************
 *   History:       23.06.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rl_fs.h"
#include "rl_fs_lib.h"

#include "cpp_os.h"

using namespace std;

/***************************************************************************************************
 *                                       DEFINITIONS
 **************************************************************************************************/

#define SDCARD_STK_SZ (2048U)

/***************************************************************************************************
 *                                      PRIVATE TYPES
 **************************************************************************************************/

/***************************************************************************************************
 *                               PRIVATE FUNCTION PROTOTYPES
 **************************************************************************************************/

fsStatus _print_result(const fsStatus _status, const char *_drive, const char *_action)
{
    printf("<sd> drive \"%s\" is %s ", _drive, _action);
    
    if (_status != fsOK)
    {
        printf("error: ");
    }

    switch(_status)
    {
        case fsOK                : printf("fsOK"); break;
        case fsError             : printf("fsError - Unspecified error"); break;
        case fsUnsupported       : printf("fsUnsupported - Operation not supported"); break;
        case fsAccessDenied      : printf("fsAccessDenied - Resource access denied"); break;
        case fsInvalidParameter  : printf("fsInvalidParameter - Invalid parameter specified"); break;
        case fsInvalidDrive      : printf("fsInvalidDrive - Nonexistent drive"); break;
        case fsInvalidPath       : printf("fsInvalidPath - Invalid path specified"); break;
        case fsUninitializedDrive: printf("fsUninitializedDrive - Drive is uninitialized"); break;
        case fsDriverError       : printf("fsDriverError - Read/write error"); break;
        case fsMediaError        : printf("fsMediaError - Media error"); break;
        case fsNoMedia           : printf("fsNoMedia - No media, or not initialized"); break;
        case fsNoFileSystem      : printf("fsNoFileSystem - File system is not formatted"); break;
        case fsNoFreeSpace       : printf("fsNoFreeSpace - No free space available"); break;
        case fsFileNotFound      : printf("fsFileNotFound - Requested file not found"); break;
        case fsDirNotEmpty       : printf("fsDirNotEmpty - The directory is not empty"); break;
        case fsTooManyOpenFiles  : printf("fsTooManyOpenFiles - Too many open files"); break;
        case fsAlreadyExists     : printf("fsAlreadyExists - File or directory already exists"); break;
        case fsNotDirectory      : printf("fsNotDirectory - Path is not a directory"); break;
    }

    printf(".\r\n");
    
    return _status;
}

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

class : public cpp_os_thread<SDCARD_STK_SZ>
{
private:
    void thread_func(void)
    {
        // Printed info about SPI drive
        for (auto i = 0; i < fs_ndrv; i++)
        {
            char drive[4];
            sprintf(drive, "%2s:", fs_DevPool[i].id);
            
            printf("<sd> %s init..\r\n", drive);
            
            if (_print_result(fchdrive(drive), drive, (const char *)"fchdrive") != fsOK)
            {
                continue;
            }

            if (_print_result(finit(drive), drive, (const char *)"finit") != fsOK)
            {
                continue;
            }
            
            if (_print_result(fmount(drive), drive, (const char *)"fmount") != fsOK)
            {
                continue;
            }
            
            char label[12];
            uint32_t serial;
            if (_print_result(fvol(drive, label, &serial), drive, (const char *)"fmount") != fsOK)
            {
                continue;
            }
            
            printf("<sd> drive \"%s\" label: \"%s\", serial number: %x.\r\n", drive, label, serial);
        }

        printf("<sd> fs_fat_fh_cnt = %d\r\n", fs_fat_fh_cnt);

        exit();
    };

public:
    using cpp_os_thread::cpp_os_thread;
    
} thread_fs_scan = {false};

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
