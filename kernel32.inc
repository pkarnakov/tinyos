;============================================================
;         Здесь размещены все 32-разрядные функции           
;============================================================

;============================================================
; 1. Процедура выводит символ 
; На входе: bl - номер символа ASCII
; ds:esi - адрес таблицы символов
; gs:edi - адрес видеобуфера
; cl - цвет символа, ch - цвет фона
; На выходе: в видеобуфере содержится символ 
; согласно таблице символов
;------------------------------------------------------------
char:
  pushad
  xor eax, eax
  mov al, 12
  mul bl
  add esi, eax
  mov al, 12
     .for_y:
     mov dl, [ds:esi]
     mov dh, 1,
         .for_x:
         test dl, dh
         jz .emppix
         mov byte [gs:edi], cl
         mov byte [gs:edi+1], cl
         mov byte [gs:edi+2], cl
         .return:
         inc edi
	 inc edi
	 inc edi
         shl dh, 1
         cmp dh, 10000000b
         jnz .for_x
     inc esi
     add edi, 3051
     dec al
     jnz .for_y
 popad
 iretd
  .emppix:
  mov byte [gs:edi], ch
  mov byte [gs:edi+1], ch
  mov byte [gs:edi+2], ch
  jmp .return
;============================================================


;============================================================
; 2. Процедура выводит ASCIIZ-строку 
; gs:edi - указатель на видеобуфер
; fs:ebp - указатель на ASCIIZ-строку
;------------------------------------------------------------
string:
pushad
push es
push ds
mov ax, gdt_chars-gdt_0
mov ds, ax
mov esi, 0
mov ax, gdt_video-gdt_0
mov gs, ax
mov bl, [fs:ebp]
mov cl, 0x0
mov ch, 0xFF
.for:
int 0x31
inc ebp
add edi, 21
mov bl, [fs:ebp]
cmp bl, 0
jnz .for
pop ds
pop es
popad
iretd
;============================================================


