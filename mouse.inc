;============================================================
; Набор процедур для работы с мышью PS/2
; и обработчик прерывания IRQ12
;============================================================
irq12:
push es
push ds
pushad
mov ax, gdt.cswr-gdt
mov es, ax
call Wait8042BufferEmpty
in al, 0x60 ; Читаем байт данных от мыши

cmp byte [es:mousepos], 0 ; Смотрим номер байта
jz near byte1
cmp byte [es:mousepos], 1
jz near byte2
mov byte [es:mousedata+2], al
mov byte [es:mousepos], 0

mov esi, [es:currentpos]

mov eax, 0
mov ebx, 0
mov bl, [es:mousedata+1] ; x
mov al, [es:mousedata+2] ; y

test byte [es:mousedata], 00010000b ; Проверяем x-знак
jnz near minusx ; Если минус, то переходим
retx:
add dword [es:currentposx], ebx 
mov ebx, [es:currentposx]

cmp ebx, 0
jl near zerox
retzx:

cmp ebx, 1024
jnl near maxx
retmx:

test byte [es:mousedata], 00100000b ; Проверяем x-знак
jnz near minusy ; Если минус, то переходим
rety:
neg eax ; Меняем знак числа, 
        ; т.к. оси "y" направлены в разные стороны
add dword [es:currentposy], eax
mov eax, [es:currentposy]

cmp eax, 0
jl near zeroy
retzy:

cmp eax, 768
jnl near maxy
retmy:

mov ecx, 1024
mov edx, 0
mul ecx
add eax, ebx
lea eax, [eax*3]
mov dword [es:currentpos], eax
mov ebp, eax ; Получили новый адрес курсора

; Восстанавливаем старые данные
mov ax, gdt.video-gdt
mov gs, ax
xor edi, edi
mov al, 24
mov bl, 24*3
.for_y:
mov ah, bl
.for_x:
mov dl, [es:cache+edi]
mov byte [gs:esi], dl
inc edi
inc esi
dec ah
jnz .for_x
add esi, (1024-24)*3
dec al
jnz .for_y

paintcur:

; Рисуем курсор
xor edi, edi
mov al, 24
.for_y:
mov ah, 24*3
.for_x:
mov ecx, [es:cursor+edi]
mov ebx, [es:mask+edi]
mov edx, [gs:ebp]
mov dword [es:cache+edi], edx
and ecx, ebx
not ebx
and edx, ebx
add ecx, edx
mov dword [gs:ebp], ecx
add ebp, 4
add edi, 4
add esi, 4
sub ah, 4
jnz .for_x
add ebp, (1024-24)*3
add esi, (1024-24)*3
dec al
jnz .for_y

cmp dword [es:currentposy], 64
ja endmouse
mov ebx, [es:currentposx]
shr ebx, 6
mov eax, [es:currentposy]
shr eax, 6
shl eax, 4
add ebx, eax
jmp near [es:mousetable+ebx*4]

endmouse:
mov al, 0x20 ; Разрешаем прерывания
out 0xA0, al
out 0x20, al
popad
pop ds
pop es
iretd

errormouse:
mov byte [es:mousepos], 0
jmp endmouse

byte1:
test al, 00001000b
jz errormouse
inc byte [es:mousepos]
mov byte [es:mousedata], al
jmp endmouse

byte2:
inc byte [es:mousepos]
mov byte [es:mousedata+1], al
jmp endmouse

minusx:
or ebx, 0xFFFFFF00
jmp near retx

minusy:
or eax, 0xFFFFFF00
jmp near rety

zerox:
mov ebx, 0
mov dword [es:currentposx], ebx
jmp retzx

zeroy:
mov eax, 0
mov dword [es:currentposy], eax
jmp retzy

maxx:
mov ebx, 1023
mov dword [es:currentposx], ebx
jmp retmx

maxy:
mov eax, 767
mov dword [es:currentposy], eax
jmp retmy

