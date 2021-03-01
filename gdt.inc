; Tаблица GDT
gdt_0:   ; Пустой дескриптор
  dw 0 
  dw 0
  db 0
  db 0
  dw 0
gdt_gdt: ; Дескриптор таблицы GDT
  dw gdt_end-gdt_0-1
  dw 0
  db 0
  db 10010010b
  dw 0
gdt_cs:
  dw 0xFFFF
  dw 0
  db 0
  db 10011010b
  dw 0000000001000000b
gdt_ss:
  dw 1023
  dw 0
  db 0x10
  db 10010010b
  dw 0
gdt_ss0:
  dw 1023
  dw 0x400
  db 0x10
  db 10010010b
  dw 0
gdt_ss1:
  dw 1023
  dw 0x800
  db 0x10
  db 10010010b
  dw 0
gdt_ss2:
  dw 1023
  dw 0xC00
  db 0x10
  db 10010010b
  dw 0
gdt_video:
dw 0xFFFF 
 ;dw (vibuffersize+20)/4096
  dw 0
  db 0
  db 11110010b
  dw 10000000b
gdt_chars:
  dw 3100
  dw 0
  db 0
  db 10010010b
  dw 0
gdt_idt:
  dw idt_end-idt_0-1
  dw 0
  db 0
  db 10010010b
  dw 0
gdt_picture:
dw 0xFFFF 
 ; dw (vibuffersize+1024*128*3)/4096
  dw (tssbase+(10*(sumsize)))&0xFFFF
  db ((tssbase+(10*(sumsize)))>>16)&0xFF
  db 11110010b
  dw (((tssbase+(10*(sumsize)))>>16)&0xFF00)|10000000b
gdt_TSSs:
  dw (sumsize*10)/4096
  dw tssbase&0xFFFF
  db (tssbase>>16)&0xFF
  db 10010010b
  dw 10000000b
gdt_tss:
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
  db 00010000b
  db ((tssbase+(%1*(sumsize)))>>24)&0xFF
%endmacro

gdt_task0_ldt:
  GDT_LDT 0
gdt_task0_tss:
  GDT_TSS 0
gdt_task1_ldt:
  GDT_LDT 1
gdt_task1_tss:
  GDT_TSS 1
gdt_task2_ldt:
  GDT_LDT 2
gdt_task2_tss:
  GDT_TSS 2
gdt_task3_ldt:
  GDT_LDT 3
gdt_task3_tss:
  GDT_TSS 3
gdt_task4_ldt:
  GDT_LDT 4
gdt_task4_tss:
  GDT_TSS 4
gdt_task5_ldt:
  GDT_LDT 5
gdt_task5_tss:
  GDT_TSS 5
gdt_task6_ldt:
  GDT_LDT 6
gdt_task6_tss:
  GDT_TSS 6
gdt_task7_ldt:
  GDT_LDT 7
gdt_task7_tss:
  GDT_TSS 8
gdt_task8_ldt:
  GDT_LDT 8
gdt_task8_tss:
  GDT_TSS 8
gdt_task9_ldt:
  GDT_LDT 9
gdt_task9_tss:
  GDT_TSS 9
gdt_cswr:
  dw 0xFFFF
  dw 0
  db 0
  db 11110010b
  dw 0000000000000000b
gdt_keystate:
  dw 0
  dw 0x0000
  db 0x00
  db 10010010b
  dw 0
gdt_evideo:
  dw 0xFFFF 
  dw 0
  db 0
  db 11110010b
  dw 10000000b
gdt_end:
