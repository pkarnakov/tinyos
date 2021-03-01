
%macro GDT_ENTRY 3
  dw ldtsize
  dw (tssbase+tsssize+(%1*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(%1*(sumsize)))>>16)&0xFF
  db 10000010b
  db 0
  db ((tssbase+tsssize+(%1*(sumsize)))>>24)&0xFF
%endmacro

; GDT table

gdt:   ; Пустой дескриптор
  dw 0 
  dw 0
  db 0
  db 0
  dw 0
.gdt: ; Дескриптор таблицы GDT
  dw gdt.end-gdt-1
  dw 0
  db 0
  db 10010010b
  dw 0
.cs:
  dw 0xFFFF
  dw 0
  db 0
  db 10011010b
  dw 0000000001000000b
.ss:
  dw 1023
  dw 0
  db 0x10
  db 10010010b
  dw 0
.ss0:
  dw 1023
  dw 0x400
  db 0x10
  db 10010010b
  dw 0
.ss1:
  dw 1023
  dw 0x800
  db 0x10
  db 10010010b
  dw 0
.ss2:
  dw 1023
  dw 0xC00
  db 0x10
  db 10010010b
  dw 0
.video:
  dw (vibuffersize+20)/4096
  dw 0
  db 0
  db 11110010b
  dw 10000000b
.chars:
  dw 3100
  dw 0
  db 0
  db 10010010b
  dw 0
.idt:
  dw idt_end-idt_0-1
  dw 0
  db 0
  db 10010010b
  dw 0
.picture:
  dw (vibuffersize+1024*128*3)/4096
  dw (tssbase+(10*(sumsize)))&0xFFFF
  db ((tssbase+(10*(sumsize)))>>16)&0xFF
  db 11110010b
  dw (((tssbase+(10*(sumsize)))>>16)&0xFF00)|10000000b
.tssbuf:
  dw (sumsize*10)/4096
  dw tssbase&0xFFFF
  db (tssbase>>16)&0xFF
  db 10010010b
  dw 10000000b
.tss:
  dw tss.end-tss-1
  dw 0
  db 0
  db 10001001b
  dw 0

%macro GDT_LDT 1
  dw ldtsize
  dw (tssbase+tsssize+(%1*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(%1*(sumsize)))>>16)&0xFF
  db 10000010b
  db 0
  db ((tssbase+tsssize+(%1*(sumsize)))>>24)&0xFF
%endmacro

%macro GDT_TSS 1
  dw tsssize-1
  dw (tssbase+(%1*(sumsize)))&0xFFFF
  db ((tssbase+(%1*(sumsize)))>>16)&0xFF
  db 10001001b
  db 00000000b
  db ((tssbase+(%1*(sumsize)))>>24)&0xFF
%endmacro

.task0_ldt:
  GDT_LDT 0
.task0_tss:
  GDT_TSS 0
  GDT_LDT 1
  GDT_TSS 1
  GDT_LDT 2
  GDT_TSS 2
  GDT_LDT 3
  GDT_TSS 3
  GDT_LDT 4
  GDT_TSS 4
  GDT_LDT 5
  GDT_TSS 5
  GDT_LDT 6
  GDT_TSS 6
  GDT_LDT 7
  GDT_TSS 7
  GDT_LDT 8
  GDT_TSS 8
  GDT_LDT 9
  GDT_TSS 9
.cswr:
  dw 0xFFFF
  dw 0
  db 0
  db 11110010b
  dw 0000000000000000b
.keystate:
  dw 0
  dw 0x0000
  db 0x00
  db 10010010b
  dw 0
.evideo:
  dw 0xFFFF 
  dw 0
  db 0
  db 11110010b
  dw 10000000b
.end:
