;===============================================================================
;                          База функций Gelios
;===============================================================================

;===============================================================================
;                          Чтение данных из файла
;             Для работы требуется функция чтения данных с диска
;-------------------------------------------------------------------------------
; На входе: Последовательно записываются в стек
;             файловый селектор (dword)
;             количество секторов (dword)
;             начальная позиция в файле (dword)
;           es:edi - буфер данных
; На выходе: в случае ошибки eax = 0
;-------------------------------------------------------------------------------
ReadFile:
  pushad
  mov ebp, esp
  add ebp, 32
  cmp dword [ss:esp+48], 0
  mov bx, [ss:ebp]
  and ebx, 11b                ; в ebx - DPL
  mov eax, [ss:ebp+16]        ; читаем из стека файловый селектор
  mov ecx, eax
  and ecx, 11b                ; в ecx - RPL от клиента
  cmp ecx, ebx
  jb near .errorend           ; если RPL<DPL - ошибка
  mov ebx, eax
  shr ebx, 24
  push bx                     ; помещаем в стек идентификатор тома
  push word 1                 ; помещаем в стек кол-во секторов
  and eax, 0x00FFFFFF
  mov edx, eax
  and edx, 11b                ; в edx - RPL
  shr eax, 2
  mov ecx, eax
  shr eax, 2  
  push eax                    ; помещаем в стек смещение адреса
  shr ebx, 4
  int 0x81                    ; читаем таблицу
  cmp eax, 0
  jz .errorend
  add esp, 10
  and ecx, 11b                ; смещение дескриптора в секторе
  shl ecx, 7
  mov esi, edi
  add esi, ecx                ; в esi - указатель на дескриптор нужного файла
  mov eax, [es:esi+84]        ; адрес файла
  mov ebx, [es:esi+80]        ; размер файла
  mov ecx, [ss:ebp+12]        ; смещение
  mov edx, [ss:ebp+16]        ; кол-во секторов
  add eax, ecx                ; конечный адрес
  add ecx, edx                ; конечное смещение для проверки
  cmp ecx, ebx
  ja .errorend
  mov ecx, [ss:ebp+20]
  shr ecx, 24
  push cx
  push edx
  push eax
  shr ecx, 4
  mov ebx, ecx
  int 0x81                    ; читаем таблицу
  cmp eax, 0
  jz .errorend
  popad
  iretd
  
.errorend:
  popad
  xor eax, eax
  iretd
;===============================================================================
  
;===============================================================================
;                         Чтение данных с диска IDE
;-------------------------------------------------------------------------------
; На входе: Последовательно записываются в стек
;             идентификатор тома (byte in word)
;             количество секторов (dword)
;             смещение адреса (dword)
;           es:edi - буфер для данных
; На выходе: в случае ошибки eax = 0
;-------------------------------------------------------------------------------
ReadIDE:
  pushad
  mov ebp, esp
  add ebp, 44
  movzx ebx, word [ss:ebp+8]  ; читаем идентификатор тома
  and ebx, 1111b
  shl ebx, 5
  mov dx, [fs:DiskTable.Class0-GlobalValues+ebx+24]   ; Читаем базовый порт
  mov bx, dx
  cld
  add dx, 0x206
  mov al, 00000010b
  out dx, al                  ; Запрещаем прерывания
  mov esi, [ss:ebp]           ; Читаем из стека смещение
  mov ebp, [ss:ebp+4]         ; Читаем из стека число секторов
  mov eax, ebp
  add eax, esi
  cmp eax, [fs:DiskTable.Class0-GlobalValues+ebx+16] ; Сравниваем с размером
  ja near .errorend           ; Если больше - ошибка
  and esi, 0x0FFFFFFF
  add esi, [fs:DiskTable.Class0-GlobalValues+ebx+20]  ; Смещение + база
  
