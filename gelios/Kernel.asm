;===============================================================================
; CP1251                 Операционная система Gelios
;                                   Ядро
;===============================================================================
%include "Const.inc"
BITS 16
org 0
StartGelios:
%include "KernelLoader.inc"

BITS 32
StartKernel:                  ; ядро грузится по адресу KernelPtr
                     


%include "IDT.inc"  
%include "GlobalValues.inc"

Stackt:
  TIMES StackSize*4 db 0
  
StackVM1:
  TIMES StackSize db 0
  

; Входим в ядро c замаскированными прерываниями
StartKernelCode: 
  mov ax, gdt_DataCode-gdt_0  ; сегмент кода ядра для записи
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ax, gdt_WriteAll-gdt_0  ; сегмент всего адресного пространства
  mov es, ax
  mov ax, gdt_Stack-gdt_0     ; сегмент стека
  mov ss, ax
  mov esp, StackSize        ; вершина стека StackSize-4
  ;mov bx, gdt_TSS-gdt_0
  ;ltr bx                      ; загружаем TR селектором TSS ядра    
  jmp skip
  mov ax, gdt_Video-gdt_0
  mov ds, ax
  mov ax, gdt_WriteAll-gdt_0
  mov es, ax
  mov ebx, [cs:VideoMode.Width]
  mov eax, 1024
  cmp ebx, 1024
  cmovg ebx, eax ; width in ebx
  mov eax, ebx
  mul dword [cs:VideoMode.PixelBytes]
  mov ebx, eax
  mov eax, 768
  mov edx, [cs:VideoMode.Height]
  cmp edx, 768   
  cmovg edx, eax ; height in edx
  mov esi, 0
  mov edi, 0x400000
  mov ecx, edx ; edx is free
  mov edx, ebx
  .forpic:
      mov eax, [es:edi]
      mov dword [ds:esi], eax
      add esi, [cs:VideoMode.PixelBytes]
      add edi, 4
      cmp esi, edx
    jl .forpic
    push edx
    mov eax, [cs:VideoMode.Width] 
    mul dword [cs:VideoMode.PixelBytes]
    pop edx
    add esi, eax
    add edx, eax
    sub esi, ebx
    push edx
    mov eax, ebx
    mov edx, 0
    div dword [cs:VideoMode.PixelBytes]
    pop edx
    add edi, 4*1024
    shl eax, 2
    sub edi, eax
  loop .forpic
  
  ;call stringadd
  skip:
  mov bx, gdt_DataCode-gdt_0
  mov es, bx
  mov bx, gdt_WriteAll-gdt_0
  mov ds, bx
    
  movzx eax,  word [cs:CPUFreq]
  mov ebx, 10
  mov edi, sMEM2
  call NumToASCII
  
  mov ax, gdt_Video-gdt_0
  mov gs, ax
  mov edi, 0
  mov esi, sCPU2
  
  mov eax, 0
  cpuid
  mov edi,sCPU2
  mov byte [es:edi],bl
  shr ebx, 8
  inc edi
  mov byte [es:edi],bl
  shr ebx, 8
  inc edi
  mov byte [es:edi],bl
  shr ebx, 8
  inc edi
  mov byte [es:edi],bl
  shr ebx, 8
  inc edi
  
  mov byte [es:edi],dl
  shr edx, 8
  inc edi
  mov byte [es:edi],dl
  shr edx, 8
  inc edi
  mov byte [es:edi],dl
  shr edx, 8
  inc edi
  mov byte [es:edi],dl
  shr edx, 8
  inc edi

  mov byte [es:edi],cl
  shr ecx, 8
  inc edi
  mov byte [es:edi],cl
  shr ecx, 8
  inc edi
  mov byte [es:edi],cl
  shr ecx, 8
  inc edi
  mov byte [es:edi],cl
  shr ecx, 8
  
  mov esi, sCPU1
  mov edi, 0   
  call PutString
  
  mov esi, string2
  call PutString
    
  mov esi, edi
  mov edi, 0
  
  ; разрешаем аппаратные прерывания и NMI
  in   al, 70h
  and  al, 7Fh
  out  70h, al
  sti
  
  finit
  fsetpm
  
