#include "../kernel.h"


//filesystem:
//	0 is fat32cd
//0xffff is binary
struct partition_data{
	struct drive_data* parent_drive;
	uint16_t filesystem;

}__attribute__((packed));

struct drive_data{
	bool partitions_mounted;
	struct partition_data drive_partitions[16];
}__attribute__((packed));

uint16_t drive_arr_len;
struct drive_data* drive_array;
