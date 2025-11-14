#include "../kernel.h"


//filesystem:
//	0 is fat32cd
//0xffff is binary
struct partition_data{
	struct drive_data* parent_drive;
	uint32_t starting_LBA;
	uint32_t ending_LBA;
	uint16_t filesystem;

}__attribute__((packed));

struct drive_data{
	bool partitions_mounted;
	struct partition_data drive_partitions[16];
}__attribute__((packed));

struct GPTLBA0{
	uint8_t boot_indicator;
	uint16_t startingCHS_low;
	uint8_t startingCHS_high;
	uint8_t os_type;
	uint16_t ending_CHS_low;
	uint8_t endingCHS_high;
	uint32_t startingLBA;
	uint32_t endingLBA;
}__attribute__((packed));

struct GPTLBA1{
	uint64_t GPT_SIG;
	uint32_t GPT_VER;
	uint32_t header_size;
	uint32_t checksum;
	uint32_t reserved;
	uint64_t self_LBA;
	uint64_t alternate_copy_LBA;
	uint64_t first_valid_LBA;
	uint64_t last_valid_LBA;
	uint64_t GUID_low;
	uint64_t GUID_high;
	uint64_t partition_array_LBA;
	uint32_t number_partitions;
	uint32_t partition_entry_size;
	uint32_t partition_array_cecksum;
	uint8_t pad_reserved[];
}__attribute__((packed));

struct partition_entry{
	uint64_t partition_GUID_low;
	uint64_t partition_GUID_high;
	uint64_t unique_GUID_low;
	uint64_t unique_GUID_high;
	uint64_t starting_LBA;
	uint64_t ending_LBA;
	uint64_t attributes;
	//36 length 16 array
}__attribute__((packed));

vector* drives;