%define size 300
  call DrawBuffer
jmp end_code

test:
pushad
push ds
push es
push fs
push gs

push word (begin_code.start-begin_code)
push word gdt_DataCode-gdt_0
push dword begin_code
push word (end_code-begin_code)

call near RealModeExecution

pop gs
pop fs
pop es
pop ds
popad
ret

begin_code:
BITS 16
  .text incbin 'art.txt'
  db 0
  .start:
  ;jmp short end_code 
  mov ax, 0x4F02
  mov bx, 0x03
  int 0x10
  mov ax, 0xB800
  mov es, ax
  mov si, .text-begin_code
  mov di, 0
  .for:
  mov al, [cs:si] 
  cmp al, 0
  jz short .stop
  cmp al, 0xA
  jnz short .notlf
  mov ax, di
  mov dx, 0
  add ax, 160-2
  mov bx, 160
  div bx
  mul bx
  mov di, ax
  inc si
  jmp short .for
  .notlf:   
  mov byte [es:di], al
  inc di
  inc si
  mov byte [es:di], 111b
  inc di
  jmp short .for
  .stop:
  
  ;mov eax, 0x90FFFFF
  ;.wait:
  ;dec eax
  ;jnz short .wait
  .wait:
  in al, 0x60
  cmp al, 0x3B
  jnz short .wait
  
  mov ax, 0x4F02
  mov ebp, Base+VideoMode.Number
  mov bx, [gs:ebp]
  or bx,1100000000000000b 
  int 0x10
end_code:
BITS 32

mov dword [es:multiple], 0
  
.prev:
  mov ebp, [cs:funsel]
  shl ebp, 2
  add ebp, function
  call near [cs:ebp]
  call DrawBuffer
  jmp .prev
  jmp $                       ; зацикливание системы
  
  
function:
  dd DrawNone, DrawFractal, DrawSpectr
funsel:
  dd 0
  
DrawNone:
ret

DrawSpectr:
  pushad
  mov ax, gdt_Video-gdt_0
  mov ds, ax
  mov eax, [cs:VideoMode.Height]
  mul dword [cs:VideoMode.Width]
  mul dword [cs:VideoMode.PixelBytes]
  mov esi, 0
  mov edx, [cs:VideoMode.PixelBytes]
  mov eax, [cs:VideoMode.Height]
  .fory:
    mov ecx, [cs:VideoMode.Width]
    call .getpixel
    .forx:
      mov dword [esi], ebx
      add esi, edx
    loop .forx    
    dec eax
  jnz .fory
  popad
  add dword [es:multiple], 10
  ret
  

; Вход: eax - смещение
; Выход: ebx - color
.getpixel:
  push eax
  push ecx
  push edx
  add eax, [cs:multiple]
  shl eax, 2
  mov cl, al
  shr eax, 8
  mov ebx, 6
  mov edx, 0
  div ebx
  shl edx, 2
  mov ebx, 0
  jmp dword [cs:.caseptr+edx]
  .a0:
    mov bl, 255
    shl ebx, 8
    mov bl, cl
    shl ebx, 8
    mov bl, 0
    jmp .end
  .a1:
    mov bl, 255
    sub bl, cl
    shl ebx, 8
    mov bl, 255
    shl ebx, 8
    mov bl, 0
    jmp .end
  .a2:
    mov bl, 0
    shl ebx, 8
    mov bl, 255
    shl ebx, 8
    mov bl, cl
    jmp .end
  .a3:
    mov bl, 0
    shl ebx, 8
    mov bl, 255
    sub bl, cl
    shl ebx, 8
    mov bl, 255
    jmp .end  
  .a4:
    mov bl, cl
    shl ebx, 8
    mov bl, 0
    shl ebx, 8
    mov bl, 255
    jmp .end
  .a5:
    mov bl, 255
    shl ebx, 8
    mov bl, 0
    shl ebx, 8
    mov bl, 255
    sub bl, cl
    jmp .end