.for:

  mov ecx, ebp
  and ecx, 0xFF

  mov dx, bx
  call bsywait                ; Ждем готовности канала
  jz near .errorend           ; Если ZF=1 - ошибка
  add dx, 6
  mov eax, esi
  shr eax, 24
  out dx, al                  ; Пишем старший байт адреса
  mov dx, bx
  call freewait               ; Ждем освобождения устройства
  jz near .errorend           ; Если ZF=1 - ошибка
  add dx, 3
  mov eax, esi
  out dx, al                  ; Пишем младший байт адреса
  shr eax, 8
  inc dx
  out dx, al                  ; Пишем первый байт адреса
  shr eax, 8
  inc dx
  out dx, al                  ; Пишем второй байт адреса
  mov al, cl
  mov dx, bx
  add dx, 2
  out dx, al                  ; Пишем кол-во секторов
  add dx, 5
  mov al, 0x20
  out dx, al                  ; Пишем команду
  mov dx, bx
  call bsywait                ; Ждем готовности канала
  jz .errorend                ; Если ZF=1 - ошибка
  call drqwait                ; Ждем готовности обмена
  jz .errorend                ; Если ZF=1 - ошибка
  and ecx, 0xFF
  shl ecx, 8
  cmp ecx, 0
  jnz .next2
  mov ecx, 0x10000
.next2:
  mov eax, ecx
  shr eax, 8
  rep insw

  add esi, eax
  sub ebp, eax

  cmp ebp, 0
  jnz near .for

.end:
  mov dx, bx
  add dx, 0x206
  mov al, 0
  out dx, al
  popad
  xor eax, eax
  not eax
  iretd

.errorend:
  mov dx, bx
  add dx, 0x206
  mov al, 0
  out dx, al
  popad
  xor eax, eax
  iretd

bsywait:
  pushad
  add dx, 7
  mov ecx, 0x00FFFFF
  .wait:
  in al, dx
  test al, 10000000b
  loopnz .wait
  test ecx, ecx
  popad
  ret

freewait:
  pushad
  add dx, 7
  mov ecx, 0x00FFFFF
  .wait:
  in al, dx
  and al, 11000000b
  cmp al, 01000000b
  loopnz .wait
  test ecx, ecx
  popad
  ret

drqwait:
  pushad
  add dx, 7
  mov ecx, 0x00FFFFF
  .wait:
  in al, dx
  test al, 00001000b
  loopz .wait
  test ecx, ecx
  popad
  ret
;===============================================================================

;===============================================================================
;                          Работа с устройствами PCI
;               Ищет устройство с заданными VendorID и DeviceID
;-------------------------------------------------------------------------------
; На входе: Последовательно записываются в стек
;             DeviceID (word)
;             VendorID (word)
; На выходе: eax - координаты устройства PCI, иначе 0 - ошибка
;-------------------------------------------------------------------------------
FindPCI_ID:
  push ebx
  push ecx
  push edx
  push ebp
  mov ebp, esp
  add ebp, 28
  mov ebx, 0x80000008
  mov cx, [ss:ebp]
  shl ecx, 16
  mov cx, [ss:ebp+4]
.for:
  mov dx, 0x0CF8
  mov eax, ebx
  out dx, eax
  mov dx, 0xCFC
  in eax, dx
  cmp eax, ecx
  jz .end
  add ebx, 0x100
  cmp ebx, 0x81000008
  jnz .for
.errorend:
  xor eax, eax
.end:
  pop ebp
  pop edx
  pop ecx
  pop ebx
  iretd
;===============================================================================

;===============================================================================
;                          Работа с устройствами PCI
;               Ищет устройство с заданным кодом класса
;-------------------------------------------------------------------------------
; На входе: Последовательно записываются в стек
;             Код класса (dword, младший байт не учитывается)
; На выходе: eax - координаты устройства PCI, иначе 0 - ошибка
;-------------------------------------------------------------------------------
FindPCI_Class:
  push ebx
  push ecx
  push edx
  mov ebx, 0x80000004
  mov ecx, [ss:esp+24]
  and ecx, 0xFFFFFF00
.for:
  mov dx, 0x0CF8
  mov eax, ebx
  out dx, eax
  mov dx, 0xCFC
  in eax, dx
  and eax, 0xFFFFFF00
  cmp eax, ecx
  jz .end
  add ebx, 0x100
  cmp ebx, 0x81000004
  jnz .for
.errorend:
  xor eax, eax
.end:
  pop edx
  pop ecx
  pop ebx
  iretd
;===============================================================================

;===============================================================================
;                    Функция преобразует число в строку  
;-------------------------------------------------------------------------------
; На входе: eax - число, ebx - основание системы счисления  
;           es:edi - указатель на строку-результат
; На выходе: конечный нулевой байт не пишется  
;-------------------------------------------------------------------------------
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
  ret

