;===============================================================================
;                            Вторичный загрузчик
; Устанавливает видеорежим, измеряет частоту процессора, объем памяти и т.д.,
; затем переводит процессор в защищенный режим, 
; загружает ядро и предает ему управление.
;===============================================================================

StartLoader:

  mov byte [cs:l_DeviceID], dl ; сохраняем начальный номер устройства
  
  mov ax, ss
	mov word [RealSS], ax
	mov word [RealSP], sp


  mov ax, cs
  mov ds, ax
  lgdt [l_gdt_ptr]         ; загружаем GDTR загрузчика


; запрещаем прерывания
  cli
  in   al, 70h
  or   al, 80h
  out  70h, al  ; запрет NMI

; открываем адресную линию A20
  in   al, 92h
  or   al, 2
  out  92h, al
  
  jmp skipbuffer
    buffer TIMES 24 db 0
    btext db '0000000000000' 
    ViewNum:   
    ; eax - NUM, ds:si - video, es=cs 
      mov ebx, 16
      mov edi, btext
      call l_NumToASCII2
      mov ah, 111b
      mov di, btext
      mov cx, 8
      .for:
        mov al, [es:di]
        mov word [si], ax
        inc di
        inc si
        inc si
      loop .for
      retn      
             
  skipbuffer:
    mov ax, cs
    mov es, ax
    mov edx, 0x534D4150
    mov ebx, 0
    mov si, 0
    mov bp, 0
    mov ax, 0xB800
    mov ds, ax
    .for:  
      mov edi, buffer
      mov eax, 0xE820  
      mov ecx, 24
      int 0x15
      push ebx
      mov eax, [es:buffer+4]
      call near ViewNum
      mov eax, [es:buffer]
      call near ViewNum
      mov word [si], ' '+11100000000b
      add si, 2
      mov eax, [es:buffer+12]
      call near ViewNum
      mov eax, [es:buffer+8]
      call near ViewNum
      mov word [si], ' '+11100000000b
      add si, 2
      mov eax, [es:buffer+16]
      call near ViewNum
      add bp, 80*2
      mov si, bp 
      pop ebx
    cmp ebx, 0
    jnz .for
    ;jmp $
  mov ax, 0x2401
  int 0x15
  jnc noterror
    jmp itcode
    errortext: db '          '
  itcode:
    movzx eax, ah
    mov ebx, 16
    mov ax, cs
    mov es, ax
    mov edi, errortext
    call l_NumToASCII
    mov ecx, 8
    mov di, 0
    mov ax, 0xB800
    mov es, ax   
    mov ah, 111b
    mov si, errortext
    .for:
      mov al, [cs:si]
      mov word [es:di], ax
      inc di
      inc di
      inc si
    loop .for
    cli
    hlt
  noterror:
  
  mov eax, cr0                ; переходим в защищенный режим
  or eax, 1
  mov cr0, eax
  
  jmp l_gdt_Code-l_gdt_0:.next
  .next:
  
  mov ax, l_gdt_WriteAll-l_gdt_0
  mov fs, ax                  ; загружаем селектор дескриптора WriteAll
  
  mov eax, cr0                ; возвращаемся в реальный режим
  and eax, -2                 ; теперь уже в режим линейной адресации
  mov cr0, eax
  
  jmp 0x1000:.next2
  .next2:

 
  ; разрешаем аппаратные прерывания и NMI
  in   al, 70h
  and  al, 7Fh
  out  70h, al
  sti
  
; Расчет параметров загрузочного диска
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ah, 0x08                ; номер функции
  mov dl, [cs:l_DeviceID]     ; DeviceID
  int 0x13                    ; вызываем функцию int13
  jc near l_error             ; переход, если ошибка
  mov al, cl
  and eax, 00111111b
  mov dword [cs:l_Sector], eax ; помещаем номер сектора + 0
  mov al, cl
  and eax, 11000000b
  shl ax, 2
  mov al, ch
  inc eax
  mov dword [cs:l_Cylinder], eax ; помещаем номер цилиндра + 1
  movzx eax, dh
  inc ax
  mov dword [cs:l_Head], eax  ; помещаем номер головки + 1
  
  mov ah, 0x0E
  mov si, l_starttext
.fortw:  
  mov al, [cs:si]
  int 0x10
  inc si
  cmp al,0
jnz .fortw