.end:
  pop edx
  pop ecx
  pop eax
  ret  
.caseptr:
dd .a0, .a1, .a2, .a3, .a4, .a5
  
DrawFractal:

  mov edi, 0
  mov ebx, size
  mov ebp, [es:VideoMode.PixelBytes]
  .fory1:
    mov eax, size-1
    mul dword [es:VideoMode.PixelBytes] 
    add edi, eax
    mov ecx, size-1
    .forx1:
      sub edi, ebp    
      mov eax, [gs:edi]
      mov dword [gs:edi+ebp], eax  
    loop .forx1
    mov eax, [es:VideoMode.Width]
    mul dword [es:VideoMode.PixelBytes]
    add edi, eax
    dec ebx
    jnz .fory1
    

  mov edi, 0
  mov ebx, size
  mov ebp, [es:VideoMode.PixelBytes]
  .fory:
    mov ecx, 1
    .forx:    
      mov eax, ecx
      add eax, [es:multiple]
      mov dword [es:x], eax
      sub dword [es:x], 0
      mov dword [es:y], ebx
      sub dword [es:y], size/6
      jmp .getpixel
      .getpixelret:
      movzx eax, dl
      shl eax, 8
      mov al, dl
      shl eax, 8
      mov dword [gs:edi], eax
      add edi, ebp
    loop .forx
    mov eax, [es:VideoMode.Width]
    sub eax, 1
    mul dword [es:VideoMode.PixelBytes]
    add edi, eax
    dec ebx
    jnz .fory
    add dword [es:multiple], 1
    cmp dword [es:multiple], size
    jnz $+9
    mov dword [es:multiple], 0
ret

.getpixel:
  mov dl, 0
  fild dword [es:c100]  
  fld ST0 
  fild dword [es:x]
  fsub
  fdivr    ; m
  fild dword [es:c100] 
  fld ST0   
  fild dword [es:y]
  fsub
  fdivr    ; n
  fldz    ; c
  fldz    ; d
  .for:
    fld ST1 ; c
    fmul ST1 ; d*c
    fadd ST0  ; 2*d*c
    fadd ST3 ; 2*d*c+n
    fld ST2  ; c
    fmul ST0 ; c*c
    fld ST2  ; d
    fmul ST0 ; d*d
    fsub    ; c*c-d*d
    fadd  ST5   ; c*c-d*d+m
    fstp ST3
    fstp ST1    
     
    fld ST0
    fmul ST0
    fld ST2
    fmul ST0
    fadd    
    
    fcomp dword [es:c200]
    
    inc dl
    cmp dl, 63
    jz .next2
        
    fstsw ax                                               
    test ax, 100000000b
    jnz .for  
    
  .next1:
  TIMES 4 fistp dword [es:none] 
  shl dl, 2
jmp .getpixelret         

  .next2:
  TIMES 4 fistp dword [es:none] 
  mov dl, 0
jmp .getpixelret   

;        a:=c*c-d*d+m;   // (c+di)=(c+di)^2+(m+ni)
;        b:=2*c*d+n;

x dd 0   ; int32
y dd 0   ; int32
c100 dd size/3 ; shortreal
c200 dd 100.0
none dd 0
multiple dd 1
  

sCPU1:
  db 'CPU ManufacturerID: '
sCPU2:
  db '            ', 0xA  
sMEM1:
  db 'CPU Frequency: '
sMEM2:
  db '        KHz' ,0xA, 0 