block0:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+0*16+0]
add ebx, [es:apptable+0*16+4]
mov ecx, [es:apptable+0*16+8]
mov edx, [es:apptable+0*16+12]
int 0x30
jmp endmouse
block1:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+1*16+0]
add ebx, [es:apptable+1*16+4]
mov ecx, [es:apptable+1*6+8]
mov edx, [es:apptable+1*16+12]
int 0x30
jmp endmouse
block2:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+2*16+0]
add ebx, [es:apptable+2*16+4]
mov ecx, [es:apptable+2*16+8]
mov edx, [es:apptable+2*16+12]
int 0x30
jmp endmouse
block3:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+3*16+0]
add ebx, [es:apptable+3*16+4]
mov ecx, [es:apptable+3*16+8]
mov edx, [es:apptable+3*16+12]
int 0x30
jmp endmouse
block4:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+4*16+0]
add ebx, [es:apptable+4*16+4]
mov ecx, [es:apptable+4*16+8]
mov edx, [es:apptable+4*16+12]
int 0x30
jmp endmouse
block5:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+5*16+0]
add ebx, [es:apptable+5*16+4]
mov ecx, [es:apptable+5*16+8]
mov edx, [es:apptable+5*16+12]
int 0x30
jmp endmouse
block6:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+6*16+0]
add ebx, [es:apptable+6*16+4]
mov ecx, [es:apptable+6*16+8]
mov edx, [es:apptable+6*16+12]
int 0x30
jmp endmouse
block7:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+7*16+0]
add ebx, [es:apptable+7*16+4]
mov ecx, [es:apptable+7*16+8]
mov edx, [es:apptable+7*16+12]
int 0x30
jmp endmouse
block8:
test byte [es:mousedata], 1
jz near endmouse
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+8*16+0]
add ebx, [es:apptable+8*16+4]
mov ecx, [es:apptable+8*16+8]
mov edx, [es:apptable+8*16+12]
int 0x30
jmp endmouse
block9:
test byte [es:mousedata], 1
jz near endmouse
mov ax, gdt.cswr-gdt
mov es, ax
mov dword [es:tsswrite], 0
mov byte [es:counttask], 0
mov byte [es:curtask], 0
mov al, 0x20 ; Разрешаем прерывания
out 0xA0, al
out 0x20, al
test byte [es:gdt.tss+5], 10b
jnz .next
jmp gdt.tss-gdt:0
.next:
popad
pop ds
pop es
iretd
jmp endmouse
block10:
jmp endmouse
block11:
jmp endmouse
block12:
jmp endmouse
block13:
jmp endmouse
block14:
jmp endmouse
block15:
jmp endmouse
block16:
jmp endmouse
block17:
jmp endmouse
block18:
jmp endmouse
block19:
jmp endmouse
block20:
jmp endmouse
block21:
jmp endmouse
block22:
jmp endmouse
block23:
jmp endmouse
block24:
jmp endmouse
block25:
jmp endmouse
block26:
jmp endmouse
block27:
jmp endmouse
block28:
jmp endmouse
block29:
jmp endmouse
block30:
jmp endmouse
block31:
jmp endmouse
block32:
jmp endmouse
block33:
jmp endmouse
block34:
jmp endmouse
block35:
jmp endmouse
block36:
jmp endmouse
block37:
jmp endmouse
block38:
jmp endmouse
block39:
jmp endmouse
block40:
jmp endmouse
block41:
jmp endmouse
block42:
jmp endmouse
block43:
jmp endmouse
block44:
jmp endmouse
block45:
jmp endmouse
block46:
jmp endmouse
block47:
jmp endmouse

mousedata: db 0, 0, 0
mousepos: db 0
currentposx: dd 1024/2
currentposy: dd 768/2
currentpos: dd (384*1024+512)*3

; Таблица переходов
mousetable:
dd block0
dd block1
dd block2
dd block3
dd block4
dd block5
dd block6
dd block7
dd block8
dd block9
dd block10
dd block11
dd block12
dd block13
dd block14
dd block15
dd block16
dd block17
dd block18
dd block19
dd block20
dd block21
dd block22
dd block23
dd block24
dd block25
dd block26
dd block27
dd block28
dd block29
dd block30
dd block31
dd block32
dd block33
dd block34
dd block35
dd block36
dd block37
dd block38
dd block39
dd block40
dd block41
dd block42
dd block43
dd block44
dd block45
dd block46
dd block47