; Читаем ядро (512 КБ) с диска
  mov esi, 1;8197             ; начальный адрес сектора
  mov ax, BufferPtr           ; сегмент буфера BufferPtr
  mov es, ax
  mov ebp, KernelPtr          ; адрес загрузки KernelPtr
.forread:
  mov bx, 0                   ; сбрасываем на начало буфера
  call l_LBAtoCHS             ; преобразуем в CHS
  mov ah, 0x02                ; функция чтения данных
  mov al, 64                  ; читаем 64 сектора
  mov dl, [cs:l_DeviceID]     ; номер устройства
  int 0x13
  jc near l_error             ; переход, если ошибка
  mov bx, 0                   
.forwrite:                    
  mov eax, [es:bx]            ; переносим данные из буфера в KernelPtr
  mov dword [fs:ebp], eax
  add ebp, 4                  ; увеличиваем указатель записи
  add bx, 4                   ; увеличиваем указатель чтения
  cmp bx, 512*64
  jnz .forwrite               ; повторяем для 32 КБ
  mov ah, 0x0E
  mov al, '.'
  int 0x10
  add esi, 64                 ; увеличиваем номер сектора на 64 (32 КБ)
  cmp esi, 1+1024
  jnz .forread                ; повторяем для 1024 секторов (512 КБ)
  
; Подсчет частоты процессора
  mov al,0FEh                 ; маскируем все прерывания кроме таймера
  out 21h,al
  hlt                         ; останавливаем процессор до прерывания таймера
  rdtsc                       ; читаем счетчик тактов
  mov ebx,eax
  hlt                         ; останавливаем процессор до прерывания таймера
  rdtsc                       ; читаем счетчик тактов
  sub eax,ebx                 ; находим число тактов за один тик таймера
  mov edx, 0
  mov ebx, 1000/18
  div ebx                     ; переводим в килогерцы
  mov ebp, Base+CPUFreq
  mov word [fs:ebp], ax       ; помещаем частоту CPU в GlobaValues
  mov al,0
  out 21h,al                  ; размаскировываем все прерывания

; Выбор и установка видеорежима
; Отображение таблицы и списка режимов
videomodes:
  mov byte [cs:l_endstatus], 0
  mov ax, cs
  mov es, ax
  mov ax, 0x4F00              ; функция чтения информации о видеодаптере
  mov di, l_videoinfo
  int 0x10
  cmp ax, 0x014F
  jz near l_error             ; проверяем статус завершения
  mov bp, [cs:l_videoinfo+0xE] ; смещение списка режимов
  mov ax, [cs:l_videoinfo+0xE+2] ; сегмент списка режимов
  mov ds, ax
  mov esi, l_vmodes
for:
  mov ax, cs
  mov es, ax
  mov cx, [ds:bp]
  cmp cx, 0xFFFF              ; проверяем конец списка
  jz near .next
  mov ax, 0x4F01              ; функция чтения информации о видеорежиме
  mov di, l_vmodeinfo
  int 0x10
  cmp ax, 0x014F
  jz near l_error             ; проверяем статус завершения
  mov byte [cs:esi], 186    
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  movzx eax, cx
  mov ebx, 16                 ; шестнадцатиричное число
  mov edi, esi
  call l_NumToASCII
  add esi, 4
  mov byte [cs:esi], 'h'
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], 186
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  movzx eax, word [cs:l_vmodeinfo+0x12] ; разрешение по горизонтали
  mov ebx, 10                 ; десятичное число
  mov edi, esi
  call l_NumToASCII
  add esi, 4
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], 186
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  movzx eax, word [cs:l_vmodeinfo+0x14] ; разрешение по вертикали
  mov ebx, 10                 ; десятичное число
  mov edi, esi
  call l_NumToASCII
  add esi, 4
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], 186
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  movzx eax, byte [cs:l_vmodeinfo+0x19] ; количество бит на пиксел
  mov ebx, 10                 ; десятичное число
  mov edi, esi
  call l_NumToASCII
  add esi, 2
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], ' '
  inc esi
  mov byte [cs:esi], 186
  inc esi
  inc esi
  add bp, 2
  jmp for
.next:
    
  mov ax, 0xB800              ; адрес видеобуфера
  mov es, ax
  mov di, 80*50
.forclear:                    ; очищаем видеобуфер
  dec di
  dec di
  mov word [es:di], 0
  jnz .forclear
  
%define soff 86


  
  
  mov si, l_stringup1         ; пишем первые три строки таблицы (заголовок)
  mov di, soff
