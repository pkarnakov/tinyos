;===============================================================================
;                         Global Descriptor Table
;                     Глобальная таблица дескрипторов
;===============================================================================

; поле доступа - 5 байт дескриптора

gdt_0:                        ; Пустой дескриптор
  dw 0
  dw 0
  db 0
  db 0
  dw 0
gdt_gdt:                      ; Дескриптор таблицы GDT
  dw gdt_end-gdt_0-1                ; предел (0..15)
  dw GDTPtr&0xFFFF                  ; адрес (0..15)
  db (GDTPtr>>16)&0xFF              ; адрес (16..23)
  db 10010010b                      ; доступ
  db 01000000b+(gdt_end-gdt_0-1)>>16&4                      ; предел (16..23) и прочее
  db (GDTPtr>>24)&0xFF              ; адрес (24..31)
gdt_Code:                     ; Дескриптор кода ядра
  dw(EndKernel-StartKernel)/4096    ; предел (0..15)                 
  dw KernelPtr&0xFFFF               ; адрес (0..15)                  
  db (KernelPtr>>16)&0xFF           ; адрес (16..23)                 
  db 10011010b                      ; доступ                         
  db 11000000b                      ; предел (16..23) и прочее       
  db (KernelPtr>>24)&0xFF           ; адрес (24..31)    
gdt_DataCode:                 ; Дескриптор данных(кода) ядра
  dw (EndKernel-StartKernel)/4096   ; предел (0..15)    
  dw KernelPtr&0xFFFF               ; адрес (0..15)                    
  db (KernelPtr>>16)&0xFF           ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 11000000b                      ; предел (16..23) и прочее              
  db (KernelPtr>>24)&0xFF           ; адрес (24..31)      
gdt_Stack:                    ; Дескриптор стека
  dw StackSize/4096                 ; предел (0..15)    
  dw Stack&0xFFFF                   ; адрес (0..15)                    
  db (Stack>>16)&0xFF               ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 11000000b                      ; предел (16..23) и прочее              
  db (Stack>>24)&0xFF               ; адрес (24..31)        
gdt_Stack0:                   ; Дескриптор стека
  dw StackSize/4096                 ; предел (0..15)    
  dw Stack0&0xFFFF                  ; адрес (0..15)                    
  db (Stack0>>16)&0xFF              ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 11000000b                      ; предел (16..23) и прочее              
  db (Stack0>>24)&0xFF              ; адрес (24..31)          
gdt_Stack1:                   ; Дескриптор стека
  dw StackSize/4096                 ; предел (0..15)    
  dw Stack1&0xFFFF                  ; адрес (0..15)                    
  db (Stack1>>16)&0xFF              ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 11000000b                      ; предел (16..23) и прочее              
  db (Stack1>>24)&0xFF              ; адрес (24..31)           
gdt_Stack2:                   ; Дескриптор стека
  dw StackSize/4096                 ; предел (0..15)    
  dw Stack2&0xFFFF                  ; адрес (0..15)                    
  db (Stack2>>16)&0xFF              ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 11000000b                      ; предел (16..23) и прочее              
  db (Stack2>>24)&0xFF              ; адрес (24..31)                            
gdt_WriteAll:                 ; Дескриптор всего адресного пространства
  dw 0xFFFF                         ; предел (0..15)                 
  dw 0                              ; адрес (0..15)                  
  db 0                              ; адрес (16..23)                 
  db 10010010b                      ; доступ                         
  db 11001111b                      ; предел (16..23) и прочее       
  db 00000000b                      ; адрес (24..31)                 
gdt_GlobalValues:             ; Дескриптор глобальных переменных
  dw GlobalValuesEnd-GlobalValues   ; предел (0..15)   
  dw 0                              ; адрес (0..15)    
  db 0                              ; адрес (16..23)   
  db 10010010b                      ; доступ           
  db 00000000b                      ; предел (16..23) и прочее
  db 00000000b                      ; адрес (24..31)      
gdt_Video2:                    ; Видеобуфер
  dw 0xFFFF                         ; предел (0..15)           
  dw 0                              ; адрес (0..15)            
  db 0                              ; адрес (16..23)           
  db 10110010b                      ; доступ                   
  db 11000000b                      ; предел (16..23) и прочее 
  db 00000000b                      ; адрес (24..31)
