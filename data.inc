
; Строки символов для вывода
texte db 'Exception 0x'
textenum db '  ', 0 
clock db '  :  :  ', 0
videotext db '  ', 0
taskerror db 'Process limit exceeded!', 0
filename TIMES 81 db 0

; Сегмент TSS ядра
tss:
  dd 0, 1023
  dd gdt.ss0-gdt, 1023
  dd gdt.ss1-gdt, 1023
  dd gdt.ss2-gdt, 0
  dd code32, 10b
  dd 0, 0
  dd 0, 0
  dd 1023, 0
  dd 0, 0
  dd 0, gdt.cs-gdt, gdt.ss-gdt, 0, 0, 0, 0, 0
.end:

tsswrite dd 0 ; Текущее смещение записи в TSSs

counttask db 0 ; Количество задач

curtask db 0 ; Текущая задача

keystatus db 10000000b
; 1|num lock|caps lock|scroll lock|ctrl|alt|shiftL|shiftR

basecode dd 0 ; Содержит адрес начала кода из реального режима


; Данные для загрузки в GDTR
gdt_ptr:
      dw gdt.end-gdt-1
      dw 0
      dw 0

; Данные для загрузки в IDTR
idt_ptr:
      dw idt_end-idt_0-1
      dw 0
      dw 0
