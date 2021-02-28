image.bin: loader.bin kernel.bin
	cat $^ > $@

loader.bin: loader.asm
	nasm $< -o $@

kernel.bin: kernel.asm $(wildcard *.inc)
	nasm $< -o $@

c.img:
	dd if=/dev/zero bs=1024 count=13M of=c.img

qemu: image.bin c.img
	qemu-system-x86_64 -fda image.bin -boot a -drive file=c.img,if=ide,index=3,media=disk,format=raw

.SUFFIXES:
.PHONY: qemu

