;============================================================
;                Проект операционной системы.           
;============================================================
;    Автор - Карнаков Петр                             
;    Дата начала - 8.11.07                                
;    Дата окончания - ?                                  
;    ОС защищенного режима: 32-разрядный код,            
;    многозадачность, работа с жестким диском и мышью     
;    Реализована файловая система                      
;============================================================
   org 0
;============================================================
;                          Ядро
;============================================================
start_os_kernel:

; Здесь производится начальная  инициализация системы:
; Инициализация GDTR и IDTR, GDT,
; установка видеорежима 1024*768*24

%include "firstinit.inc"  
  
;============================================================
; С этой метки идет 32-разрядный код!
;============================================================
BITS 32
code32:
mov ax, gdt_picture-gdt_0
mov es, ax
mov ds, ax
mov ebx, 100000b
mov edi, 0
mov esi, 0
mov eax, 704*3*2
int 0x37

mov ax, gdt_evideo-gdt_0 ; Селектор видеобуфера
mov es, ax
mov ax, gdt_picture-gdt_0 ; Селектор памяти с картинкой
mov ds, ax
mov edi, 1024*704*3
mov eax, 8
for:
sub edi, eax
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:edi], mm0 ; данные сразу по 8 байт!
jnz for

mov ax, gdt_picture-gdt_0
mov es, ax
mov ds, ax
mov ebx, 110000b
mov esi, 0
mov edi, 0
mov eax, 2*64*3
int 0x37

mov ax, gdt_video-gdt_0 ; Селектор видеобуфера
mov es, ax
mov ax, gdt_picture-gdt_0 ; Селектор памяти с картинкой
mov ds, ax
mov edi, 1024*64*3
mov esi, 1024*64*3
mov eax, 8
for1:
sub edi, eax
sub esi, eax
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:esi], mm0 ; данные сразу по 8 байт!
jnz for1

call setmouse

mov al, 11111000b
out 0x21, al
mov al, 11101111b
out 0xA1, al

sti

mov ax, gdt_cswr-gdt_0
mov ds, ax
mov es, ax
mov fs, ax
mov eax, 1024*64*3+900
mov ebx, 10000b
.for:
mov edi, filename
int 0x38
mov edi, eax
mov ebp, filename
int 0x32
add eax, 1024*3*12
add ebx, 10000b
cmp byte [es:filename], 0
jnz .for

; Читаем картинки с диска
mov ax, gdt_picture-gdt_0 
mov es, ax
mov ds, ax

mov ebx, 1000000b
mov edi, 0
mov esi, 0
mov eax, 704*3*2
int 0x37

mov ebx, 1010000b
mov edi, 1024*704*3
mov esi, 0
mov eax, 704*3*2
int 0x37

mov ebx, 1100000b
mov edi, 1024*704*3*2
mov esi, 0
mov eax, 704*3*2
int 0x37

mov ebx, 1110000b
mov edi, 1024*704*3*3
mov esi, 0
mov eax, 704*3*2
int 0x37

mov ebx, 10000000b
mov edi, 1024*704*3*4
mov esi, 0
mov eax, 704*3*2
int 0x37

forhlt1:
hlt
jmp forhlt1

for6:

mov ax, gdt_evideo-gdt_0 ; Селектор видеобуфера
mov es, ax
mov ax, gdt_picture-gdt_0 ; Селектор памяти с картинкой
mov ds, ax
mov edi, 1024*704*3
mov eax, 8
for2:
sub edi, eax
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:edi], mm0 ; данные сразу по 8 байт!
jnz for2

mov ax, gdt_evideo-gdt_0 ; Селектор видеобуфера
mov es, ax
mov ax, gdt_picture-gdt_0 ; Селектор памяти с картинкой
mov ds, ax
mov edi, 1024*704*3*2
mov esi, 1024*704*3
mov eax, 8
for3:
sub edi, 8
sub esi, 8
movq mm0, [ds:edi] ; MMX - читаем и пишем 
movq [es:esi], mm0 ; данные сразу по 8 байт!
jnz for3

jmp for6

forhlt:
hlt
jmp forhlt

%include "data.inc"

%include "irqs.inc"

%include "kernel32.inc"

%include "mouse.inc"

%include "idt.inc"

%include "executes.inc"

%include "gdt.inc"

%include "chars.inc"

%include "apps.inc"

%include "cursor.inc"