gdt_Video:                    ; Видеобуфер
  dw 0xFFFF                         ; предел (0..15)           
  dw 0                              ; адрес (0..15)            
  db 0x30                           ; адрес (16..23)           
  db 10110010b                      ; доступ                   
  db 11000000b                      ; предел (16..23) и прочее 
  db 00000000b                      ; адрес (24..31)
gdt_CodeVM:                     ; Дескриптор кода VM86
  dw 0xFFFF                         ; предел (0..15)                 
  dw 0x0000                         ; адрес (0..15)                  
  db 0x04                           ; адрес (16..23)                 
  db 10011010b                      ; доступ                         
  db 00000000b                      ; предел (16..23) и прочее       
  db 0                              ; адрес (24..31)   
gdt_CodeLoader:                     ; Дескриптор кода VM86
  dw 0xFFFF                         ; предел (0..15)                 
  dw 0x0000                         ; адрес (0..15)                  
  db 0x01                           ; адрес (16..23)                 
  db 10011010b                      ; доступ                         
  db 00000000b                      ; предел (16..23) и прочее       
  db 0                              ; адрес (24..31)  
gdt_StackVM:                    ; Дескриптор стека VM86
  dw 0xFFFF                         ; предел (0..15)    
  dw 0x0000                         ; адрес (0..15)                    
  db 0x02                           ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 00000000b                      ; предел (16..23) и прочее              
  db 0                              ; адрес (24..31)    
gdt_DataVM:                    ; Дескриптор данных VM86
  dw 0xFFFF             ; предел (0..15)    
  dw 0x0000                         ; адрес (0..15)                    
  db 0x03                           ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 00000000b                      ; предел (16..23) и прочее              
  db 0                              ; адрес (24..31)      
gdt_DataCodeVM:                    ; Дескриптор кода для записи VM86
  dw 0xFFFF             ; предел (0..15)    
  dw 0x0000                         ; адрес (0..15)                    
  db 0x04                           ; адрес (16..23)                 
  db 10010010b                      ; доступ                                
  db 00000000b                      ; предел (16..23) и прочее              
  db 0                              ; адрес (24..31)     
   
  
;TIMES 0x10000-($-gdt_0) db 0
gdt_end:




idt_0:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_2:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_3:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_4:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_5:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_6:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_7:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_8:
     dw (exc1-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc1-StartGelios)>>16)    ; смещение (16..31)
idt_9:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_A:
     dw (exc1-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc1-StartGelios)>>16)    ; смещение (16..31)
idt_B:
     dw (exc1-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc1-StartGelios)>>16)    ; смещение (16..31)
idt_C:
     dw (exc1-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc1-StartGelios)>>16)    ; смещение (16..31)
idt_D:
     dw (exc1-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc1-StartGelios)>>16)    ; смещение (16..31)
idt_E:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_F:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_10:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_11:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_12:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_13:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_14:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_15:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_16:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_17:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_18:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_19:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1A:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1B:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1C:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1D:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1E:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_1F:
     dw (exc0-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001111b                   ; доступ
     dw ((exc0-StartGelios)>>16)    ; смещение (16..31)
idt_20:                       ; IRQ0
     dw (Timer-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_21:                       ; IRQ1
     dw (KeyMan-StartGelios)&0xFFFF        ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((KeyMan-StartGelios)>>16)  ; смещение (16..31)
idt_22:                       ; IRQ2
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_23:                       ; IRQ3
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_24:                       ; IRQ4
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_25:                       ; IRQ5
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_26:                       ; IRQ6
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_27:                       ; IRQ7
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_28:                       ; IRQ8
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_29:                       ; IRQ9
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_2A:                       ; IRQ10
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_2B:                       ; IRQ11
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_2C:                       ; IRQ12
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_2D:                       ; IRQ13
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_2E:                       ; IRQ14
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)
idt_2F:                       ; IRQ15
     dw (irq-StartGelios)&0xFFFF          ; смещение (0..15)
     dw gdt_Code-gdt_0              ; селектор
     db 0                           ; резерв
     db 10001110b                   ; доступ
     dw ((irq-StartGelios)>>16)    ; смещение (16..31)   
idt_end: