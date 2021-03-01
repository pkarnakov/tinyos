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
	$(QEMU) -drive file=image.bin,format=raw,if=floppy -boot a -drive file=c.img,if=ide,index=3,media=disk,format=raw

image.qcow2: c.img
	qemu-img convert -f raw -O qcow $< $@

floppy.img:
	mkfs.vfat -C "floppy.img" 1440

f.img: floppy.img image.bin
	cat image.bin <(tail -c +$$(($$(stat -c %s image.bin)+1)) floppy.img) > f.img

.SUFFIXES:
.PHONY: qemu

