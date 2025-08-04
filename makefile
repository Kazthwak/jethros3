.PHONY: run clean

default: run

run: jethros.iso hard_drive
	@rm serial.log -f
	@qemu-system-x86_64 -drive format=raw,file=jethros.iso,media=disk -drive format=raw,file=hard_drive,media=disk \
	-serial file:serial.log -m 1G -boot menu=off

hard_drive: disc/*
	@rm -f hard_drive
	@#echo "File blank_disc not found. It is a 64M file of 0s
	@cp blank_disc hard_drive
	@mcopy disc/* hard_drive

jethros.iso: jethros.bin
	@cp jethros.bin ./isodir/boot/jethr.os
	@grub-mkrescue -o jethros.iso isodir > /dev/null

kernel_asm.o: kernel.asm
	@nasm kernel.asm -o kernel_asm.o -f elf32

boot.o: boot.asm
	@nasm boot.asm -o boot.o -f elf32

kernel.o: kernel.c kernel.h c/IO.c c/graphics.c c/gdt.c c/idt.c c/interrupts.c c/utils.c c/keyboard.c c/time.c c/debug.c \
c/text.c c/init.c c/mem.c c/font.h c/disc.c c/time.c c/scancodes.h c/malloc.c c/disc.h
	@~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

jethros.bin: kernel_asm.o boot.o kernel.o
	@~/opt/cross/bin/i686-elf-gcc -T ./linker.ld -o jethros.bin -ffreestanding -O2 -nostdlib boot.o kernel_asm.o \
	kernel.o -lgcc
