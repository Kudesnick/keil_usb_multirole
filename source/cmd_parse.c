
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cmd_parse.h"

typedef struct
{
    char *cmd;
    uint32_t param;
    void(*func)(uint32_t);
} cmd_t;

void cmd_func_dummy(uint32_t param)
{
    printf("<cmd> dummy with param %d.\r\n", param);
}

const cmd_t commands[] =
{
    {
        .cmd   = "LED_ON",
        .param = 0,
        .func  = cmd_func_dummy,
    },
    {
        .cmd   = "LED_OFF",
        .param = 1,
        .func  = cmd_func_dummy,
    },
};

char buffer[32];

void cmd_parse(uint8_t *buf, uint32_t cnt)
{
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
                        commands[j].func(commands[j].param);
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
}
