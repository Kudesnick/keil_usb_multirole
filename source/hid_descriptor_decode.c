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
 *   File:          hid_descriptor_decode.c
 *   Description:   decode HID descriptor for device detect and debug
 *
 ***************************************************************************************************
 *   History:       24.09.2019 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hid_descriptor_decode.h"

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

const struct 
{
    uint8_t id;
    char * const name;
} id_names[] =
{
    // Main Items
    {0x81U, "HID_Input"          },                  
    {0x91U, "HID_Output"         },                  
    {0xB1U, "HID_Feature"        },                  
    {0xA1U, "HID_Collection"     },                  
    {0xC0U, "HID_EndCollection"  },                    
    // Global Items
    {0x05U, "HID_UsagePage"      },                  
    {0x06U, "HID_UsagePageVendor"},                  
    {0x15U, "HID_LogicalMin"     },                  
    {0x16U, "HID_LogicalMinS"    },                  
    {0x17U, "HID_LogicalMinL"    },                  
    {0x25U, "HID_LogicalMax"     },                  
    {0x26U, "HID_LogicalMaxS"    },                  
    {0x27U, "HID_LogicalMaxL"    },                  
    {0x35U, "HID_PhysicalMin"    },                  
    {0x36U, "HID_PhysicalMinS"   },                  
    {0x37U, "HID_PhysicalMinL"   },                  
    {0x45U, "HID_PhysicalMax"    },                  
    {0x46U, "HID_PhysicalMaxS"   },                  
    {0x47U, "HID_PhysicalMaxL"   },                  
    {0x55U, "HID_UnitExponent"   },                  
    {0x65U, "HID_Unit"           },                  
    {0x66U, "HID_UnitS"          },                  
    {0x67U, "HID_UnitL"          },                  
    {0x75U, "HID_ReportSize"     },                  
    {0x76U, "HID_ReportSizeS"    },                  
    {0x77U, "HID_ReportSizeL"    },                  
    {0x85U, "HID_ReportID"       },                  
    {0x95U, "HID_ReportCount"    },                  
    {0x96U, "HID_ReportCountS"   },                  
    {0x97U, "HID_ReportCountL"   },                  
    {0xA4U, "HID_Push"           },                     
    {0xB4U, "HID_Pop"            },                     
    // Local Items
    {0x09U, "HID_Usage"          },                  
    {0x19U, "HID_UsageMin"       },                  
    {0x29U, "HID_UsageMax"       },                  
};

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

static uint8_t get_data_size(uint8_t _code)
{
    switch(_code & 0x3)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 4;
    }
    return 0;
}

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

void hid_desc_print(const uint8_t * _desc, uint32_t len)
{
    uint8_t tab_num = 0;
    
    for(uint8_t * ptr = (uint8_t *)_desc; ptr < (_desc + len); ptr += 1 + get_data_size(*ptr))
    {
        bool err = true;
        
        for(uint8_t i = 0; i < (sizeof(id_names)/sizeof(id_names[0])); i++)
        {
            if (*ptr == id_names[i].id)
            {
                if (*ptr == 0xC0U && tab_num > 0) // HID_EndCollection
                {
                    tab_num--;
                }
                
                for(uint8_t t = tab_num; t > 0; t--)
                {
                    printf("  ");
                }
                
                uint8_t sz = get_data_size(*ptr);
                
                switch(sz)
                {
                    case 0: printf("%s\r\n", id_names[i].name); break;
                    case 1: printf("%s(0x%02X)\r\n", id_names[i].name, *((uint8_t *) (ptr + 1))); break;
                    case 2: printf("%s(0x%04X)\r\n", id_names[i].name, *((uint16_t *)(ptr + 1))); break;
                    case 4: printf("%s(0x%08X)\r\n", id_names[i].name, *((uint32_t *)(ptr + 1))); break;
                }
                
                if (*ptr == 0xC0U && tab_num == 0) // HID_EndCollection
                {
                    printf("\r\n");
                }
                
                if (*ptr == 0xA1U) // HID_Collection
                {
                    tab_num++;
                }
                
                err = false;
                break;
            }
        }
        
        if (err)
        {
            printf("HID descriptor parsing error.\r\n");
            return;
        }
    }
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/
