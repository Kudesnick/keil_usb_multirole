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

void _print_result(fsStatus _status, uint8_t _drive, char * _action)
{
    printf("<sd> ");
    
    if (_status == fsOK)
    {
        printf("drive \"%s:\" is %s.\r\n", fs_DevPool[_drive].id, _action);
    }
    else
    {
        printf("drive \"%s:\" is not %s. Error: ", fs_DevPool[_drive].id, _action);
        
        switch(_status)
        {
            case fsOK                : break;
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
            case fsAlreadyExists     : printf("File or directory already exists"); break;
            case fsNotDirectory      : printf("Path is not a directory"); break;
        }
        
        printf("\r\n");
    }
}

uint8_t _set_drive_name(uint8_t _drive_num, char * _name)
{
    if (_drive_num >= fs_ndrv)
    {
        _drive_num = 0;
    }
    
    memcpy(_name, fs_DevPool[_drive_num].id, sizeof(fs_DevPool[0].id) - 1);
    memcpy(_name + sizeof(fs_DevPool[0].id) - 1, ":", sizeof(":"));
    
    return _drive_num;
};

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
        // Работаем с картой памяти        
        for (auto i = 0; i < fs_ndrv; i++)
        {
            char label[32];
            char drive[sizeof(fs_DevPool[0].id) + 1] = "MX:";
            _set_drive_name(i, drive);
            printf("<sd> %s init...\r\n", drive);
            fchdrive(drive);

            _print_result(finit (""), i, (char *)"intialised");
            _print_result(fmount(""), i, (char *)"mounted"); 
            fvol("", label, NULL);
            printf("<sd> label: \"%s\"\r\n", label);
        }

        printf("<sd> fs_fat_fh_cnt %d\r\n", fs_fat_fh_cnt);

        for(;;){};
        
    };

} test_sdcard = {};

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
