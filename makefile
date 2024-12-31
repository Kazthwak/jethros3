.PHONY: run clean

default: run

run: jethros.iso
	@rm serial.log -f
	@qemu-system-x86_64 -drive format=raw,file=jethros.iso,media=disk -serial file:serial.log -m 2G

jethros.iso: jethros.bin
	@cp jethros.bin ./isodir/boot/jethr.os
	@chronic grub-mkrescue -o jethros.iso isodir

kernel_asm.o: kernel.asm
	@nasm kernel.asm -o kernel_asm.o -f elf32

boot.o: boot.asm
	@nasm boot.asm -o boot.o -f elf32

kernel.o: kernel.c kernel.h c/IO.c c/graphics.c c/gdt.c c/idt.c c/interrupts.c c/utils.c c/keyboard.c c/time.c c/debug.c \
c/text.c c/init.c c/mem.c c/font.h c/disc.c c/time.c
	@~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

jethros.bin: kernel_asm.o boot.o kernel.o
	@~/opt/cross/bin/i686-elf-gcc -T ./linker.ld -o jethros.bin -ffreestanding -O2 -nostdlib boot.o kernel_asm.o \
	kernel.o -lgcc