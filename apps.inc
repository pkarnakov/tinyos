; Здесь нужно размещать исходные коды приложений на NASM
app1:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ax, cs
mov ds, ax
mov esi, 0
.for:
  mov ax, si
  call spectral
  mov byte [es:esi*3], cl
  mov byte [es:esi*3+1], bh
  mov byte [es:esi*3+2], bl
  ;mov ecx, 500
  ;loop $
  inc esi
  cmp esi, 786432
  jnz .for
jmp $
;----------------------------------------------------------------------
; функция возвращает цвета спектра Red - bl, Green - bh, Blue - cl
; На входе - ax - аргумент
;----------------------------------------------------------------------
spectral:
and ax, 1111111111b
mov bx, 256
mov edx, 0
div bx
and eax, 0x7
lea edi, [.colortable-app1+eax*4]
jmp near [cs:edi]
.color0:
mov bl, 255
mov bh, dl
mov cl, 0
ret
.color1:
mov bl, 255
sub bl, dl
mov bh, 255
mov cl, 0
ret
.color2:
mov bl, 0
mov bh, 255
mov cl, dl
ret
.color3:
mov bl, 0
mov bh, 255
sub bh, dl
mov cl, 255
ret
.color4:
mov bl, dl
mov bh, 0
mov cl, 255
ret
.color5:
mov bl, 255
mov bh, 0
mov cl, 255
sub cl, dl
ret
; Таблица переходов:
.colortable:
dd .color0-app1
dd .color1-app1
dd .color2-app1
dd .color3-app1
dd .color4-app1
dd .color5-app1
;----------------------------------------------------------------------
app1data:
db 0
app2:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 0
mov byte [es:ebx*3+1], 0
mov byte [es:ebx*3+2], 255
jnz .for
jmp $
app2data:
db 0
app3:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 0
mov byte [es:ebx*3+1], 128
mov byte [es:ebx*3+2], 255
jnz .for
jmp $
app3data:
db 0
app4:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 0
mov byte [es:ebx*3+1], 255
mov byte [es:ebx*3+2], 255
jnz .for
jmp $
app4data:
db 0
app5:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 0
mov byte [es:ebx*3+1], 255
mov byte [es:ebx*3+2], 0
jnz .for
jmp $
app5data:
db 0
app6:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 250
mov byte [es:ebx*3+1], 200
mov byte [es:ebx*3+2], 20
jnz .for
jmp $
app6data:
db 0
app7:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 255
mov byte [es:ebx*3+1], 0
mov byte [es:ebx*3+2], 0
jnz .for
jmp $
app7data:
db 0
app8:
mov ax, gdt_evideo-gdt_0
mov es, ax
mov ebx, 1024*704
.for:
mov ecx, 500
loop $
dec ebx
mov byte [es:ebx*3], 255
mov byte [es:ebx*3+1], 255
mov byte [es:ebx*3+2], 255
jnz .for
jmp $
app8data:
db 0

app9:
mov ax, gdt_evideo-gdt_0 ; Селектор видеобуфера
mov es, ax
mov ax, gdt_picture-gdt_0 ; Селектор памяти с картинкой
mov ds, ax

repeat:
mov edi, 1024*704*3
mov eax, 8
for21:
sub edi, eax
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:edi], mm0 ; данные сразу по 8 байт!
jnz for21

mov ecx, 0x2FFFFFF*2
loop $

mov edi, 1024*704*3*2
mov esi, 1024*704*3
mov eax, 8
for22:
sub edi, 8
sub esi, 8
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:esi], mm0 ; данные сразу по 8 байт!
jnz for22

mov ecx, 0x2FFFFFF*2
loop $

mov edi, 1024*704*3*3
mov esi, 1024*704*3
mov eax, 8
for23:
sub edi, 8
sub esi, 8
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:esi], mm0 ; данные сразу по 8 байт!
jnz for23

mov ecx, 0x2FFFFFF*2
loop $

mov edi, 1024*704*3*4
mov esi, 1024*704*3
mov eax, 8
for24:
sub edi, 8
sub esi, 8
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:esi], mm0 ; данные сразу по 8 байт!
jnz for24

mov ecx, 0x2FFFFFF*2
loop $

mov edi, 1024*704*3*5
mov esi, 1024*704*3
mov eax, 8
for25:
sub edi, 8
sub esi, 8
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:esi], mm0 ; данные сразу по 8 байт!
jnz for25

mov ecx, 0x2FFFFFF*2
loop $

jmp repeat

jmp $
app9data:
tmp: dw 0
append:

; Таблица приложений
; Содержит данные, необходимые для создания TSS
apptable:
;-------------------------------------
dd app1             ; Смещение кода
dd app1data         ; Смещение данных
dd app1data-app1-1  ; Предел кода
dd app2-app1data-1  ; Предел данных
;-------------------------------------
dd app2             ; Смещение кода
dd app2data         ; Смещение данных
dd app2data-app2-1  ; Предел кода
dd app3-app2data-1  ; Предел данных
;-------------------------------------
dd app3             ; Смещение кода
dd app3data         ; Смещение данных
dd app3data-app3-1  ; Предел кода
dd app4-app3data-1  ; Предел данных
;-------------------------------------
dd app4             ; Смещение кода
dd app4data         ; Смещение данных
dd app4data-app4-1  ; Предел кода
dd app5-app4data-1  ; Предел данных
;-------------------------------------
dd app5             ; Смещение кода
dd app5data         ; Смещение данных
dd app5data-app5-1  ; Предел кода
dd app6-app5data-1  ; Предел данных
;-------------------------------------
dd app6             ; Смещение кода
dd app6data         ; Смещение данных
dd app6data-app6-1  ; Предел кода
dd app7-app6data-1  ; Предел данных
;-------------------------------------
dd app7             ; Смещение кода
dd app7data         ; Смещение данных
dd app7data-app7-1  ; Предел кода
dd app8-app7data-1  ; Предел данных
;-------------------------------------
dd app8             ; Смещение кода
dd app8data         ; Смещение данных
dd app8data-app8-1  ; Предел кода
dd app9-app8data-1  ; Предел данных
;-------------------------------------
dd app9             ; Смещение кода
dd app9data         ; Смещение данных
dd app9data-app9-1  ; Предел кода
dd append-app9data-1 ;Предел данных
;-------------------------------------