HexDigit:
  cmp dl, 10
  jb .less
  add dl, 'A'-10
  ret
.less:
  add dl, '0'
  ret
;===============================================================================

;===============================================================================
;                          Вывод символа на экран
;-------------------------------------------------------------------------------
; На входе последовательно записываются в стек:
;             ASCII-номер символа  (byte in word)
;             смещение записи в сегмент gs (dword)
; На выходе: 
;-------------------------------------------------------------------------------
PutChar:
  pushad
  mov ebp, esp
  add ebp, 36
  movzx eax, word [ss:ebp+4]
  mov esi, dword [ss:ebp]
  shl eax, 7
  mov edi, eax
  add edi, .font
  mov edx, 16
  .fory:
  mov ecx, 8
  .forx:
  mov bl, [cs:edi]
  not bl
  mov al, bl
  mov ah, 0
  shl eax, 16
  mov al, bl
  mov ah, bl
  mov dword [gs:esi], eax
  add esi, 4
  inc edi
  dec ecx
  jnz .forx
  sub esi, 32
  add esi, dword [cs:VideoResW]
  add esi, dword [cs:VideoResW]
  add esi, dword [cs:VideoResW]
  add esi, dword [cs:VideoResW]
  dec edx
  jnz .fory
  popad
  ret
  
.font:
incbin "font.bin"
;===============================================================================

;===============================================================================
;                         Вывод строки на экран
;-------------------------------------------------------------------------------
; На входе последовательно записываются в стек:
;             es:esi - адрес ASCII-Z строки
;             gs:edi - адрес записи в видеобуфер
; На выходе: 
;-------------------------------------------------------------------------------
PutString:
  pushad
  .for:
  movzx ax, byte [es:esi]
  push ax
  push edi
  call PutChar
  add esp, 6
  add edi, 8*4
  inc esi
  cmp byte [es:esi], 0
  jnz .for
  popad
  ret
;===============================================================================

;===============================================================================
;                      Выполнить код в (не)реальном режиме
;-------------------------------------------------------------------------------
; На входе последовательно записываются в стек:  
;        WORD селектор сегмента с кодом (сегмент данных или кода (не R/O)),
;       DWORD смещение кода в выбранном сегменте,
;        WORD размер выполняемого кода в байтах
; Результат:
;    Система будет переведена в реальный режим.
;    Контроллер прерываний будет установлен на стандартные номера
;    IDTR будет установлен на 0x0000
;    Сегментные регистры будут инициализированы следующим образом:
;    CS - 0x4000
;    SS - 0x2000
;    DS, ES, FS - 0x3000
;    GS - сегмент всего адресного пространства (UNREAL MODE)
;    SP = 0xFFFE, остальные - как при вызове процедуры.
;    Код и данные будут переписаны в соответствующие сегменты
;    Управление будет передано по адресу 0x1000:0
;    Программа будет дополнена кодом перехода в защищенный режим
; Выход:             
;    В регистрах eax, ebx, ecx, edx, esi, edi, ebp будет то, что останется
;    после выполнения программы.
;-------------------------------------------------------------------------------
RealModeExecution:
  pushad
  movzx ecx, word [ss:esp+PUSHAD_SIZE+4]
  mov edi, [ss:esp+PUSHAD_SIZE+2+4]
  mov ax, [ss:esp+PUSHAD_SIZE+6+4]
  mov es, ax
  mov ax, gdt_DataCodeVM-gdt_0
  mov ds, ax
  add ecx, 3
  shr ecx, 2  
  mov esi, 0
  .for:       ; переписываем код
    mov eax, dword [es:edi]
    mov dword [esi], eax
    add esi, 4
    add edi, 4
  loop .for
  
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
  mov ax, gdt_Video-gdt_0;gdt_WriteAll-gdt_0
  mov gs, ax
  
  mov eax,0
  mov cr3, eax
  
  lidt [cs:l_IDTLimit] 
  
  mov  eax, cr0
  and  al, 0FEh
  mov  cr0, eax
  call far  0x1000:Come_To_Real-StartLoader ; ДАЛЬНИЙ ПЕРЕХОД
  ret
;===============================================================================