;============================================================
; 3. Процедура создает сегмент TSS процесса
; На входе: eax - начало кода, ebx - начало данных
;           ecx - предел кода, edx - предел данных
;------------------------------------------------------------
addtask:
pushad
cmp byte [cs:counttask], 10
jae near .end
push fs
push ds
push es
push eax
mov ax, gdt_TSSs-gdt_0
mov ds, ax
mov ax, gdt_cswr-gdt_0
mov fs, ax
mov esi, [cs:tsswrite]
mov dword [ds:esi], 0    	; LINK
add esi, 4
mov dword [ds:esi], ss0size-1   	; ESP0
add esi, 4
mov word [ds:esi], 11100b   ; SS0
mov word [ds:esi+2], 0
add esi, 4
mov dword [ds:esi], ss1size-1    ; ESP1
add esi, 4
mov word [ds:esi], 100101b    ; SS1
mov word [ds:esi+2], 0
add esi, 4
mov dword [ds:esi], ss2size-1    ; ESP2
add esi, 4
mov word [ds:esi], 101110b    ; SS2
mov word [ds:esi+2], 0  
add esi, 4
mov dword [ds:esi], 0     ; CR3
add esi, 4
mov dword [ds:esi], 0   ; EIP
add esi, 4
mov dword [ds:esi], 1000000010b   ; EFLAGS
add esi, 4
mov dword [ds:esi], 0     ; EAX
add esi, 4
mov dword [ds:esi], 0     ; ECX
add esi, 4
mov dword [ds:esi], 0     ; EDX
add esi, 4
mov dword [ds:esi], 0     ; EBX
add esi, 4
mov dword [ds:esi], sssize-1    ; ESP
add esi, 4
mov dword [ds:esi], 0     ; EBP
add esi, 4
mov dword [ds:esi], 0     ; ESI
add esi, 4
mov dword [ds:esi], 0     ; EDI
add esi, 4
mov dword [ds:esi], 0     ; ES
add esi, 4  
mov word [ds:esi], 1111b    ; CS
mov word [ds:esi+2], 0
add esi, 4
mov word [ds:esi], 10111b   ; SS
mov word [ds:esi+2], 0
add esi, 4
mov word [ds:esi], 0      ; DS
mov word [ds:esi+2], 0
add esi, 4
mov word [ds:esi], 0      ; FS
mov word [ds:esi+2], 0
add esi, 4
mov word [ds:esi], 0      ; GS
mov word [ds:esi+2], 0
add esi, 4
mov ah, 0
mov al, [cs:counttask]
shl ax, 4
add ax, gdt_task0_ldt-gdt_0
mov word [ds:esi], ax     ; LDTR
mov word [ds:esi+2], 0
add esi, 4
mov word [ds:esi], 0      ; I/O
mov word [ds:esi+2], 0xFFFF
add esi, 4
mov dword [ds:esi], 0     ; LDT_0
mov dword [ds:esi+4], 0
add esi, 8
pop eax
mov word [ds:esi], cx     ; LDT_CS
shr ecx, 16
and cl, 00001111b
or cl,  01000000b
mov word [ds:esi+2], ax
shr eax, 16          
mov byte [ds:esi+4], al
mov ch, ah
mov word [ds:esi+6], cx
mov byte [ds:esi+5], 11111010b
add esi, 8
mov word [ds:esi], sssize-1     ; LDT_SS
mov eax, [cs:tsswrite]
add eax, ldtsize+tsssize
mov cx, gdt_gdt-gdt_0
mov es, cx
mov ch, [es:gdt_TSSs-gdt_0+7]
mov cl, [es:gdt_TSSs-gdt_0+4]
shl ecx, 16
mov cx, [es:gdt_TSSs-gdt_0+2]
add eax, ecx
mov word [ds:esi+2], ax
shr eax, 16
mov byte [ds:esi+4], al
mov al, 0
mov word [ds:esi+6], ax
mov byte [ds:esi+5], 11110010b
add esi, 8
mov word [ds:esi], ss0size-1     ; LDT_SS0
mov eax, [cs:tsswrite]
add eax, ldtsize+tsssize+sssize
mov cx, gdt_gdt-gdt_0
mov es, cx
mov ch, [es:gdt_tss-gdt_0+7]
mov cl, [es:gdt_tss-gdt_0+4]
shl ecx, 16
mov cx, [es:gdt_tss-gdt_0+2]
add eax, ecx
mov word [ds:esi+2], ax
shr eax, 16
mov byte [ds:esi+4], al
mov al, 0
mov word [ds:esi+6], ax
mov byte [ds:esi+5], 10010010b
add esi, 8
mov word [ds:esi], ss1size-1    ; LDT_SS1
mov eax, [cs:tsswrite]
add eax, ldtsize+tsssize+sssize+ss0size
mov cx, gdt_gdt-gdt_0
mov es, cx
mov ch, [es:gdt_tss-gdt_0+7]
mov cl, [es:gdt_tss-gdt_0+4]
shl ecx, 16
mov cx, [es:gdt_tss-gdt_0+2]
add eax, ecx
mov word [ds:esi+2], ax
shr eax, 16
mov byte [ds:esi+4], al
mov al, 0
mov word [ds:esi+6], ax
mov byte [ds:esi+5], 10110010b
add esi, 8
mov word [ds:esi], ss2size-1     ; LDT_SS2
mov eax, [cs:tsswrite]
add eax, ldtsize+tsssize+sssize+ss0size+ss1size
mov cx, gdt_gdt-gdt_0
mov es, cx
mov ch, [es:gdt_tss-gdt_0+7]
mov cl, [es:gdt_tss-gdt_0+4]
shl ecx, 16
mov cx, [es:gdt_tss-gdt_0+2]
add eax, ecx
mov word [ds:esi+2], ax
shr eax, 16
mov byte [ds:esi+4], al
mov al, 0
mov word [ds:esi+6], ax
mov byte [ds:esi+5], 11010010b
add esi, 8
		; LDT_video
mov word [ds:esi], vibuffersize/4096 
mov eax, [cs:tsswrite]
add eax, ldtsize+tsssize+sssize+ss0size+ss1size+ss2size
mov cx, gdt_gdt-gdt_0
mov es, cx
mov ch, [es:gdt_tss-gdt_0+7]
mov cl, [es:gdt_tss-gdt_0+4]
shl ecx, 16
mov cx, [es:gdt_tss-gdt_0+2]
add eax, ecx
mov word [ds:esi+2], ax
shr eax, 16
mov byte [ds:esi+4], al
mov al, 10000000b
mov word [ds:esi+6], ax
mov byte [ds:esi+5], 11110010b
add esi, 8
mov word [ds:esi], dx     ; LDT_DS
shr edx, 16
and dl, 00001111b
mov word [ds:esi+2], bx
shr ebx, 16          
mov byte [ds:esi+4], bl
mov dh, bh
mov word [ds:esi+6], dx
mov byte [ds:esi+5], 11110010b
add dword [fs:tsswrite], sumsize
inc byte [fs:counttask]
pop es
pop ds
pop fs
popad
iretd
.end:
push fs
mov ax, gdt_cswr-gdt_0
mov fs, ax
mov ebp, taskerror
mov edi, visizew*vibytes*300
int 0x32
pop fs
popad
iretd
;============================================================

