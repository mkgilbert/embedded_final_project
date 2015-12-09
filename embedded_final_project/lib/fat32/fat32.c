#include "fat32.h"
#include "lib/sdcard/sdcard.h"
#include "system.h"
#include "lib/uart/uart.h"
#include "lib/print_tools/colors.h"
#include <string.h>
#include <stdio.h>

fat32_disk_t current_disk;
fat32_file_t last_file;

void fat32_init(){
	fat32_disk_init(&current_disk);
	fat32_read_partition();
}

void fat32_disk_init(fat32_disk_t * disk){
    // Set the default state
    disk->state = FAT32_STATE_EMPTY;
    // Ensure that the partitions are initialized
    fat32_partition_init(&(disk->partition));
	fat32_read_partition();
}

void fat32_file_init(fat32_file_t * file){
    // Set the default state
    file->state = FAT32_STATE_EMPTY;
    // Ensure everything is empty
    bzero(file->dir_name, 12);
    bzero(file->long_fname, 64);
    file->size = 0x00;
    file->first_cluster = 0x00;
    file->attrs = 0x00;
	file->previous_cluster = 0x00;
	file->clusters_read = 0x00;
}
void fat32_partition_init(fat32_partition_t * partition){
    // Set the default state
    partition->state = FAT32_STATE_EMPTY;
    // Ensure everything is empty
    partition->bytes_per_sector = 0x00;
    partition->sectors_per_cluster = 0x00;
    partition->num_reserved_sectors = 0x00;
    partition->num_fats = 0x00;
    partition->sectors_per_fat = 0x00;
    partition->root_dir_first_cluster = 0x00;
    partition->signature = 0x00;
    partition->lba_begin = 0x00;
    partition->fat_begin_lba = 0x00;
    partition->cluster_begin_lba = 0x00;
    partition->type_code = 0x00;
}

void fat32_get_root(fat32_file_t * file){
    fat32_file_init(file);
    file->attrs = FAT32_FILE_ATTR_DIRECTORY;
    file->first_cluster = FAT32_ROOT_CLUSTER;
    file->state = FAT32_STATE_OK;
}

void fat32_read_partition(){
    // Define a buffer so we can read in
    uint8_t buffer[64];
    fat32_partition_t * partition = &(current_disk.partition);
    
    // Read the partition table
    //TODO: Abstract away SD_read
    sd_read(0, 446, buffer, 16);
	
	uart_print_buffer(&buffer, 64, 16);
    
    // Grab the beginning of the Volume ID
    partition->lba_begin = fat32_parse_uint32(buffer + 8);
    // Grab the type code of the partition
    partition->type_code = fat32_parse_uint8(buffer + 4);
    
    // This is only a valid partition if type_code is set
    if (!partition->type_code){
        return;
    }
    
    // Read the Volume ID information
    
    //TODO: Abstract away SD_read
    sd_read(partition->lba_begin, 0, buffer, 64);
	
	uart_print_buffer(&buffer, 64, 16);
    
    partition->num_reserved_sectors = fat32_parse_uint16(buffer + 0x0E);
    partition->num_fats = fat32_parse_uint8(buffer + 0x10);
    partition->sectors_per_fat = fat32_parse_uint32(buffer + 0x24);
    partition->sectors_per_cluster = fat32_parse_uint8(buffer + 0x0D);
    partition->root_dir_first_cluster =  fat32_parse_uint32(buffer + 0x2C);
    partition->bytes_per_sector = fat32_parse_uint16(buffer + 0x0B);
    
    partition->fat_begin_lba = partition->lba_begin + partition->num_reserved_sectors;
    partition->cluster_begin_lba = partition->lba_begin + partition->num_reserved_sectors + (partition->num_fats * partition->sectors_per_fat);
    
    // We loaded this with info so set the state
    partition->state = FAT32_STATE_OK;
}

