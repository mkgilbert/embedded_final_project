#include "system.h"
#include <stdint.h>

uint16_t str_len(char * string){
    uint16_t len = 0;
    while (string[len] != 0x0)
        len++;
    return len;
}