string2:
  db `F1   help \n`
  db `F2   system information\n`
  db `F3   reset cursor\n`
  db `F4   draw picture\n`
  db `ESC  change videomode\n`
  db `F10  restart kernel\n`
  db `->   select application\n`
  db `DEL  clear screen\n`
  db 0

  
;===============================================================================
;                               Функция 0
;-------------------------------------------------------------------------------
; На входе последовательно записываются в стек:
;         начальный объем памяти в страницах по 4 КБ (dword)
;         код класса драйвера (ID не учитывается) (dword)
; На выходе: 
;-------------------------------------------------------------------------------
DriverCreate:
  pushad
  mov ebp, esp
  add ebp, 42 
  popad
  iretd
  
; Дескриптор GDT для LDT драйвера
driver_ldt:
  dw 8*4-1                          ; предел (0..15)
  dw 0                              ; адрес (0..15)
  db 0                              ; адрес (16..23)
  db 10000010b                      ; доступ
  db 00000000b                      ; предел (16..23) и прочее
  db 0                              ; адрес (24..31)
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
      add esi, [cs:VideoMode.PixelBytes]
      inc edi
      dec ecx
    jnz .forx
    push edx
    mov eax, dword [cs:VideoMode.Width]
    sub eax, 8
    mul dword [cs:VideoMode.PixelBytes]    
    add esi, eax
    pop edx
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
;             es:esi - адрес ASCII-Z строки
;             gs:edi - адрес записи в видеобуфер
; На выходе:  edi - номер после последнего символа
;-------------------------------------------------------------------------------
PutString:
  pushad
  mov eax, 8
  mul dword [cs:VideoMode.PixelBytes]
  mov ebp, eax
  mov eax, [cs:VideoMode.PixelBytes]
  mul dword [cs:VideoMode.Width]
  mov ecx, eax
  .for:
  movzx ax, byte [es:esi]
  cmp ax, 0xA
  jnz .notlf
    mov edx, 0
    mov eax, edi
    div ecx
    add eax, 16
    mul ecx
    mov edi, eax
    jmp .next
  .notlf:
  push ax
  push edi
  call PutChar
  add esp, 6
  add edi, ebp
  mov eax, edi
  mov edx, 0
  div ecx
  cmp edx, 0
  jnz .next
    mov eax, 15
    mul ecx
    add edi, eax
  .next:
  inc esi
  cmp byte [es:esi], 0
  jnz .for
  mov dword [ss:esp], edi
  popad
  ret
;===============================================================================

;===============================================================================
;             RAM to Videobuffer
;-------------------------------------------------------------------------------
  DrawBuffer:
  cli
    pushad
    push ds
    push fs
    mov ax, gdt_DataCode-gdt_0
    mov ds, ax
    mov eax, [VideoMode.Width]
    mul dword [VideoMode.Height]
    mul dword [VideoMode.PixelBytes]
    mov ecx, eax
    mov esi, 0
    mov ax, gdt_Video-gdt_0
    mov ds, ax
    mov ax, gdt_Video2-gdt_0
    mov fs, ax
    shr ecx, 3
    dec ecx
    mov eax, 8
    .for:
      movq mm0, [esi]
      movq [fs:esi], mm0
      add esi, eax
    dec ecx
    jnz .for
    pop fs
    pop ds
    popad
    sti
    ret
;===============================================================================


%macro take_reg 1
  mov bx, gdt_DataCode-gdt_0
  mov es, bx
  mov bx, gdt_WriteAll-gdt_0
  mov ds, bx
    
  mov eax,  %1
  mov ebx, 16
  mov edi, sMEM2
  call NumToASCII
  
  mov ax, gdt_Video-gdt_0
  mov gs, ax
  
  mov esi, sMEM2
  mov edi, 0   
  call PutString  
  
   jmp $  
%endmacro