.for1:
  mov al, [cs:si]
  mov byte [es:di], al 
  inc di 
  mov byte [es:di], 00000111b
  inc di
  inc si
  cmp al, 0
  jnz .for1
  add di, (80-37)*2
  cmp di, 3*80*2+soff
  jnz .for1
  
  mov si, l_vmodes            ; адрес списка видеорежимов
.for:
  mov al, [cs:si]
  mov byte [es:di], al 
  inc di 
  mov byte [es:di], 00000111b
  inc di
  inc si
  cmp al, 0
  jnz .for
  add di, (80-37)*2
  cmp di, 48*80+soff
  jnz .for
  
  mov si, l_stringdown        ; адрес конечной строки таблицы
.for2:
  mov al, [cs:si]
  mov byte [es:di], al 
  inc di 
  mov byte [es:di], 00000111b
  inc di
  inc si
  cmp al, 0
  jnz .for2
  add di, (80-37)*2
  cmp di, 50*80+soff
  jnz .for2
  
  mov si, [cs:l_videoptr]
  mov byte [es:si], '>'
  mov byte [es:si+1], 00001111b
  
  mov si, l_videotext
  mov di, 0
  mov al, [cs:si]
  inc si
.fortw1:  
  cmp al, 0xD
  jnz .nextd
  mov ax,di
  mov dx,0
  mov cx, 160
  div cx
  sub di,dx
  mov al, [cs:si]
  inc si
  jmp .fortw1
  .nextd:
  cmp al,0xA
  jnz .nexta
  add di,160
  mov al, [cs:si]
  inc si
  jmp .fortw1
  .nexta:
  mov byte [es:di], al
  inc di
  mov byte [es:di], 00000111b
  inc di
  mov al, [cs:si]
  inc si

  cmp al,0
jnz .fortw1

  mov ah, 0x1
  mov ch, 0x20
  int 0x10
  
  cli
  mov ax, 0
  mov es, ax
  mov ax, l_key
  xchg word [es:9*4], ax
  push ax
  mov ax, cs
  xchg word [es:9*4+2], ax
  push ax
  sti
  
.idle:                        ; ждем получения номера
  hlt
  cmp byte [cs:l_endstatus], 0xFF
  jnz .idle
  
  cli
  
  mov ax, 0
  mov es, ax  
  pop ax
  mov word [es:9*4+2], ax
  pop ax
  mov word [es:9*4], ax
  
  mov si, [cs:l_videoinfo+0xE] ; смещение списка режимов
  mov ax, [cs:l_videoinfo+0xE+2] ; сегмент списка режимов
  mov ds, ax
  mov ax, [cs:l_vmodesptr]    ; узнаем номер первого режима
  shl ax, 1
  add si, ax
  mov ax, [cs:l_tableptr]     ; узнаем номер первого режима
  shl ax, 1
  add si, ax
  
  mov ax, 0x4F02
  mov bx, [ds:si]             ; устанавливаем выбранный видеорежим
  or bx, 0100000000000000b    ; 32-битная адресация
  mov ebp, Base+VideoMode.Number
  mov word [fs:ebp], bx
  int 0x10
  cmp ax, 0x014F
  jz near l_error
    
  mov ax, cs
  mov es, ax
  mov ax, 0x4F01
  mov cx,  [ds:si]
  mov di, l_vmodeinfo
  int 0x10                    ; получаем информацию о видеорежиме
  
  mov eax, 0
  mov ax, [cs:l_vmodeinfo+0x12] 
  mov ebp, Base+VideoMode.Width    
  mov dword [fs:ebp], eax
  
  mov eax, 0
  mov ax, [cs:l_vmodeinfo+0x14]
  mov ebp, Base+VideoMode.Height
  mov dword [fs:ebp], eax
  
  mov eax, 0
  mov al, [cs:l_vmodeinfo+0x19]
  shr al, 3
  mov ebp, Base+VideoMode.PixelBytes
  mov dword [fs:ebp], eax
  
  mov eax,0
  mov al, [cs:l_vmodeinfo+0x1F]
  mov ebp, Base+VideoMode.RedBits
  mov dword [fs:ebp], eax
  
  
  mov eax, 0
  mov al, [cs:l_vmodeinfo+0x21]
  mov ebp, Base+VideoMode.GreenBits
  mov dword [fs:ebp], eax
  
  mov eax, 0
  mov al, [cs:l_vmodeinfo+0x23]
  mov ebp, Base+VideoMode.BlueBits
  mov dword [fs:ebp], eax
  
  mov eax, 0
  mov al, [cs:l_vmodeinfo+0x25]
  mov ebp, Base+VideoMode.ReserveBits
  mov dword [fs:ebp], eax
  
  mov ebp, GDTPtr-gdt_0+gdt_Video2     ; инициализация дескриптора видеоданных
  mov eax, [cs:l_vmodeinfo+0x28]
  mov word [fs:ebp+2], ax
  shr eax, 16
  mov byte [fs:ebp+4], al
  mov byte [fs:ebp+7], ah

  mov ax, cs
  mov ds, ax
  lidt [IDTLimit]               ; загружаем IDTR

  lgdt [GDTLimit]               ; загружаем GDTR
  
  ;mov eax, PDTPtr
  ;mov cr3, eax                ; Загружем в cr3 адрес PDT
  
