;===============================================================================
;                         Загрузчик ОС Gelios
;-------------------------------------------------------------------------------
;  Загружает вторичный загрузчик, выполняющий все остальные функции
;   по инициализации  ядра, по физическому адресу 0x10000
;===============================================================================

  BITS 16
  
org 0
  jmp 0x07C0:init
init:
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov byte [cs:DeviceID], dl  ; сохраняем начальный номер устройства
  mov ah, 0x08                ; номер функции, узнаем геометрию диска
  ;mov dl, 0x80               ; DeviceID, остается при загрузке
  int 0x13                    ; вызываем функцию int13
  jc error                    ; переход, если ошибка
  mov al, cl
  and eax, 00111111b
  mov dword [cs:Sector], eax  ; помещаем номер сектора + 0
  mov al, cl
  and eax, 11000000b
  shl ax, 2
  mov al, ch
  inc eax
  mov dword [cs:Cylinder], eax ; помещаем номер цилиндра + 1
  movzx eax, dh
  inc ax
  mov dword [cs:Head], eax    ; помещаем номер головки + 1
  mov esi, 1;8197
  mov ax, 0x1000
  mov es, ax
  mov bx, 0
  call LBAtoCHS               ; преобразуем в CHS
  mov ah, 0x02                ; функция чтения данных
  mov al, 64                  ; читаем 64 сектора
  mov dl, [cs:DeviceID]       ; номер устройства
  int 0x13
  jc error                    ; переход, если ошибка
  add bx, 64*512
  add esi, 64
  call LBAtoCHS               ; преобразуем в CHS
  mov ah, 0x02                ; функция чтения данных
  mov al, 64                  ; читаем 64 сектора
  mov dl, [cs:DeviceID]       ; номер устройства
  int 0x13
  jc error                    ; переход, если ошибка
  mov dl, [cs:DeviceID]
  jmp 0x1000:0                ; переход на вторичный загрузчик 
  
%macro HexToChar 1
  add %1, '0'
  cmp %1, 10+'0'
  jl .low%1
  add %1, 'A'-'0'-10
  .low%1:
%endmacro 
  
error:                        ; выводим сообщение об ошибке
  mov bl, ah
  mov bh, ah
  shr bh, 4
  and bl, 1111b
  HexToChar bl
  HexToChar bh
  mov si, string2
  mov ax, cs
  mov ds, ax
  mov byte [si], bh
  inc si
  mov byte [si], bl
  mov si, string              ; адрес строки сообщения
  mov ah, 0x0F
  int 0x10
  mov ah, 0x0E
for:
  mov al, [si]             
  cmp al, 0
  jz endfor
  int 0x10
  inc si
  jmp for
endfor:
  cli                         ; останавливаем систему
  hlt
  jmp $                       ; на всякий случай ;)
  
;===============================================================================
;               Функция преобразует LBA-адрес в CHS-адрес    
;-------------------------------------------------------------------------------
; На входе: esi - линейный адрес сектора с нуля       
; На выходе: координаты в формате int 13h       
;-------------------------------------------------------------------------------
; s - sector, c - cylinder, h - head, m - max
LBAtoCHS:
  mov eax, esi                ; LBA=(c*mh+h)*ms+s
  mov edx, 0
  div dword [cs:Sector]       ; /ms ; eax=c*mh+h ; edx=s-1
  inc edx
  mov cl, dl                  ; номер сектора (s)
  mov edx, 0
  div dword [cs:Head]         ; /mh ; eax=c      ; edx=h
  mov ch, al                  ; номер цилиндра (c)
  mov dh, dl                  ; номер головки (h)
  ret
;===============================================================================
  
  Sector:     dd 0            ; максимальный номер сектора
  Cylinder:   dd 0            ; максимальный номер цилиндра + 1
  Head:       dd 0            ; максимальный номер головки + 1
  DeviceID    db 0            ; начальный номер устройства

  string db 0xA,0xD,'DISK ERROR (INT 13h). CODE ' 
  string2 db '  h', 0 ; сообщение об ошибке
  
  TIMES 510-($-$$) db 0

  db 0x55, 0xAA               ; сигнатура загрузчика