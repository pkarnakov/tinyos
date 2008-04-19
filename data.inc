
; Строки символов для вывода
texte db 'Exception 0x'
textenum db '  ', 0 
clock db '  :  :  ', 0
videotext db '  ', 0
taskerror db 'Process limit exceeded!', 0
filename TIMES 81 db 0

; Сегмент TSS ядра
tss: dd 0, 1023, gdt_ss0-gdt_0, 1023, gdt_ss1-gdt_0, 1023, gdt_ss2-gdt_0, 0, code32, 10b, 0, 0, 0, 0, 1023, 0, 0, 0, 0, gdt_cs-gdt_0, gdt_ss-gdt_0, 0, 0, 0, 0, 0 
tss_end:

tsswrite dd 0 ; Текущее смещение записи в TSSs

counttask db 0 ; Количество задач

curtask db 0 ; Текущая задача

keystatus db 10000000b
; 1|num lock|caps lock|scroll lock|ctrl|alt|shiftL|shiftR

basecode dd 0 ; Содержит адрес начала кода из реального режима


; Данные для загрузки в GDTR
gdt_ptr:
      dw gdt_end-gdt_0-1
      dw 0
      dw 0

; Данные для загрузки в IDTR
idt_ptr:
      dw idt_end-idt_0-1
      dw 0
      dw 0
