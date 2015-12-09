#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>

/* Preprocessor directives */
#define SD_GO_IDLE_STATE 0x00
#define SD_SEND_OP_COND 0x01
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

#define SD_STATE_NOT_INITIALIZED 0x00
#define SD_STATE_IDLE 0x01
#define SD_STATE_COMMAND_READY 0x02
#define SD_STATE_BUSY 0x03

#define parse_uint8(X)   (((uint8_t)(X)[0]))
#define parse_uint16(X)  ((((uint16_t)(X)[1]) << 8) | (((uint16_t)(X)[0])))
#define parse_uint24(X)  ((((uint32_t)(X)[2]) << 16) | (((uint32_t)(X)[1]) << 8) | (((uint32_t)(X)[0])))
#define parse_uint32(X)  ((((uint32_t)(X)[3]) << 24) | (((uint32_t)(X)[2]) << 16) | (((uint32_t)(X)[1]) << 8) | (((uint32_t)(X)[0])))

int8_t sd_init();
uint8_t sd_command(uint8_t cmd, uint32_t arg, uint16_t read);
uint8_t sd_command_crc(uint8_t cmd, uint32_t arg, uint16_t read, uint8_t crc);
void sd_read(uint32_t sector, uint16_t offset, uint8_t * buffer, uint16_t len);
void sd_write(uint32_t sector, uint8_t * buffer);

uint8_t sd_state;

#endif /* !SDCARD_H */