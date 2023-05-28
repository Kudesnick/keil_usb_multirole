
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cmd_parse.h"
#include "gpio.h"

#define countof(a) (sizeof(a)/sizeof(a[0]))

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
#ifdef LED_PIN
    {.cmd   = "LED_ON"        , .gpio  = {LED_PIN       , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "LED_OFF"       , .gpio  = {LED_PIN       , OFF}, .func  = cmd_func_gpio_set},
#endif                                                  
#ifdef PWR_PIN                                          
    {.cmd   = "PWR_ON"        , .gpio  = {PWR_PIN       , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "PWR_OFF"       , .gpio  = {PWR_PIN       , OFF}, .func  = cmd_func_gpio_set},
#endif                                                  
#ifdef PWRKEY_PIN                                        
    {.cmd   = "PWRKEY_ON"     , .gpio  = {PWRKEY_PIN    , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "PWRKEY_OFF"    , .gpio  = {PWRKEY_PIN    , OFF}, .func  = cmd_func_gpio_set},
#endif                                                  
#ifdef RESET_PIN                                        
    {.cmd   = "RESET_ON"      , .gpio  = {RESET_PIN     , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "RESET_OFF"     , .gpio  = {RESET_PIN     , OFF}, .func  = cmd_func_gpio_set},
#endif
#ifdef GNSS_PWR_PIN
    {.cmd   = "GNSS_PWR_ON"   , .gpio  = {GNSS_PWR_PIN  , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "GNSS_PWR_OFF"  , .gpio  = {GNSS_PWR_PIN  , OFF}, .func  = cmd_func_gpio_set},
#endif
#ifdef GNSS_RESET_PIN
    {.cmd   = "GNSS_RESET_ON" , .gpio  = {GNSS_RESET_PIN, ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "GNSS_RESET_OFF", .gpio  = {GNSS_RESET_PIN, OFF}, .func  = cmd_func_gpio_set},
#endif
#ifdef GNSS_STB_PIN
    {.cmd   = "GNSS_STB_ON"   , .gpio  = {GNSS_STB_PIN  , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "GNSS_STB_OFF"  , .gpio  = {GNSS_STB_PIN  , OFF}, .func  = cmd_func_gpio_set},
#endif                                                  
#ifdef GNSS_ANT_PIN                                     
    {.cmd   = "GNSS_ANT_ON"   , .gpio  = {GNSS_ANT_PIN  , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "GNSS_ANT_OFF"  , .gpio  = {GNSS_ANT_PIN  , OFF}, .func  = cmd_func_gpio_set},
#endif
#ifdef NRF_RES_PIN
    {.cmd   = "NRF_RES_ON"    , .gpio  = {NRF_RES_PIN   , ON }, .func  = cmd_func_gpio_set},
    {.cmd   = "NRF_RES_OFF"   , .gpio  = {NRF_RES_PIN   , OFF}, .func  = cmd_func_gpio_set},
#endif
};

char buffer[32];

void cmd_parse_init(void)
{
    for (int i = 1; i < countof(commands); i += 2)
    {
        if (commands[i].func == cmd_func_gpio_set)
            gpio_init(commands[i].gpio);
        else
            commands[i].func(commands[i].param);
    }
}

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
            for(uint32_t j = 0; j < countof(commands); j++)
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
