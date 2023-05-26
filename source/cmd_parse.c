
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cmd_parse.h"
#include "gpio.h"

typedef struct
{
    char *cmd;
    union
    {
        uint32_t param;
        gpio_t   gpio;
    };
    char*(*func)(uint32_t);
} cmd_t;

char* cmd_func_dummy(uint32_t arg)
{
    static const char ret[] = "ok\r";
    printf("<cmd> dummy with param %d.\r", arg);
    return (char*)ret;
}

char* cmd_func_gpio_set(uint32_t arg)
{
    const gpio_t gpio = *(gpio_t*)&arg;
    gpio_set(gpio);
    char ports[] = "ABCDEFGHIJKLMNOP";
    static char buf[32];
    sprintf(buf, "PORT%c_%d %s\r", ports[GPIO_PORT_SOURCE(gpio.pin)], GPIO_PIN_SOURCE(gpio.pin), gpio.state ? "on" : "off");
    return buf;
}

char* echo(uint32_t arg)
{
    extern void set_echo(bool);
    set_echo(arg);
    
    return (arg) ? "echo on\r"  : "echo off\r";
}

const cmd_t commands[] =
{
    {
        .cmd   = "ECHO_ON",
        .param = 1,
        .func  = echo,
    },
    {
        .cmd   = "ECHO_OFF",
        .param = 0,
        .func  = echo,
    },    
    {
        .cmd   = "LED_ON",
        .gpio  = {PORTC_13, GPIO_MODE_OD, HI, ON},
        .func  = cmd_func_gpio_set,
    },
    {
        .cmd   = "LED_OFF",
        .gpio  = {PORTC_13, GPIO_MODE_OD, HI, OFF},
        .func  = cmd_func_gpio_set,
    },
};

char buffer[32];

char* cmd_parse(uint8_t *buf, uint32_t cnt)
{
    char *ret = NULL;
    static uint32_t idx  = 0;
    static const char prefix[] = "$T+";

    for (uint32_t i = 0;i < cnt;i++)
    {
        if (idx < strlen(prefix))
        {
            if (buf[i] == prefix[idx])
            {
                idx++;
            }
            else
            {
                idx = (buf[i] == prefix[0]) ? 1 : 0;
            }
        }
        else
        {
            char ch = buf[i];
            if (ch == '\r' || ch == '\n') ch = '\0';
            uint32_t buf_index = idx - strlen(prefix);
            buffer[buf_index] = ch;
            
            bool must_be_cmd = false;
            for(uint32_t j = 0; j < sizeof(commands)/sizeof(commands[0]); j++)
            {
                if (memcmp(commands[j].cmd, buffer, buf_index + 1) == 0)
                {
                    if (strlen(commands[j].cmd) > buf_index)
                    {
                        must_be_cmd = true;
                    }
                    else if (strlen(commands[j].cmd) == buf_index && ch == '\0')
                    {
                        ret = commands[j].func(commands[j].param);
                        must_be_cmd = false;
                        break;
                    }
                }
            }

            if (must_be_cmd)
            {
                idx++;
            }
            else
            {
                idx = (buf[i] == prefix[0]) ? 1 : 0;
            }
        }
    }
    
    return ret;
}