;===============================================================================
;                      Выполнить код в (не)реальном режиме
;-------------------------------------------------------------------------------
; На входе последовательно записываются в стек:  
;        WORD start
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
;    Управление будет передано по адресу 0x1000:start
;    Программа будет дополнена кодом возврата в защищенный режим
; Выход:             
;    В регистрах eax, ebx, ecx, edx, esi, edi, ebp будет то, что останется
;    после выполнения программы.
;-------------------------------------------------------------------------------
RealModeExecution:
  mov ax, gdt_WriteAll-gdt_0;
  mov gs, ax
  mov ax, gdt_Video-gdt_0;
  mov fs, ax
  mov dword [gs:Base+TEMP_ESP], esp
  mov word [gs:Base+TEMP_SS], ss

  ;pushad 
  movzx ecx, word [ss:esp+4]

  mov edi, [ss:esp+2+4]
  mov ax, [ss:esp+6+4]

  mov dx, [ss:esp+6+4+2]
  ;mov ax, gdt_DataCode-gdt_0

  mov es, ax    
  mov ax, gdt_DataCodeVM-gdt_0
  mov ds, ax
  mov ebp, ecx
  add ecx, 3
  shr ecx, 2  
  mov esi, 0 
  .for:       ; переписываем код
    mov eax, dword [es:edi]
    mov dword [esi], eax
    add esi, 4
    add edi, 4
  loop .for  
; SS for EBP, DS for other
  mov byte [ds:ebp+0], 0xEA
  mov word [ds:ebp+1], Back_To_PM-StartLoader
  mov word [ds:ebp+3], 0x1000 
   
; запрет всех прерываний
  cli
  in   al, 70h
  or   al, 80h
  out  70h, al  ; запрет NMI  
  
  
  mov ax, gdt_DataVM-gdt_0
  mov es, ax
  mov ds, ax
  mov ss, ax
  ;mov fs,ax
  mov ax, gdt_WriteAll-gdt_0;gdt_Video-gdt_0;
  mov gs, ax
  mov ax, gdt_Video2-gdt_0
  mov fs, ax
  
  mov eax,0
  mov cr3, eax  

  
  lidt [cs:l_IDTLimit] 
  jmp gdt_CodeLoader-gdt_0:Come_To_Real-StartLoader
  mov  eax, cr0
  and  al, 0FEh
  mov  cr0, eax
  ;push word gdt_Code-gdt_0
  ;push dword .next  
  ;jmp 0x1000:Come_To_Real-StartLoader ; ДАЛЬНИЙ ПЕРЕХОД
  
  ;call 0x1000:Come_To_Real-StartLoader ; ДАЛЬНИЙ ПЕРЕХОД 
  Rnext:
  mov ax, gdt_DataCode-gdt_0
  mov es, ax

  mov ax, gdt_DataCode-gdt_0  ; сегмент кода ядра для записи
  mov ds, ax
  mov ax, gdt_WriteAll-gdt_0  ; сегмент всего адресного пространства
  mov es, ax
  mov ax, gdt_Stack-gdt_0     ; сегмент стека
  mov ss, ax
  ;mov esp, StackSize-4        ; вершина стека StackSize-4 
  ;popad
  mov ax, [gs:Base+TEMP_SS]
  mov ss,ax
  mov dword esp, [gs:Base+TEMP_ESP]
  ;add esp, PUSHAD_SIZE
  ;mov ax, gdt_WriteAll-gdt_0
  ;mov ds, ax
  ;mov esp, [ds:0x10000+PM_ESP] 
  ;mov ax, gdt_Stack-gdt_0 
  ;mov ss, ax  
  ;hlt
  ;jmp $-1
    ;take_reg ecx
  sti
  ;take_reg ecx
  ;jmp $
  mov eax, [ss:esp]
  mov [ss:esp+10], eax
  add esp, 10
  ret
;===============================================================================

TEMP_ESP dd 0
TEMP_SS dw 0


TIMES 512*1024-($-$$) db 0     ; размер ядра - 512 КБ

EndKernel:
