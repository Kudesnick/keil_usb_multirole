
#include <stdint.h>

#pragma once

void cmd_parse_init(void);
char* cmd_parse(uint8_t *buf, uint32_t cnt);