; Разрешаем использование больших страниц памяти
  ;mov eax, cr4 
  ;or eax, 10000b
  ;mov cr4, eax

; Маскирование прерываний
; запрещаем прерывания
  cli
  in   al, 70h
  or   al, 80h
  out  70h, al  ; запрет NMI
  
; Устанавливаем частоту таймера
	;mov al, 0x34            
	;out 0x43, al
	;mov al, (1193180/timerfreq)&0xFF
	;out 0x40, al
	;mov al, ((1193180/timerfreq)>>8)               
	;out 0x40, al
	

; Устанавливаем для IRQ0-IRQ7 номера прерываний 0x20-0x27
  mov dx, 0x20
  mov ah, 0x20
  call l_ProgInt

; Устанавливаем для IRQ8-IRQ15 номера прерываний 0x28-0x2F
  mov dx, 0xA0
  mov ah, 0x28
  call l_ProgInt
  
  mov al,11111101b
  out 21h,al                  ; разрешаем все прерывания

  mov eax, cr0                ; устанавливаем бит защищенного режима
  or eax, 1                   ;             и бит сраничной адресации
  mov cr0, eax
  
  ;db 0x66
  jmp gdt_Code-gdt_0:dword StartKernelCode
  
  
l_starttext: db 'Starting Gelios. Reading data from the disk', 0
l_videotext: db 0xD,0xA,0xA,0xA,0xA,0xA,0xA,0xA,0xA,' Operating system project';' GELIOS Operating System'
             db 0xD, 0xA, 0xA,' Compiled on NASM',
             db 0xD,0xA,' ',__DATE__,' ',__TIME__,0xD,0xA,
             db 0xD, 0xA, 0xA,' Author: Karnakov Petr',0xD,0xA,' e-mail: petakarn1@gmail.com',0

l_CodePtr:
  dd StartKernelCode
  dw gdt_Code-gdt_0
  
  GDTLimit:dw gdt_end-gdt_0-1 ; Предел GDT
  GDTAddress:  dd GDTPtr      ; Адрес GDT

  IDTLimit:dw idt_end-idt_0-1 ; Предел IDT  
  IDTAddress:  dd IDTPtr      ; Адрес IDT
  
  %include "GDT.inc"

;===============================================================================
;                             Обработчик ошибок 
;             Выводит сообщение об ошибке и останавливает систему
;-------------------------------------------------------------------------------
l_error:                      ; если ошибка - переходим сюда
  mov ax, 0xB800              ; адрес видеобуфера
  mov es, ax
  mov si, l_string            ; адрес строки сообщения
  mov di, 24*80*2             ; инициализируем указатель (последняя строка)
.for:
  mov al, [cs:si]
  mov byte [es:di], al        ; выводим сообщение об ошибке
  inc di 
  mov byte [es:di], 00001100b
  inc di
  inc si
  cmp al, 0
  jnz .for
  cli                         ; останавливаем систему
  hlt
  
l_string db 'Loading process error.', 0
;===============================================================================

;===============================================================================
;                    Функция преобразует число в строку  
;-------------------------------------------------------------------------------
; На входе: eax - число, ebx - основание системы счисления  
;           es:edi - указатель на строку-результат
; На выходе: конечный нулевой байт не пишется  
;-------------------------------------------------------------------------------
l_NumToASCII:
  pushad
  xor esi, esi
l_convert_loop:
  xor edx, edx
  div ebx
  call l_HexDigit
  push edx
  inc esi
  test eax, eax
  jnz l_convert_loop
  cld
