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
  dw tss_end-tss-1
  dw 0
  db 0
  db 10001001b
  dw 0
gdt_task0_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize)&0xFFFF
  db ((tssbase+tsssize)>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize)>>16)&0xFF00
gdt_task0_tss:
  dw tsssize-1
  dw (tssbase)&0xFFFF
  db ((tssbase)>>16)&0xFF
  db 10001001b
  dw ((tssbase)>>16)&0xFF00
gdt_task1_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+sumsize)&0xFFFF
  db ((tssbase+tsssize+sumsize)>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+sumsize)>>16)&0xFF00
gdt_task1_tss:
  dw tsssize-1
  dw (tssbase+sumsize)&0xFFFF
  db ((tssbase+sumsize)>>16)&0xFF
  db 10001001b
  dw ((tssbase+sumsize)>>16)&0xFF00
gdt_task2_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(2*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(2*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(2*(sumsize)))>>16)&0xFF00
gdt_task2_tss:
  dw tsssize-1
  dw (tssbase+(2*(sumsize)))&0xFFFF
  db ((tssbase+(2*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(2*(sumsize)))>>16)&0xFF00
gdt_task3_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(3*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(3*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(3*(sumsize)))>>16)&0xFF00
gdt_task3_tss:
  dw tsssize-1
  dw (tssbase+(3*(sumsize)))&0xFFFF
  db ((tssbase+(3*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(3*(sumsize)))>>16)&0xFF00
gdt_task4_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(4*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(4*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(4*(sumsize)))>>16)&0xFF00
gdt_task4_tss:
  dw tsssize-1
  dw (tssbase+(4*(sumsize)))&0xFFFF
  db ((tssbase+(4*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(4*(sumsize)))>>16)&0xFF00
gdt_task5_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(5*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(5*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(5*(sumsize)))>>16)&0xFF00
gdt_task5_tss:
  dw tsssize-1
  dw (tssbase+(5*(sumsize)))&0xFFFF
  db ((tssbase+(5*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(5*(sumsize)))>>16)&0xFF00
gdt_task6_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(6*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(6*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(6*(sumsize)))>>16)&0xFF00
gdt_task6_tss:
  dw tsssize-1
  dw (tssbase+(6*(sumsize)))&0xFFFF
  db ((tssbase+(6*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(6*(sumsize)))>>16)&0xFF00
gdt_task7_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(7*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(7*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(7*(sumsize)))>>16)&0xFF00
gdt_task7_tss:
  dw tsssize-1
  dw (tssbase+(7*(sumsize)))&0xFFFF
  db ((tssbase+(7*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(7*(sumsize)))>>16)&0xFF00
gdt_task8_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(8*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(8*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(8*(sumsize)))>>16)&0xFF00
gdt_task8_tss:
  dw tsssize-1
  dw (tssbase+(8*(sumsize)))&0xFFFF
  db ((tssbase+(8*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(8*(sumsize)))>>16)&0xFF00
gdt_task9_ldt:
  dw ldtsize-1
  dw (tssbase+tsssize+(9*(sumsize)))&0xFFFF
  db ((tssbase+tsssize+(9*(sumsize)))>>16)&0xFF
  db 10000010b
  dw ((tssbase+tsssize+(9*(sumsize)))>>16)&0xFF00
gdt_task9_tss:
  dw tsssize-1
  dw (tssbase+(9*(sumsize)))&0xFFFF
  db ((tssbase+(9*(sumsize)))>>16)&0xFF
  db 10001001b
  dw ((tssbase+(9*(sumsize)))>>16)&0xFF00
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