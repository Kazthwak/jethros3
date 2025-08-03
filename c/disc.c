#include "../kernel.h"

//fatfs stuff

/*
void disc_init_fat_32(){
	struct disc_sector boot_sector;
	if(!disc_read(&boot_sector, 0x0)){
		filesystem = 0xff;
		print_string("Disc initialisation failed!\n");
		return;
	}
	//load structs into permanant ones
	struct fat_BPB* tmp_fat_BPB = (struct fat_BPB*)&boot_sector;
	struct fat_32_EBPB* tmp_fat_EBPB = (struct fat_32_EBPB*)(((uint32_t)&boot_sector)+sizeof(struct fat_BPB));
	//silence the compiler. sector cannot be uninitialised, as that would be caught by the disc_read function returning false
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
	disc_BPB = *tmp_fat_BPB;
	disc_EBPB_32 = *tmp_fat_EBPB;
	#pragma GCC diagnostic pop
	if(disc_BPB.bytes_per_sector != 0x200){
		print_string("Disc initialisation failed. Sector size is not 512 bytes\n");
		filesystem = 0xff;
		return;
	}
	//set the total_sectors variable, based on wether the number_sectors field is 0
	if(disc_BPB.number_sectors == 0){
		total_sectors = disc_BPB.large_sector_count;
	}else{
		total_sectors = disc_BPB.number_sectors;
	}
	fat_sector_location = disc_BPB.reserved_sectors;
	//this is returning the EBPB
	disc_read(&boot_sector, ((disc_EBPB_32.cluster_root_directory - 2) * disc_BPB.sectors_per_cluster) + (disc_BPB.reserved_sectors + (disc_BPB.number_FATs * disc_EBPB_32.FAT_size) + 0));
	dump_mem(&boot_sector, 0x200);
}
*/



bool disc_poll_ATA_PIO(){
	
	//400ns delay
	for(volatile uint8_t i = 0; i < 14; i++){
		bytein(0x1f7);
	}
	//1 second deadline. Should work as long as TIMER_HZ is less than 2^16
	uint16_t deadline = time + TIMER_FREQUENCY*1;
	while(time < deadline){
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

bool disc_read_ATA_PIO(volatile struct disc_sector* sector_address, uint32_t LBA){
	//starting disc read
	//select the master drive
	byteout(0x1f6, slave_drive | ((LBA >> 24) & 0x0F));
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
