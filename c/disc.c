#include "../kernel.h"
#include "disc.h"
/*
 * int sd_writesector(uint32_t sector, uint8_t *buffer, uint32_t sector_count);
 * 
 * // sd_readsector: Return 1 on success, 0 on failure
 * int sd_readsector(uint32_t sector, uint8_t *buffer, uint32_t sector_count);
 * 
 */


//fatfs stuff

void mono_disc_init(){
	struct drive_data* disc1 = kmalloc(sizeof(struct drive_data));
	drives = new_vector();
	if(!drives || !disc1){
		print_string("KMALLOC DRIVE INITIALISATION FAILED\n");
		error_can_continue();
		return;
	}
	void* tmp = drives->resize(drives, 1);
	if(!tmp){
		print_string("DRIVE_VECTOR_RESIZING FAILED\n");
		error_can_continue();
		drives->destroy(drives);
		return;
	}
	drives->data[0] = (uint32_t)disc1;
	disc1->partitions_mounted = false;
	volatile disc_sector tmp_sect = {0};
	if(!disc_read_ATA_PIO(&tmp_sect, 0)){
		print_string("DISC_READ FAILED (LBA0)\n");
		error_can_continue(); 
		return;
	}
	struct GPTLBA0 LBA0 = *(struct GPTLBA0*)(((uintptr_t)&tmp_sect) + 0x1be);

	if(LBA0.boot_indicator != 0x00 || LBA0.startingCHS_low != 0x200 || LBA0.startingCHS_high || LBA0.os_type != 0xee){
		print_string("ERROR, DISC NOT GPT\n");
		error_can_continue();
		return;
	}
	
	if(!disc_read_ATA_PIO(&tmp_sect, 1)){
		print_string("DISC_READ FAILED\n (partition header table)");
		error_can_continue();
		return;
	}
	//tmpp_sect contains LBA1: Partition table header
	struct GPTLBA1 partition_table_header = *((struct GPTLBA1*)&(tmp_sect));
	if(partition_table_header.GPT_SIG != 0x5452415020494645)/*Signature backwards because compiler does little endian*/{
		print_string("Incorrect signature\n");
		error_can_continue();
		return;
	}
	uint32_t partition_array_LBA = partition_table_header.partition_array_LBA;
	if(!disc_read_ATA_PIO(&tmp_sect, partition_array_LBA)){
		print_string("DISC READ FAILED (partition array)\n");
		error_can_continue();
		return;
	}
	struct partition_entry part1 = *(struct partition_entry*)&tmp_sect;
	((struct drive_data*)drives->data[0])->drive_partitions[0].starting_LBA = part1.starting_LBA;
	((struct drive_data*)drives->data[0])->drive_partitions[0].ending_LBA = part1.ending_LBA;
	((struct drive_data*)drives->data[0])->drive_partitions[0].filesystem = 0;	
	fl_init();
	if(fl_attach_media(abstracted_disc_part_read, abstracted_disc_part_write) != FAT_INIT_OK){
		print_string("DISC LIBRARY INIT FAILIURE");
		error_can_continue();
		return;
	}
	
	// dump_obj((((struct drive_data*)drives->data[0])->drive_partitions[0]))

}

int abstracted_disc_part_read(uint32_t sector, uint8_t* buffer, uint32_t sector_count){
	if(sector_count != 1){print_string("SECTOR_COUNT_READ_NOT_1"); hang();}
	int tmp = disc_read_ATA_PIO((volatile disc_sector*)buffer, (((struct drive_data*)drives->data[0])->drive_partitions[0].starting_LBA)+sector);
	return(tmp);
}

int abstracted_disc_part_write(uint32_t sector, uint8_t* buffer, uint32_t sector_count){
	if(sector_count != 1){print_string("SECTOR_COUNT_WRITE_NOT_1"); hang();}
	print_string("WRITE USED");
	hang();
}

bool disc_poll_ATA_PIO(){
	
	//400ns delay
	for(volatile uint8_t i = 0; i < 14; i++){
		bytein(0x1f7);
	}
	//1 second deadline. Should work as long as TIMER_HZ is less than 2^16
	uint16_t deadline = time + TIMER_FREQUENCY*1	;
	while((uint16_t)time < deadline){
	uint8_t status = bytein(0x1f7);
	uint8_t BSY, DRQ, ERR, DF;
	BSY = (status&0b10000000)>>7;
	DRQ = (status&0b00001000)>>3;
	ERR = (status&0b00000001)>>0;
	 DF = (status&0b00100000)>>5;
	if(ERR == 1 || DF == 1){
		//AN ERROR HAS OCCURRED
		return(false);
	}
	if(BSY == 0 && DRQ == 1){
		//ready to progress
		return(true);
	}
	}
	//DEADLINE PASSED
	return(false);
}

bool disc_read_ATA_PIO(volatile disc_sector* sector_address, uint32_t LBA){
	//starting disc read
	//select the master drive
	byteout(0x1f6, master_drive | ((LBA >> 24) & 0x0F));
	//request 1 sector
	byteout(0x1f2, 1);
	//send the sector address
	byteout(0x1f3, LBA&0xff);
	byteout(0x1f4, (LBA>>8)&0xff);
	byteout(0x1f5, (LBA>>16)&0xff);

	//send the read command
	byteout(0x1f7, 0x20);

	//poll for ready
	if(!disc_poll_ATA_PIO()){return(false);}
	for(uint16_t i = 0; i<256; i++){
		uint16_t val = wordin(0x1f0);
		sector_address->data[i*2] = val&0xff;
		sector_address->data[i*2+1]=val>>0x8;
		//val = ((val&0xff)<<8) | (val>>8);
		// hexword(val);
		// putchar(' ');
	}
	return(true);
}