;============================================================
; 4. Функция преобразует число в строку:
; На входе: eax - число
; ebx - основание системы счисления
; es:edi - указатель на строку - результат
; На выходе - заполненный буфер
;------------------------------------------------------------
NumToASCII:
pushad
xor esi, esi
convert_loop:
xor edx, edx
div ebx
call HexDigit
push edx
inc esi
test eax, eax
jnz convert_loop
cld
write_loop:
pop eax
stosb
dec esi
test esi, esi
jnz write_loop
popad
iretd

HexDigit:
cmp dl, 10
jb .less
add dl, 'A'-10
ret
.less:
add dl, '0'
ret
;============================================================

;============================================================
; 5. Функция преобразует строку в число:
; На входе: 
; ecx - основание системы счисления
; es:esi - указатель на ASCIIZ-строку
; На выходе: eax - число
;------------------------------------------------------------
ASCIIToNum:
push esi
xor eax, eax
mov ebx, eax
.next:
lodsb
test al, al
jz .done
call convert_char
imul ebx, ecx
add ebx, eax
jmp short .next
.done:
xchg ebx, eax
pop esi
iretd

convert_char:
sub al, '0'
cmp al, 10
jb done
add al, '0'
and al, 0x5F
sub al, 'A'-10
and al, 0x0F
done:
ret
;============================================================

;============================================================
; 6. Процедура читает данные с диска IDE
; На входе: esi - LBA-адрес
; es:edi - адрес буфера
; dx - базовый порт контроллера (0x1F0, 0x170)
; cl - количество секторов для чтения
; На выходе: в буфере будут находиться данные с диска
;------------------------------------------------------------
readsectors:
push ds
pushad
mov ax, es
mov ds, ax
or esi, 0xE0000000
mov bx, dx
add dx, 0x206
mov al, 10b
out dx, al
mov dx, bx
call bsywait
add dx, 6
mov eax, esi
shr eax, 24
out dx, al
mov dx, bx
call freewait
add dx, 3
mov eax, esi
out dx, al
shr eax, 8
inc dx
out dx, al
shr eax, 8
inc dx
out dx, al
mov al, cl
mov dx, bx
add dx, 2
out dx, al
add dx, 5
mov al, 0x20
out dx, al
mov dx, bx
call bsywait
call drqwait
cld
and ecx, 0xFF
xchg cl, ch
mov esi, edi
rep insw
mov dx, bx
add dx, 0x206
mov al, 0
out dx, al
popad
pop ds
iretd

bsywait:
push ax
push bx
push dx
add dx, 7
mov bl, 10000000b
.wait:
in al, dx
test al, bl
jnz .wait
pop dx
pop bx
pop ax
ret

freewait:
push ax
push bx
push dx
add dx, 7
mov bl, 01000000b
mov bh, 11000000b
.wait:
in al, dx
and al, bh
cmp al, bl
jnz .wait
pop dx
pop bx
pop ax
ret

drqwait:
push ax
push bx
push dx
add dx, 7
mov bl, 1000b
.wait:
in al, dx
test al, bl
jz .wait
pop dx
pop bx
pop ax
ret
;============================================================

;============================================================
;			О Ш И Б К А ! ! ! ! ! ! !
; 7. Процедура записывает данные на диск IDE
; На входе: esi - LBA-адрес
; es:edi - адрес буфера
; dx - базовый порт контроллера (0x1F0, 0x170)
; cl - количество секторов для записи
; На выходе: на диске будут находиться данные из буфера
;------------------------------------------------------------
writesectors:
iretd
push ds
pushad
mov ax, es
mov ds, ax
or esi, 0xE0000000
mov bx, dx
add dx, 0x206
mov al, 10b
out dx, al
mov dx, bx
call bsywait1
add dx, 6
mov eax, esi
shr eax, 24
out dx, al
mov dx, bx
call freewait1
add dx, 3
mov eax, esi
out dx, al
shr eax, 8
inc dx
out dx, al
shr eax, 8
inc dx
out dx, al
mov al, cl
mov dx, bx
add dx, 2
out dx, al
add dx, 5
mov al, 0x20
out dx, al
mov dx, bx
call bsywait1
call drqwait1
cld
and ecx, 0xFF
xchg cl, ch
mov esi, edi
rep outsw
mov dx, bx
add dx, 0x206
mov al, 0
out dx, al
popad
pop ds
iretd

bsywait1:
push ax
push bx
push dx
add dx, 7
mov bl, 10000000b
.wait:
in al, dx
test al, bl
jnz .wait
pop dx
pop bx
pop ax
ret

freewait1:
push ax
push bx
push dx
add dx, 7
mov bl, 01000000b
mov bh, 11000000b
.wait:
in al, dx
and al, bh
cmp al, bl
jnz .wait
pop dx
pop bx
pop ax
ret