void fat32_file_lookup(fat32_file_t * file, char * fname){
	fat32_file_t root;
	fat32_file_init(file);
	fat32_get_root(&root);
	    
    // Create a shortcut to the partition we're working with
    fat32_partition_t * partition = &current_disk.partition;
    
    uint8_t buffer[512];
    uint8_t sector = 0;
    
    sd_read(partition->cluster_begin_lba + (root.first_cluster - 2) * partition->sectors_per_cluster + sector, 0, buffer, 512);

    unsigned char * working_buffer = buffer;
    /*
     
     1. If the first byte of the entry is equal to 0 then there are no more files/directories in this directory. Yes, goto 2. No, finish.
     2. If the first byte of the entry is equal to 0xE5 then the entry is unused. Yes, goto number 3. No, goto number 9
     3. Is this entry a long file name entry? If the 11'th byte of the entry equals 0x0F, then it is a long file name entry. Otherwise, it is not. Yes, goto number 4. No, goto number 5.
     4. Read the portion of the long filename into a temporary buffer. Goto 9.
     5. Parse the data for this entry using the table from further up on this page. It would be a good idea to save the data for later. Possibly in a virtual file system structure. goto number 7
     6. Is there a long file name in the temporary buffer? Yes, goto number 8. No, goto 9
     7. Apply the long file name to the entry that you just read and clear the temporary buffer. goto number 9
     8. Increment pointers and/or counters and check the next entry. (goto number 1)
     */
    
    uint8_t j;
    uint8_t fname_index;
    while (working_buffer[0] != 0) {
        // Check to see if this entry is unused
        if (working_buffer[0] == 0xE5){
            // Increment the buffer and skip this entry
            working_buffer += 32;
            // See if we need to read the next sector
            if (working_buffer - buffer >= 512){
                sd_read(partition->cluster_begin_lba + (root.first_cluster - 2) * partition->sectors_per_cluster + ++sector, 0, buffer, 512);
                working_buffer = buffer;
            }
            continue;
        }
        
        while (working_buffer[11] == 0x0F) {
            fname_index = working_buffer[0];
            fname_index &= ~(0x40);
            fname_index --;
            
            file->long_fname[fname_index*13 + 0] = working_buffer[1];
            file->long_fname[fname_index*13 + 1] = working_buffer[3];
            file->long_fname[fname_index*13 + 2] = working_buffer[5];
            file->long_fname[fname_index*13 + 3] = working_buffer[7];
            file->long_fname[fname_index*13 + 4] = working_buffer[9];
            file->long_fname[fname_index*13 + 5] = working_buffer[14];
            file->long_fname[fname_index*13 + 6] = working_buffer[16];
            file->long_fname[fname_index*13 + 7] = working_buffer[18];
            file->long_fname[fname_index*13 + 8] = working_buffer[20];
            file->long_fname[fname_index*13 + 9] = working_buffer[22];
            file->long_fname[fname_index*13 + 10] = working_buffer[24];
            file->long_fname[fname_index*13 + 11] = working_buffer[28];
            file->long_fname[fname_index*13 + 12] = working_buffer[30];
            
            // Increment the buffer
            working_buffer += 32;
            // See if we need to read the next sector
            if (working_buffer - buffer >= 512){
                sd_read(partition->cluster_begin_lba + (root.first_cluster - 2) * partition->sectors_per_cluster + ++sector, 0, buffer, 512);
                working_buffer = buffer;
            }
        }
        
        for (j = 0; j < 11; j++) {
            file->dir_name[j] = working_buffer[j];
        }
        
        file->attrs = working_buffer[0x0B];
        file->first_cluster = (uint32_t) parse_uint16(working_buffer + 0x14) << 16 | parse_uint16(working_buffer + 0x1A);
        file->size = parse_uint32(working_buffer + 0x1C);
        
        file->state = FAT32_STATE_OK;
        
		if (strcmp(fname, file->long_fname) == 0){
			return;
		}
		
        // Increment the buffer
        working_buffer += 32;
        // See if we need to read the next sector
        if (working_buffer - buffer >= 512){
            sd_read(partition->cluster_begin_lba + (root.first_cluster - 2) * partition->sectors_per_cluster + ++sector, 0, buffer, 512);
            working_buffer = buffer;
        }
    }

    // Clear the file as we didn't find a match
    fat32_file_init(file);
    return;
}

void fat32_read_file_data(fat32_file_t * file, uint8_t * buffer, uint32_t length, uint32_t offset){
    
    bzero(buffer, length);
    
    // Create a shortcut to the partition we're working with
    fat32_partition_t * partition = &current_disk.partition;
    uint32_t sector = (offset/512) % partition->sectors_per_cluster;
    uint32_t cluster = file->first_cluster;
	
	if (file->previous_cluster != 0){
		cluster = file->previous_cluster;
	}
    
    offset -= sector * 512;

    uint8_t fat_buffer[4];
	
	while(offset / (partition->sectors_per_cluster * 512) > file->clusters_read){
        sd_read(partition->fat_begin_lba + cluster/128, (cluster % 128) * 4, fat_buffer, 4);
        cluster = fat32_parse_uint32(fat_buffer);
		file->clusters_read++;
		file->previous_cluster = cluster;
	}
    
    if (file->size < length){
        length = file->size;
    }

    sd_read(partition->cluster_begin_lba + (cluster - 2) * partition->sectors_per_cluster + sector, offset % 512, buffer, length);
	
    return;
}

void fat32_write_file_data(fat32_file_t * file, uint8_t * buffer, uint32_t length, uint32_t offset){
    // Create a shortcut to the partition we're working with
    fat32_partition_t * partition = &current_disk.partition;
    uint32_t sector = (offset/512) % partition->sectors_per_cluster;
    uint32_t cluster = file->first_cluster;
    
    offset -= sector * 512;
    
    if (file->size < length){
        length = file->size;
    }
	    
    uint8_t fat_buffer[4];
    while (offset >= 512) {
        if (offset >= partition->sectors_per_cluster*512){
            sd_read(partition->fat_begin_lba + cluster/128, (cluster % 128) * 4, fat_buffer, 4);
            cluster = fat32_parse_uint32(fat_buffer);
        }
        offset -= 512 * partition->sectors_per_cluster;
    }
	sd_write(partition->cluster_begin_lba + (cluster - 2) * partition->sectors_per_cluster + sector, buffer);
    
    return;
}