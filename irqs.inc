; Обработчик прерываний IRQ2-IRQ7
irq07:
push ax
mov al, 0x20
out 0x20, al
pop ax
iretd 

; Обработчик прерываний IRQ8-IRQ15
irq815:
push ax
mov al, 0x20
out 0xA0, al
pop ax
iretd

; Обработчик прерывания IRQ1

%include "irq1new.inc"

; %1: number
; %2: charater offset
%macro printchar 2
  pushad
  mov al, %2
  mov ah, 8 * 3
  mul ah
  mov bl, %1
  add bl, '0'
  mov esi, chars
  mov edi, 0
  add di, ax
  mov cl, 0
  mov ch, 255
  int 0x31
  popad
%endmacro

%macro printhex 2
  push es
  push fs
  pushad
  mov ax, ds
  mov es, ax
  mov fs, ax
  mov eax, %1
  mov ebx, 16
  mov edi, tss_str
  int 0x33
  mov ah, 8 * 3
  mov al, %2
  mul ah
  mov edi, 0
  add di, ax
  mov ebp, tss_str
  int 0x32
  popad
  pop fs
  pop es
%endmacro

; Обработчик прерывания IRQ0
irq0:
  push ax
  push bx

  mov bl, [cs:counttask]
  cmp bl, 0
  jz .end

  mov ah, 0
  mov al, [cs:curtask]
  inc al
  div bl
  mov [cs:curtask], ah
  mov bx, 0
  mov bl, ah
  shl bx, 4
  lea bx, [gdt.task0_tss-gdt + bx]

  test byte [cs:gdt+bx+5], 10b
  ; if task busy, return
  jnz .end

  ; else swith to next task
  mov word [cs:jmpinf+4], bx
  mov al, 0x20
  out 0x20, al
  pop bx
  pop ax
  jmp far [cs:jmpinf]
  iretd

  .end:
  mov al, 0x20
  out 0x20, al
  pop bx
  pop ax
  iretd

jmpinf:
dd 0
dd 0