l_write_loop:
  pop eax
  stosb
  dec esi
  test esi, esi
  jnz l_write_loop
  popad
  ret

l_HexDigit:
  cmp dl, 10
  jb .less
  add dl, 'A'-10
  ret
.less:
  add dl, '0'
  ret
;===============================================================================

;===============================================================================
;                    Функция преобразует число в строку
;                    8    d i g i t s  
;-------------------------------------------------------------------------------
; На входе: eax - число, ebx - основание системы счисления  
;           es:edi - указатель на строку-результат
; На выходе: конечный нулевой байт не пишется  
;-------------------------------------------------------------------------------
l_NumToASCII2:
  pushad
  xor esi, esi
  mov ecx, 8
l_convert_loop2:
  xor edx, edx
  div ebx
  call l_HexDigit2
  push edx
  inc esi
  test eax, eax
  loop l_convert_loop2
  cld
l_write_loop2:
  pop eax
  stosb
  dec esi
  test esi, esi
  jnz l_write_loop2
  popad
  ret

l_HexDigit2:
  cmp dl, 10
  jb .less
  add dl, 'A'-10
  ret
.less:
  add dl, '0'
  ret
;===============================================================================
  
;===============================================================================
;                       Обработчик прерывания клавиатуры  
;                         Реализует выбор видеорежима
;-------------------------------------------------------------------------------
l_key: 
  pushad
  push es
  in al, 0x60
  cmp al, 0x50                ; кнопка "вниз"
  jnz near .downnext
  mov ax, [cs:l_vmodesptr]
  add ax, [cs:l_tableptr]
  inc ax
  mov dx, 37
  mul dx
  add ax, l_vmodes+7
  mov si, ax
  cmp byte [cs:si], 'h'
  jnz near .end
  cmp word [cs:l_tableptr] , 20 ; если номер не вышел за пределы, то
  jnz .downovernext             ; выходим
  mov ax, [cs:l_vmodesptr]      
  inc ax
  cmp ax, 80
  mov bx, 79
  cmovz ax, bx
  mov word [cs:l_vmodesptr], ax
  mov ax, [cs:l_vmodesptr]
  mov dx, 37
  mul dx
  mov si, l_vmodes            ; адрес списка видеорежимов
  add si, ax
  mov di, 3*80*2+soff   
  mov ax, 0xB800
  mov es, ax
.fordown:
  mov al, [cs:si]
  mov byte [es:di], al 
  inc di 
  mov byte [es:di], 00000111b
  inc di
  inc si
  cmp al, 0
  jnz .fordown
  add di, (80-37)*2
  cmp di, 48*80+soff
  jnz .fordown
  mov si, [cs:l_videoptr]
  mov byte [es:si], '>'
  mov byte [es:si+1], 00001111b
  mov word [cs:l_videoptr], si
  jmp .end
.downovernext:
  inc word [cs:l_tableptr]    ; увеличиваем указатель элемента
  mov ax, 0xB800
  mov es, ax
  mov si, [cs:l_videoptr]
  mov byte [es:si], ' '
  mov byte [es:si+1], 00001111b
  add si, 160
  mov byte [es:si], '>'
  mov byte [es:si+1], 00001111b
  mov word [cs:l_videoptr], si
  jmp .end
.downnext:
  cmp al, 0x48                ; кнопка "вверх"
  jnz near .upnext
  cmp word [cs:l_tableptr] , 0
  jnz .upovernext
  mov ax, [cs:l_vmodesptr]
  dec ax
  cmp ax, -1
  mov bx, 0
  cmovz ax, bx
  mov word [cs:l_vmodesptr], ax
  mov ax, [cs:l_vmodesptr]
  mov dx, 37
  mul dx
  mov si, l_vmodes            ; адрес списка видеорежимов
  add si, ax
  mov di, 3*80*2+soff   
  mov ax, 0xB800
  mov es, ax
.forup:
  mov al, [cs:si]
  mov byte [es:di], al 
  inc di 
  mov byte [es:di], 00000111b
  inc di
  inc si
  cmp al, 0
  jnz .forup
  add di, (80-37)*2
  cmp di, 48*80+soff
  jnz .forup
  mov si, [cs:l_videoptr]
  mov byte [es:si], '>'
  mov byte [es:si+1], 00001111b
  mov word [cs:l_videoptr], si
  jmp .end
