;===============================================================================
;                         Interrupt Descriptor Table
;                  Таблица векторов прерываний и исключений
;===============================================================================



exc0:                         ; Обработчик исключений без кода ошибки
  iretd

exc1:                         ; Обработчик исключений с кодом ошибки
  add esp, 2                  ; Извлечение кода ошибки из стека
  iretd
  
irq:
  push eax
  mov al, 0x20
  out 0x20, al
  pop eax
  iretd
  
isup: dd 0

Timer:
  pushad
  mov al,  0x20
  out 0x20, al
  popad
  iretd
  
KeyMan:
  push eax
  push ebx
  push ecx
  push edx
  push edi
  push ebp
  mov eax, 0
  in al, 0x60
  mov bl, 0
  test al, 10000000b
  jnz .isdown
  mov bl, 1
  .isdown:
  mov edi, eax
  and edi, 01111111b
  mov byte [es:Keyboard.Keys+edi], bl  
  cmp al, 1  ; ESC-P
  jnz .next_reboot
    call near test
    jmp .next
  .next_reboot:
  cmp al, 0x3B  ; F1
  jnz .next_help
    mov edi, esi
    mov esi, string2
    call PutString
    mov esi, edi
    jmp .next
  .next_help:
  cmp al, 0x3C  ; F2
  jnz .next_info
    mov edi, esi
    mov esi, sCPU1
    call PutString
    mov esi, edi
    jmp .next
  .next_info:
  cmp al, 0x3D  ; F3
  jnz .next_ret
    mov esi, 0
    jmp .next
  .next_ret:
  cmp al, 0x44  ; F10
  jnz .next_reboot2
; Входим в реальный режим
  mov al, 0x20
  out 0x20, al
  pop ebp
  pop edi
  pop edx
  pop ecx
  pop ebx
  pop eax
  jmp gdt_CodeVM-gdt_0:ifkernel2
  .next_reboot2:
  cmp al, 0x53     ; delete
  jnz .next_clear
  mov eax, [cs:VideoMode.Width]
  mul dword [cs:VideoMode.PixelBytes]
  mul dword [cs:VideoMode.Height]
  mov edi, eax
  PCMPEQD mm0, mm0
  .forclear:
  sub edi, 8
  movq [gs:edi], mm0
  jnz .forclear
  mov esi, 0
  jmp .next
  .next_clear:
  cmp al, 0x4D   ; right arrow
  jnz .next_right
  mov eax, [cs:funsel]
  inc eax
  mov ebx, 3
  mov edx, 0
  div ebx
  mov dword [es:funsel], edx
  mov dword [es:multiple], 0
  jmp .next
  .next_right:
  cmp al, 0x3E  ; F4
  jnz .next_pic
      PUSH ES
      PUSH DS
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
    mov esi, 0
    POP DS
    POP ES
    jmp .next
  .next_pic:
  cmp al, 0x1C      ; enter
  jnz .next_enter
  mov eax, [cs:VideoMode.Width]
  mul dword [cs:VideoMode.PixelBytes]
  mov ebx, eax
  mov eax, esi
  mov edx, 0  
  div ebx
  sub esi, edx
  mov eax, [cs:VideoMode.Width]
  mul dword [cs:VideoMode.PixelBytes]
  shl eax, 4
  add esi, eax
  jmp .next
  .next_enter:
  test al, 10000000b
  jnz .next
  movzx ebx, al
  mov byte al, [cs:keytable+ebx]  ; символ для вывода
  mov cl, [cs:Keyboard.Keys+0x2A]
  or cl, [cs:Keyboard.Keys+0x36]
  cmp cl,0
  jz .nexthigh
  mov byte al, [cs:keytable.shift+ebx]
  .nexthigh:
  cmp al, 0
  jz .next
  movzx ax, al
  push ax
  push esi
  call near PutChar
  add esp, 6
  mov eax, 8
  mul dword [cs:VideoMode.PixelBytes]
  add esi, eax
  mov eax, [cs:VideoMode.Width]
  mul dword [cs:VideoMode.PixelBytes]  
  mov ebx, eax
  mov eax, esi
  mov edx, 0
  div ebx
  cmp edx, 0
  jnz .next_over
  mov eax, [cs:VideoMode.Width]
  mul dword [cs:VideoMode.PixelBytes]
  mov ebx, 15
  mul ebx
  add esi, eax
  .next_over:
  .next:
  call DrawBuffer  
  mov al, 0x20
  out 0x20, al
  pop ebp
  pop edi
  pop edx
  pop ecx
  pop ebx
  pop eax
  


  iretd
  
jmpptrkey:
dd ifkernel
dw gdt_CodeVM-gdt_0
  

        db 0,0,'1','2','3','4','5','6','7','8','9','0','-','=',0,0,'q','w','e','r','t','y','u','i','o','p','[',']',0,0,'a','s','d','f','g','h','j','k','l',';','‘','`',0,'\','z','x','c','v','b','n','m',',','.','/',0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.shift: db 0,0,'!','@','#','$','%','^','&','*','(',')','_','+',0,0,'Q','W','E','R','T','Y','U','I','O','P','{','}',0,0,'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V','B','N','M','<','>','?',0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
keytable:        db 0,0,'1','2','3','4','5','6','7','8','9','0','-','=',0,0,'й','ц','у','к','е','н','г','ш','щ','з','х','ъ',0,0,'ф','ы','в','а','п','р','о','л','д','ж','э','ё',0,'\','я','ч','с','м','и','т','ь','б','ю','.',0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.shift: db 0,0,'!','"','±',';','%',':','?','*','(',')','_','+',0,0,'Й','Ц','У','К','Е','Н','Г','Ш','Щ','З','Х','Ъ',0,0,'Ф','Ы','В','А','П','Р','О','Л','Д','Ж','Э','Ё',0,'/','Я','Ч','С','М','И','Т','Ь','Б','Ю',',',0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