;============================================================
; Ожидание очистки входного буфера I8042    
; При выходе:    
; флаг ZF установлен - нормальное завершение,
; флаг ZF сброшен - ошибка тайм-аута. 
;------------------------------------------------------------
Wait8042BufferEmpty:
push ecx
mov ecx, 0xFFFF  ;задать число циклов ожидания
kb:
in al, 0x64;получить статус
test    al, 10b;буфер i8042 свободен?
loopnz  kb  ;если нет, то цикл
pop ecx
;Если при выходе из подпрограммы сброшен
;флаг ZF - ошибка
ret	  ;возврат
;============================================================

;============================================================
; Ожидание поступления данных от мыши   
; При выходе:    
; флаг ZF установлен - нормальное завершение, 
; флаг ZF сброшен - ошибка тайм-аута.   
;------------------------------------------------------------
WaitMouseData:
push ecx
mov ecx, 0xFFFF  ;задать число циклов ожидания
mouse:
in al, 0x64;опросить регистр статуса
test    al, 100000b ;данные поступили?
loopz   mouse    ;если нет, то цикл
pop ecx
;Если при выходе из подпрограммы установлен
;флаг ZF - ошибка
ret	  ;возврат
;------------------------------------------------------------

;============================================================
; Инициализация мыши
;------------------------------------------------------------
setmouse:
pushad
   cli
   mov bl, 0xa8  
   call kb_cmd
   call kb_read  
   mov bl, 0x20  
   call kb_cmd
   call kb_read
   or al, 3
   mov bl, 0x60 
   push eax
   call kb_cmd
   pop eax
   call kb_write
   mov bl, 0xd4  
   call kb_cmd
   mov al, 0xf4 
   call kb_write
   call kb_read

; Рисуем курсор
mov ax, gdt.cswr-gdt
mov es, ax
mov ebp, [es:currentpos]
mov ax, gdt.video-gdt
mov gs, ax
mov edi, 0
mov al, 24
.for_y:
mov ah, 24*3
.for_x:
mov cl, [es:cursor+edi]
mov ch, [es:mask+edi]
mov dl, [gs:ebp]
mov byte [es:cache+edi], dl
and cl, ch
not ch
and dl, ch
add cl, dl
mov byte [gs:ebp], cl
inc ebp
inc edi
dec ah
jnz .for_x
add ebp, (1024-24)*3
dec al
jnz .for_y
popad
ret

   kb_read:
   push ecx 
   push edx
   mov ecx, 0xffff
   kr_loop:
   in al, 0x64
   test al, 1
   jnz kr_ready
   loop kr_loop
   mov ah, 1
   jmp kr_exit
   kr_ready:
   push ecx
   mov ecx, 32
   kr_delay:
   loop kr_delay
   pop ecx
   in al, 0x60
   xor ah, ah
   kr_exit:
   pop edx 
   pop ecx
   ret

   kb_write:
   push ecx 
   push edx
   mov dl, al
   mov ecx, 0xffff
   kw_loop1:
   in al, 0x64
   test    al, 0x20
   jz kw_ok1
   loop    kw_loop1
   mov ah, 1
   jmp kw_exit
   kw_ok1:
   in al, 0x60
   mov ecx, 0xffff
   kw_loop:
   in al, 0x64
   test al, 2
   jz kw_ok
   loop kw_loop
   mov ah, 1
   jmp kw_exit
   kw_ok:
   mov al, dl
   out 0x60, al
   mov ecx, 0xffff
   kw_loop3:
   in al, 0x64
   test al, 2
   jz kw_ok3
   loop kw_loop3
   mov ah, 1
   jmp kw_exit
   kw_ok3:
   mov ah, 8
   kw_loop4:
   mov ecx, 0xffff
   kw_loop5:
   in al, 0x64
   test al, 1
   jnz kw_ok4
   loop kw_loop5
   dec ah
   jnz kw_loop4
   kw_ok4:
   xor ah, ah
   kw_exit:
   pop edx 
   pop ecx
   ret

   kb_cmd:
   mov ecx, 0xffff
   c_wait:
   in al, 0x64
   test    al, 2
   jz c_send
   loop c_wait
   jmp c_error
   c_send:
   mov al, bl
   out 0x64, al
   mov ecx, 0xffff
   c_accept:
   in al, 0x64
   test    al, 2
   jz c_ok
   loop c_accept
   c_error:
   mov ah, 1
   jmp c_exit
   c_ok:
   xor ah, ah
   c_exit:
   ret
;============================================================
