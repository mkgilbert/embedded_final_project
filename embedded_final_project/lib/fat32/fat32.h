#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>

#define fat32_parse_uint8(X)   (((uint8_t)(X)[0]))
#define fat32_parse_uint16(X)  ((((uint16_t)(X)[1]) << 8) | (((uint16_t)(X)[0])))
#define fat32_parse_uint24(X)  ((((uint32_t)(X)[2]) << 16) | (((uint32_t)(X)[1]) << 8) | (((uint32_t)(X)[0])))
#define fat32_parse_uint32(X)  ((((uint32_t)(X)[3]) << 24) | (((uint32_t)(X)[2]) << 16) | (((uint32_t)(X)[1]) << 8) | (((uint32_t)(X)[0])))

#define FAT32_ROOT_CLUSTER 0x02

#define FAT32_STATE_OK 0x01
#define FAT32_STATE_EMPTY 0x00

#define FAT32_FILE_ATTR_DIRECTORY 0x10
#define FAT32_FILE_ATTR_HIDDEN 0x02

#define fat32_file_is_directory(FILE) (((FILE)->attrs & FAT32_FILE_ATTR_DIRECTORY) > (0) ? (1) : (0))
#define fat32_file_is_hidden(FILE) (((FILE)->attrs & FAT32_FILE_ATTR_HIDDEN) > (0) ? (1) : (0))

typedef struct _fat32_partition {
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t num_reserved_sectors;
    uint8_t num_fats;
    uint32_t sectors_per_fat;
    uint32_t root_dir_first_cluster;
    uint16_t signature;
    uint32_t lba_begin;
    uint32_t fat_begin_lba;
    uint32_t cluster_begin_lba;
    uint8_t type_code;
    uint8_t state;
} fat32_partition_t;

typedef struct _fat32_file {
    char dir_name[12];
    uint8_t attrs;
    uint32_t first_cluster;
    uint32_t size;
    char long_fname[64];
    uint8_t state;
	uint32_t previous_cluster;
	uint16_t clusters_read;
} fat32_file_t;

typedef struct _fat32_disk_t {
    fat32_partition_t partition;
    uint8_t state;
} fat32_disk_t;

void fat32_init();

void fat32_disk_init(fat32_disk_t* disk);
void fat32_file_init(fat32_file_t * file);
void fat32_partition_init(fat32_partition_t * partition);
void fat32_read_partition();
void fat32_read_file_data(fat32_file_t * file, uint8_t * buffer, uint32_t length, uint32_t offset);
void fat32_get_root(fat32_file_t * file);
void fat32_write_file_data(fat32_file_t * file, uint8_t * buffer, uint32_t length, uint32_t offset);

#endif /* !FAT32_H */