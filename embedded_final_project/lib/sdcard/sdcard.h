#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>

/* Preprocessor directives */
// SD commands
// - Always OR's in 0x40 in sd_command
#define SD_GO_IDLE_STATE 0x00
#define SD_ACMD_SEND_OP_COND 0x29
#define SD_SWITCH_FUNC 0x06
#define SD_SEND_IF_COND 0x08
#define SD_SEND_CSD 0x09
#define SD_SEND_CID 0x0A
#define SD_STOP_TRANSMISSION 0x0C
#define SD_SEND_STATUS 0x0D
#define SD_SET_BLOCKLEN 0x10
#define SD_READ_SINGLE_BLOCK 0x11
#define SD_READ_MULTIPLE_BLOCK 0x12
#define SD_WRITE_SINGLE_BLOCK 0x18
#define SD_APP_CMD 0x37

#define SD_COMMAND_LEN 6
#define SD_RW_WAIT_RETRIES 65535

#define SD_CS_PORT B
#define SD_CS_PIN 4

#define parse_uint8(X)   (((uint8_t)(X)[0]))
#define parse_uint16(X)  ((((uint16_t)(X)[1]) << 8) | (((uint16_t)(X)[0])))
#define parse_uint24(X)  ((((uint32_t)(X)[2]) << 16) | (((uint32_t)(X)[1]) << 8) | (((uint32_t)(X)[0])))
#define parse_uint32(X)  ((((uint32_t)(X)[3]) << 24) | (((uint32_t)(X)[2]) << 16) | (((uint32_t)(X)[1]) << 8) | (((uint32_t)(X)[0])))

int8_t sd_init();
uint8_t sd_command(uint8_t cmd, uint32_t arg, uint16_t read);
void sd_read(uint32_t sector, uint16_t offset, uint8_t * buffer, uint16_t len);
void sd_write(uint32_t sector, uint8_t * buffer);

uint8_t sd_state;

#endif /* !SDCARD_H */