.upovernext:
  dec word [cs:l_tableptr]    ; увеличиваем указатель элемента
  mov ax, 0xB800
  mov es, ax
  mov si, [cs:l_videoptr]
  mov byte [es:si], ' '
  mov byte [es:si+1], 00001111b
  sub si, 160
  mov byte [es:si], '>'
  mov byte [es:si+1], 00001111b
  mov word [cs:l_videoptr], si
  jmp .end
.upnext:
  cmp al, 0x1C                ; кнопка "ввод"
  jnz .enternext
  mov byte [cs:l_endstatus], 0xFF
.enternext:
.end:
  mov al, 0x20
  out 0x20, al
  pop es
  popad
  iret
;===============================================================================
  
;===============================================================================
;               Функция преобразует LBA-адрес в CHS-адрес    
;-------------------------------------------------------------------------------
; На входе: esi - линейный адрес сектора с нуля       
; На выходе: координаты в формате int 13h       
;-------------------------------------------------------------------------------
; s - sector, c - cylinder, h - head, m - max
l_LBAtoCHS:
  mov eax, esi                ; LBA=(c*mh+h)*ms+s
  mov edx, 0
  div dword [cs:l_Sector]     ; /ms ; eax=c*mh+h ; edx=s-1
  inc edx
  mov cl, dl                  ; номер сектора (s)
  mov edx, 0
  div dword [cs:l_Head]       ; /mh ; eax=c      ; edx=h
  mov ch, al                  ; номер цилиндра (c)
  mov dh, dl                  ; номер головки (h)
  ret
;===============================================================================

;===============================================================================
;             Процедура перепрограммирует контроллер прерываний
;-------------------------------------------------------------------------------
l_ProgInt:
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx
  ret
;===============================================================================
  
;===============================================================================
;                       Временная таблица GDT загрузчика
;-------------------------------------------------------------------------------
l_gdt_0:                      ; нулевой пустой дескриптор
  dw 0
  dw 0
  db 0
  db 0
  dw 0
l_gdt_WriteAll:               ; дескриптор всего адресного пространства
  dw 0xFFFF
  dw 0
  db 0
  db 10010010b
  dw 0000000010001111b    
l_gdt_Code:                     ; Дескриптор кода VM86
  dw 0xFFFF                         ; предел (0..15)                 
  dw 0x0000                         ; адрес (0..15)                  
  db 0x01                           ; адрес (16..23)                 
  db 10011010b                      ; доступ                         
  db 00000000b                      ; предел (16..23) и прочее       
  db 0                              ; адрес (24..31)  
l_gdt_end:
  
l_gdt_ptr:                    ; указатель на таблицу GDT загрузчика
  dw l_gdt_end-l_gdt_0
  dd l_gdt_0+0x10000
;===============================================================================
  
;===============================================================================
;               Данные, используемые окном выбора видеорежима
;-------------------------------------------------------------------------------
l_videoinfo: TIMES 1024 db 0  ; буфер для информации о видеоадаптере
l_vmodeinfo: TIMES 1024 db 0  ; временный буфер для информации о видеорежиме
l_vmodes:                     ; текстовые строки видеорежимов
%rep 100
  db 186,'         ',186,'        ',186,'        ',186,'      ',186, 0
%endrep
; Строки, формирующие границы таблицы
l_stringup1:                  ; верхняя линия заголовка
  db 201,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205
  db 203,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,187, 0
  db 186,' Number  ',186,' Width  ',186,' Height ',186,' Bits ',186, 0
l_stringup3:                  ; нижняя линия заголовка
  db 204,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205
  db 206,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,185, 0
l_stringdown:                 ; завершение таблицы
  db 200,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205
  db 202,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,188, 0

l_tableptr    dw 0            ; указатель на выбранный элемент таблицы
l_vmodesptr   dw 0            ; указатель на первый отображаемый видеорежим
l_endstatus   db 0            ; статус завершения выбора, 0xFF - готово
l_videoptr    dw 3*160+soff+2     ; указатель на символ
;===============================================================================

l_Sector      dd 0            ; максимальный номер сектора + 0
l_Cylinder    dd 0            ; максимальный номер цилиндра + 1
l_Head        dd 0            ; максимальный номер головки + 1
l_DeviceID    db 0            ; начальный номер устройства