drqwait1:
push ax
push bx
push dx
add dx, 7
mov bl, 1000b
.wait:
in al, dx
test al, bl
jz .wait
pop dx
pop bx
pop ax
ret
;============================================================

;============================================================
; 8. Процедура читает данные из файла
; На входе: es:edi - адрес буфера
; ebx:esi - адрес файла
; eax - кол-во секторов для операции
; На выходе: в буфере будут находится данные из файла
; или в файле будут данные из буфера
;------------------------------------------------------------
readfile:
pushad
push eax
push edi
push es
mov esi, ebx
shr esi, 6
inc esi
or esi, 0xF0000000
mov ax, gdt_cswr-gdt_0
mov es, ax
mov edi, selbuf
mov dx, 0x170
mov cl, 1
int 0x35
mov esi, ebx
shr esi, 4
and esi, 11b
shl esi, 7
mov esi, [es:esi+selbuf+84]
or esi, 0xF0000000
mov dx, 0x170
mov cl, 1
pop es
pop edi
pop eax
.for:
int 0x35
inc esi
add edi, 512
dec eax
jnz .for
popad
iretd

selbuf: TIMES 512 db 0
;============================================================

;============================================================
; 9. Процедура читает имя файла
; На входе: ds:edi - адрес буфера (81 байт)
; ebx - файловый селектор
; На выходе: в буфере будет содержаться имя файла
;------------------------------------------------------------
namefile:
pushad
push es
push edi
mov esi, ebx
shr esi, 6
inc esi
or esi, 0xF0000000
mov ax, gdt_cswr-gdt_0
mov es, ax
mov edi, selbuf2
mov dx, 0x170
mov cl, 1
int 0x35
mov esi, ebx
shr esi, 4
and esi, 11b
shl esi, 7
add esi, selbuf2
mov ecx, 80
pop edi
.for:
mov al, [es:esi]
mov byte [ds:edi], al
inc esi
inc edi
loop .for
mov byte [ds:edi], 0
pop es
popad
iretd

selbuf2: TIMES 512 db 0
;============================================================

;============================================================
;********************** Разработка **************************
; 10. Процедура читает данные с устройства IDE-ATAPI
; На входе: esi - LBA-адрес
; es:edi - адрес буфера
; dx - базовый порт контроллера (primary: 0x1F0, secondary: 0x170)
; ecx - количество секторов для чтения
; На выходе: в буфере будут находиться данные с диска
;------------------------------------------------------------
readsectorscd:
push ds
pushad
mov ax, es
mov ds, ax
or esi, 0xE0000000
mov bx, dx
add dx, 0x206
mov al, 10b
out dx, al
mov dx, bx
call bsywaitcd
add dx, 6
mov eax, esi
shr eax, 24
out dx, al
mov dx, bx
call freewaitcd
add dx, 3
mov eax, esi
out dx, al
shr eax, 8
inc dx
out dx, al
shr eax, 8
inc dx
out dx, al
mov al, cl
mov dx, bx
add dx, 2
out dx, al
add dx, 5
mov al, 0xA0
out dx, al
mov dx, bx
call bsywaitcd
call drqwaitcd
mov ax, gdt_cswr-gdt_0
mov fs, ax
mov eax, esi
mov byte [fs:packet+5], al
mov byte [fs:packet+4], ah
shr eax, 16
mov byte [fs:packet+3], al
xor ah, ah
mov byte [fs:packet+2], ah



mov eax, ecx

mov byte [fs:packet+9], al
mov byte [fs:packet+8], ah
shr eax, 16
mov byte [fs:packet+7], al
mov byte [fs:packet+6], ah

mov ax, [cs:packet+0]
out dx, ax
mov ax, [cs:packet+2]
out dx, ax
mov ax, [cs:packet+4]
out dx, ax
mov ax, [cs:packet+6]
out dx, ax
mov ax, [cs:packet+8]
out dx, ax
mov ax, [cs:packet+10]
out dx, ax
cld
shl ecx, 10
mov esi, edi
rep insw
mov dx, bx
add dx, 0x206
mov al, 0
out dx, al
popad
pop ds
iretd

packet:
db 0xA8
TIMES 11 db 0

bsywaitcd:
push ax
push bx
push dx
add dx, 7
mov bl, 10000000b
.wait:
in al, dx
test al, bl
jnz .wait
pop dx
pop bx
pop ax
ret

freewaitcd:
push ax
push bx
push dx
add dx, 7
mov bl, 01000000b
mov bh, 11000000b
.wait:
in al, dx
and al, bh
cmp al, bl
jnz .wait
pop dx
pop bx
pop ax
ret

drqwaitcd:
push ax
push bx
push dx
add dx, 7
mov bl, 1000b
.wait:
in al, dx
test al, bl
jz .wait
pop dx
pop bx
pop ax
ret
;============================================================
