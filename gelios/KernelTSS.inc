;===============================================================================
;                      Сегмент состояния задачи (TSS) ядра
;                 Служит для инициализации регистров процессора
;===============================================================================
TSS:
  dd 0                        ; Link
  dd StackSize-1              ; esp0
  dd gdt_Stack0-gdt_0         ; ss0
  dd StackSize-1              ; esp1
  dd gdt_Stack1-gdt_0         ; ss1
  dd StackSize-1              ; esp2
  dd gdt_Stack2-gdt_0         ; ss2
  dd 0                        ; cr3
  dd StartKernelCode          ; eip
  dd 10b                      ; eflags
  dd 0                        ; eax
  dd 0                        ; ecx
  dd 0                        ; edx
  dd 0                        ; ebx
  dd StackSize-1              ; esp
  dd 0                        ; ebp
  dd 0                        ; esi
  dd 0                        ; edi
  dd gdt_WriteAll-gdt_0       ; es
  dd gdt_Code-gdt_0           ; cs
  dd gdt_Stack-gdt_0          ; ss
  dd gdt_DataCode-gdt_0       ; ds
  dd 0                        ; fs
  dd 0                        ; gs
  dd 0                        ; LDTR
  dd 0                        ; i/o map
TSS_End: