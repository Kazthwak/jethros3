.PHONY: run clean

default: run

run: jethros.img
	@rm serial.log -f
	@qemu-system-x86_64 -drive format=raw,file=jethros.img,media=disk -boot menu=off -serial file:serial.log -m 1G \
	-drive if=pflash,format=raw,readonly=on,file=./grub_files/OVMF_CODE.4m.fd -drive if=pflash,format=raw,file=./grub_files/OVMF_VARS.4m.fd

jethros.img: jethros.bin BOOTX64.EFI makefile
	@cp jethros.bin ./disc/boot/jethr.os
	@dd if=/dev/zero of=jethros.img bs=1M count=64
	@parted jethros.img --script mklabel gpt mkpart ESP fat32 1MiB 100% set 1 esp on
	@mformat -i jethros.img@@1048576 -F ::
	@mdir -i jethros.img@@1048576 ::/EFI >/dev/null 2>&1 || mmd -i jethros.img@@1048576 ::/EFI
	@mdir -i jethros.img@@1048576 ::/EFI/BOOT >/dev/null 2>&1 || mmd -i jethros.img@@1048576 ::/EFI/BOOT
	@mcopy -i jethros.img@@1048576 BOOTX64.EFI ::/EFI/BOOT/
	@mcopy -i jethros.img@@1048576 -s ./disc/* ::

BOOTX64.EFI: grub_files/* makefile
	@grub-mkstandalone -O x86_64-efi -o BOOTX64.EFI --modules="part_gpt fat normal configfile linux multiboot efi_gop efi_uga multiboot2" "boot/grub/grub.cfg=grub_files/grub.cfg"

kernel_asm.o: kernel.asm
	@nasm kernel.asm -o kernel_asm.o -f elf32

boot.o: boot.asm
	@nasm boot.asm -o boot.o -f elf32

kernel.o: kernel.c kernel.h c/IO.c c/graphics.c c/gdt.c c/idt.c c/interrupts.c c/utils.c c/keyboard.c c/time.c c/debug.c c/load_prog.c \
c/text.c c/init.c c/mem.c c/font.h c/disc.c c/time.c c/scancodes.h c/malloc.c c/disc.h makefile c/vector.h lib/string-code.h lib/stdlib-code.h c/syscall.c c/shell.c
	@~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -ffreestanding -O3 -nostdlib -lgcc -Wall -Wextra -IFAT
	@#-std=gnu99 -Wall -Wextra
	
jethros.bin: kernel_asm.o boot.o kernel.o makefile FAT/lib/libfatfs.a
	@~/opt/cross/bin/i686-elf-gcc -T ./linker.ld -o jethros.bin -ffreestanding -O2 -nostdlib boot.o kernel_asm.o \
	kernel.o FAT/lib/libfatfs.a -lgcc

clean:
	@rm boot.o BOOTX64.EFI jethros.img kernel.o kernel_asm.o serial.log

FAT/lib/libfatfs.a: 
	@echo FAT_LIB_REBUILT
	@make -C FAT clean
	@make -C FAT GCC_PREFIX=~/opt/cross/bin/i686-elf- COMPILER=gcc SHARED_LIB=no EXTRA_CFLAGS="-DFAT_PRINTF_NOINC_STDIO -I./lib"
