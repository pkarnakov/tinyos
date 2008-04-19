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

; Обработчик прерывания IRQ0
irq0:
pushad
push es
mov ax, gdt_cswr-gdt_0
mov es, ax
mov bl, [es:counttask]
cmp bl, 0
jz .end
mov ah, 0
mov al, [es:curtask]
inc al
div bl
mov byte [es:curtask], ah
mov al, 0
shr ax, 4
add ax, gdt_task0_tss-gdt_0
mov bx, ax
test byte [es:bx+5+gdt_0], 10b
jnz .end
mov word [es:jmpinf+4], ax
mov al, 0x20
out 0x20, al
jmp far [es:jmpinf]
pop es
popad
iretd
.end:
mov al, 0x20
out 0x20, al
pop es
popad
iretd
jmpinf:
dd 0
dw 0