ifkernel:

    ; запрет всех прерываний
    cli
    in   al, 70h
    or   al, 80h
    out  70h, al  ; запрет NMI


  MOV DWORD [GS:0], 0XFFFF0000

  
  mov ax, gdt_DataVM-gdt_0
  mov es, ax
  mov ds, ax
  mov ss, ax
  mov fs,ax
  mov gs, ax
  
  mov eax,0
  mov cr3, eax
  
  lidt [cs:l_IDTLimit] 
  
    mov  eax, cr0
    and  al, 0FEh
    mov  cr0, eax
    jmp  0x1000:after ; ДАЛЬНИЙ ПЕРЕХОД

  ;jmp after
  
after:

  mov ax, [cs:RealSS]
  mov ss, ax
  mov sp, [cs:RealSP]
  mov ax, 0x1000
  mov es, ax
  mov ds, ax  
  mov fs, ax
  mov es, ax
  
        ; закрываем A20
        mov     al,0d1h
        out     64h,al
        mov     al,0ddh
        out     61h,al

  

  
; Устанавливаем для IRQ0-IRQ7 номера прерываний 0x8-0xF
  mov dx, 0x20
  mov ah, 0x8
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx

; Устанавливаем для IRQ8-IRQ15 номера прерываний 0x10-0x17
  mov dx, 0xA0
  mov ah, 8+8
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx
  
  mov al,11111101b
  out 21h,al                  ; разрешаем все прерывания
  mov al,-1
  out 0A1h,al                  ; разрешаем все прерывания

  
    ; разрешаем аппаратные прерывания и NMI
    in   al, 70h
    and  al, 7Fh
    out  70h, al
    sti
     
  mov ah, 0
  mov al, 2
  int 0x10  
  
  MOV DL, [cs:l_DeviceID]
  
  mov byte [cs:l_DeviceID], dl ; сохраняем начальный номер устройства
  
  mov ax, ss
	mov word [RealSS], ax
	mov word [RealSP], sp

  db 0x66
  lgdt [cs:l_gdt_ptr]         ; загружаем GDTR загрузчика


; запрещаем прерывания
  cli
    in   al, 70h
    or   al, 80h
    out  70h, al  ; запрет NMI

; открываем адресную линию A20
  in   al, 92h
  or   al, 2
  out  92h, al
  
  mov eax, cr0                ; переходим в защищенный режим
  or eax, 1
  mov cr0, eax
  
  mov ax, l_gdt_WriteAll-l_gdt_0
  mov fs, ax                  ; загружаем селектор дескриптора WriteAll
  
  mov eax, cr0                ; возвращаемся в реальный режим
  and eax, -2                 ; теперь уже в режим линейной адресации
  mov cr0, eax

 
    ; разрешаем аппаратные прерывания и NMI
    in   al, 70h
    and  al, 7Fh
    out  70h, al
    sti
    
  mov word [cs:l_tableptr], 0
  mov word [cs:l_vmodesptr], 0
  mov byte [cs:l_endstatus], 0
  mov word [cs:l_videoptr], 3*160+soff+2
    
  jmp near videomodes
  
ifkernel2:

  ; запрет всех прерываний
  cli
  in   al, 70h
  or   al, 80h
  out  70h, al  ; запрет NMI  
  
  mov ax, gdt_DataVM-gdt_0
  mov es, ax
  mov ds, ax
  mov ss, ax
  mov fs,ax
  mov ax, gdt_WriteAll-gdt_0
  mov gs, ax
  
  mov eax,0
  mov cr3, eax
  
  lidt [cs:l_IDTLimit] 
  
  mov  eax, cr0
  and  al, 0FEh
  mov  cr0, eax
  jmp  0x1000:Come_To_Real-StartLoader ; ДАЛЬНИЙ ПЕРЕХОД
  
after2:

  mov ax, [cs:RealSS]
  mov ss, ax
  mov sp, [cs:RealSP]
  mov ax, 0x1000
  mov es, ax
  mov ds, ax  
  mov fs, ax
  mov es, ax
  
        ; закрываем A20
        mov     al,0d1h
        out     64h,al
        mov     al,0ddh
        out     61h,al

  

  
; Устанавливаем для IRQ0-IRQ7 номера прерываний 0x8-0xF
  mov dx, 0x20
  mov ah, 0x8
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx

; Устанавливаем для IRQ8-IRQ15 номера прерываний 0x10-0x17
  mov dx, 0xA0
  mov ah, 8+8
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx
  
  mov al,11111100b
  out 21h,al                  ; разрешаем все прерывания
  mov al,-1
  out 0A1h,al                  ; разрешаем все прерывания

  
    ; разрешаем аппаратные прерывания и NMI
    in   al, 70h
    and  al, 7Fh
    out  70h, al
    sti
     
  mov ah, 0
  mov al, 2
  int 0x10  
  
  mov word [cs:l_tableptr], 0
  mov word [cs:l_vmodesptr], 0
  mov byte [cs:l_endstatus], 0
  mov word [cs:l_videoptr], 3*160+soff+2
    
  
  MOV DL, [cs:l_DeviceID]

  jmp near StartLoader
  
Come_To_Real:
  mov  eax, cr0
  and  al, 0FEh
  mov  cr0, eax
  jmp 0x1000:Come_To_Real.next-StartLoader ; ДАЛЬНИЙ ПЕРЕХОД
  .next:
  ;pop dword [cs:PM_EIP]
  ;pop word [cs:PM_CS]
  ;mov dword [cs:PM_ESP], esp
  ;push word ss
  ;pop word [cs:PM_SS]

  ;popad

  mov ax, 0x2000
  mov ss, ax
  mov esp, 0xFFFE
  mov ax, 0x3000
  mov ds, ax
  mov es, ax
 ; mov fs, ax
  push dx
  ;pushad
  
; закрываем A20
  ;mov     al,0d1h
  ;out     64h,al
  ;mov     al,0ddh
  ;out     61h,al 
    
; Устанавливаем для IRQ0-IRQ7 номера прерываний 0x8-0xF
  mov dx, 0x20
  mov ah, 0x8
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx

; Устанавливаем для IRQ8-IRQ15 номера прерываний 0x10-0x17
  mov dx, 0xA0
  mov ah, 8+8
  mov al,0x11
  out dx,al
  out 0x80, al
  mov al,ah
  inc dx
  out dx,al
  out 0x80, al
  mov al,4
  out dx,al
  out 0x80, al
  mov al,1
  out dx,al
  out 0x80, al
  mov al,0xFF
  out dx,al
  dec dx
  
  mov al,11111101b
  out 21h,al                  ; разрешаем все прерывания
  mov al,-1
  out 0A1h,al                  ; разрешаем все прерывания

  
; разрешаем аппаратные прерывания и NMI
  in   al, 70h
  and  al, 7Fh
  out  70h, al
  sti

  MOV DL, [cs:l_DeviceID]
  pop dx
  mov word [cs:RMPTR], dx
  jmp far [cs:RMPTR]
  
  ;----------------
  RMPTR:
  dw 0
  dw 0x4000
  ;--------------
  
Back_To_PM:

  ;pushad

  mov ebp, Base+IDTLimit
  db 0x66
  lidt [gs:ebp]               ; загружаем IDTR
  
  mov ebp, Base+GDTLimit
  db 0x66
  lgdt [gs:ebp]               ; загружаем GDTR

; Маскирование прерываний
  cli
  
; Устанавливаем частоту таймера
	mov al, 0x34            
	out 0x43, al
	mov al, (1193180/timerfreq)&0xFF
	out 0x40, al
	mov al, ((1193180/timerfreq)>>8)               
	out 0x40, al
	

; Устанавливаем для IRQ0-IRQ7 номера прерываний 0x20-0x27
  mov dx, 0x20
  mov ah, 0x20
  call l_ProgInt

; Устанавливаем для IRQ8-IRQ15 номера прерываний 0x28-0x2F
  mov dx, 0xA0
  mov ah, 0x28
  call l_ProgInt
  
  mov al,11111100b
  out 21h,al                  ; разрешаем все прерывания

  mov eax, cr0                ; устанавливаем бит защищенного режима
  or eax, 1;0x80000001          ;             и бит сраничной адресации
  mov cr0, eax
  
  db 0x66
  ;jmp far [cs:l_CodePtr]      ; переход в ядро
  jmp far [cs:PM_EIP]      ; переход в ядро
  
  

  jmpptr:
  dw after
  dw 0x1000

  l_IDTLimit:dw 3FFh
                 dd 0

  RealSS: dw 0
  RealSP: dw 0
  PM_SS: dw 0
  PM_ESP: dd 0
  PM_EIP: dd Rnext
  PM_CS: dw gdt_Code-gdt_0


  PM_IDTR: dw idt_end-idt_0-1
           dd IDTPtr
           

 
                        
TIMES 64*1024-($-$$) db 0     ; занимаем 64 КБ под загрузчик