;===============================================================================
;                           Константы
;===============================================================================

%define Base       0x100000-StartGelios     ; База для всех адресов
%define Base2       0x10000-StartGelios     ; База для всех адресов
%define KernelPtr  Base+StartGelios         ; Адрес начала ядра
%define GDTPtr     Base2+gdt_0               ; Адрес GDT
%define IDTPtr     Base2+idt_0               ; Адрес IDT
%define BufferPtr  0x3000                   ; Сегмент буфера реального режима
%define StackSize  1024*16                  ; Размер стека ядра в байтах
%define Stack      Base+Stackt              ; Адрес стека
%define Stack0     Base+Stackt+StackSize    ; Адрес стека0
%define Stack1     Base+Stackt+StackSize*2  ; Адрес стека1
%define Stack2     Base+Stackt+StackSize*3  ; Адрес стека2
%define timerfreq  100                      ; Частота таймера
%define PITPtr     0x200000                 ; Адрес PIT
%define LastByte   PITPtr+0xFFFFF           ; Последний байт ядра
%define StackVM    Base+StackVM1             ; Адрес стека VM86
%define KernelVMPtr Base+VM86Code           ; Адрес кода VM86
%define MyTSSPtr   Base+MyTSS               ; Адрес TSS VM86
%define KernelVMPtr Base+RealSegment
%define PUSHAD_SIZE 32                    ; размер данных PUSHAD/POPAD