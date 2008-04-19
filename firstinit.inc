; Необходимые константы
%idefine sssize 1024 ; размер стека0
%idefine ss0size 1024 ; размер стека1
%idefine ss1size 1024 ; размер стека2
%idefine ss2size 1024 ; размер стека3
%idefine visizew 1024 ; размер экрана по горизонтали
%idefine visizeh 768 ; размер экрана по вертикали
%idefine vibytes 3 ; количество байт на пиксель
%idefine tsssize 104 ; размер TSS
%idefine ldtsize 64 ; размер LDT
%idefine sumsize ldtsize+tsssize+sssize+ss0size+ss1size+ss2size+vibuffersize ;сумма
%idefine vibuffersize visizeh*visizew*vibytes ; Размер видеобуфера
%idefine tssbase 0x101000 ; базовый адрес TSSs (память 0x100000 - для стека ядра)
%idefine timerfreq 100 ; частота переключения задач (таймера)

BITS 16

mov ax, 0x4F02
mov bx, 0x118|0x4000
int 0x10 ; Устанавливаем разрешение 1024*768*24


mov ax, cs
mov es, ax
mov di, 0x8000
mov ax, 0x4F01
mov cx, 0x118|0x4000
int 0x10

; Определим дескриптор видео
mov dword eax, [cs:0x8000+0x28]
mov ecx, eax
add ecx, 1024*64*3
mov word [cs:gdt_video+2], ax
shr eax, 16
mov byte [cs:gdt_video+4], al
mov byte [cs:gdt_video+7], ah

mov word [cs:gdt_evideo+2], cx
shr ecx, 16
mov byte [cs:gdt_evideo+4], cl
mov byte [cs:gdt_evideo+7], ch


;mov ax, 0
;mov es, ax
;mov ax, 0x4F00
;mov di, 0x8000
;int 0x10
;mov si, [es:0x8000+0x0E+0]
;mov ax, [es:0x8000+0x0E+2]
;mov ds, ax
;.for:
;mov ax, 0x4F01
;mov di, 0x8000
;mov cx, [ds:si]
;inc si
;inc si
;int 0x10
;mov bl, [es:0x8000+0x19]
;mov bh, 0
;add bx, [es:0x8000+0x12]
;add bx, [es:0x8000+0x14]
;cmp bx, 1024+768+24
;jnz .for
;mov ax, 0x4F02
;mov bx, cx
;or bx, 0x4000
;int 0x10
;
;; Определим дескриптор видео
;mov dword eax, [es:0x8000+0x28]
;mov ecx, eax
;add ecx, 1024*64*3
;mov word [cs:gdt_video+2], ax
;shr eax, 16
;mov byte [cs:gdt_video+4], al
;mov byte [cs:gdt_video+7], ah
;
;mov word [cs:gdt_evideo+2], cx
;shr ecx, 16
;mov byte [cs:gdt_evideo+4], cl
;mov byte [cs:gdt_evideo+7], ch

; Запрет аппаратных прерываний
cli

xor eax, eax
mov ax, cs
shl eax, 4
mov dword [cs:basecode], eax

; Вычисляем адрес таблицы GDT
  mov ax, cs
  mov bx, gdt_0
  call adress
; Помещаем в дескриптор GDT
  mov byte [cs:gdt_gdt+4], al
  mov word [cs:gdt_gdt+2], bx
; И в область для регистра GDTR
  mov word [cs:gdt_ptr+4], ax
  mov word [cs:gdt_ptr+2], bx

; Вычисляем адрес таблицы IDT
  mov ax, cs
  mov bx, idt_0
  call adress
; Помещаем в дескрипторы
  mov byte [cs:gdt_idt+4], al
  mov word [cs:gdt_idt+2], bx
; И в область для регистра IDTR
  mov word [cs:idt_ptr+4], ax
  mov word [cs:idt_ptr+2], bx

; Вычисляем адрес текущего сегмента
  mov ax, cs
  mov bx, 0
  call adress
; Определяем дескриптор кода
  mov byte [cs:gdt_cs+4],al
  mov word [cs:gdt_cs+2],bx
; Определяем дескриптор кода для записи
  mov byte [cs:gdt_cswr+4],al
  mov word [cs:gdt_cswr+2],bx

; Определим дескриптор символов
  mov ax, cs
  mov bx, chars
  call adress
  mov word [cs:gdt_chars+2], bx
  mov byte [cs:gdt_chars+4], al

; Определим дескриптор TSS
  mov ax, cs
  mov bx, tss
  call adress
  mov word [cs:gdt_tss+2], bx
  mov byte [cs:gdt_tss+4], al

; Определим дескриптор keystate
  mov ax, cs
  mov bx, keystatus
  call adress
  mov word [cs:gdt_keystate+2], bx
  mov byte [cs:gdt_keystate+4], al

; Устанавливаем для IRQ0-IRQ7 номера прерываний 20h-27h
  mov     dx, 0x20
  mov     ah, 32
  call    set_int_ctrlr

; Устанавливаем для IRQ8-IRQ15 номера прерываний 28h-2Fh
  mov     dx, 0xA0
  mov     ah, 40
  call    set_int_ctrlr

; Устанавливаем частоту таймера
	mov   al,0x34            
	out   0x43,al
	mov   al,(1193180 / timerfreq)&0xFF            ;1193180 / 100
	out   0x40,al
	mov   al,((1193180 / timerfreq)>>8)&0xFF               
	out   0x40,al

; Определяем регистр GDTR
  lgdt [dword cs:gdt_ptr]

; Определяем регистр IDTR
  lidt [dword cs:idt_ptr]

; открываем адресную линию A20
  in   al, 92h
  or   al, 2
  out  92h, al

; Переходим в защищенный режим
  mov eax, cr0 ;Получим содержимое CR0
  or eax, 1 ;Установим бит защищенного режима
  mov cr0, eax ;Запишем назад в CR0

jmp gdt_tss-gdt_0:0; Переход на главную задачу

error:
  mov ax, 0xB800              ; адрес видеобуфера
  mov ds, ax
  mov di, 24*80*2             ; инициализируем указатель (последняя строка)
  mov byte [di], 'a'
  jmp $

errorg:
  mov ax, 0xA000              ; адрес видеобуфера
  mov ds, ax
  mov ax, 100
  mov di, 1024 * 10 * 3
  .for:
  mov byte [di], 0
  mov byte [di+1], 0
  mov byte [di+2], 255
  add di, 3
  dec ax
  jnz .for
  jmp $

%include "kernel16.inc"
