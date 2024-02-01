# TinyOS

A prototype operating system in x86 assembly. Features:

* 32-bit protected mode
* multitasking
* video mode 1024x768x24 through VESA
* mouse support
* custom filesystem on IDE devices

This is an old project from 2008 started for a school competition.

## Clone

```
git clone https://github.com/pkarnakov/tinyos.git
```

## Requirements

* [NASM](https://nasm.us) - the Netwide Assembler
* [QEMU](https://www.qemu.org) - emulator
* [TigerVNC](https://tigervnc.org) - VNC client to connect to QEMU

## Build and run

Build the floppy disk image using NASM and download the hard drive image (13M)
```
$ make build
```

Run QEMU with the images
```
$ make run
VNC server running on ::1:5900
```

Connect using TigerVNC
```
$ vncviewer :5900
```

## Screencast

<a href="http://pkarnakov.github.io/tinyos/media/tinyos.mp4"><img src="https://pkarnakov.github.io/tinyos/media/tinyos.gif" height=200 alt="tinyos.mp4"></a>
