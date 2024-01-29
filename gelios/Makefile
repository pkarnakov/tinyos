QEMU = qemu-system-x86_64
IMAGEURL = https://pkarnakov.github.io/tinyos/gelios_hdd.img

default: build

loader.bin: loader.asm
	nasm $< -o $@

kernel.bin: Kernel.asm $(wildcard *.inc)
	nasm $< -o $@

floppy.img: loader.bin kernel.bin
	cat $^ > $@

hdd.img:
	wget $(IMAGEURL) -O $@

build: floppy.img hdd.img

run: floppy.img hdd.img
	$(QEMU) -drive file=floppy.img,format=raw,if=floppy -boot a -drive file=hdd.img,if=ide,index=0,media=disk,format=raw

clean:
	rm -f image.bin floppy.img loader.bin kernel.bin

.SUFFIXES:
.PHONY: run clean build default
