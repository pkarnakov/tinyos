QEMU = qemu-system-x86_64

image.bin: loader.bin kernel.bin
	cat $^ > $@

loader.bin: loader.asm
	nasm $< -o $@

kernel.bin: Kernel.asm $(wildcard *.inc)
	nasm $< -o $@

c.img:
	dd if=/dev/zero bs=1024 count=13M of=c.img

qemu: image.bin c.img
	$(QEMU) -drive file=image.bin,format=raw,if=floppy -boot a -drive file=c.img,if=ide,index=0,media=disk,format=raw

.SUFFIXES:
.PHONY: qemu

