; Участки кода, обрабатывающие исключения
exc0:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '0' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '1' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc2:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '2' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc3:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '3' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc4:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '4' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc5:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '5' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc6:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '6' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
mov ax, gdt.cswr-gdt
mov es, ax
dec byte [es:counttask]
mov byte [es:curtask], 0
sub dword [es:tsswrite], sumsize
jmp gdt.tss-gdt:0
pop edi
pop ebp
pop fs
pop ax
iretd

exc7:
;push ax
;push fs
;push ebp
;push edi
;mov ax, gdt.cswr-gdt
;mov fs, ax
;mov byte [fs:textenum], '0' 
;mov byte [fs:textenum+1], '7' 
;mov ebp, texte
;mov edi, visizew*vibytes*(visizeh/2)
;int 0x32
;pop edi
;pop ebp
;pop fs
;pop ax
clts
iretd

exc8:
inc esp
inc esp
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '8' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc9:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], '9' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

excA:
inc esp
inc esp
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], 'A' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

excB:
inc esp
inc esp
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], 'B' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

excC:
inc esp
inc esp
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], 'C' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

excD:
inc esp
inc esp
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], 'D' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

excE:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], 'E' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

excF:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '0' 
mov byte [fs:textenum+1], 'F' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc10:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '0' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc11:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '1' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc12:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '2' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc13:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '3' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc14:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '4' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc15:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '5' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc16:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '6' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc17:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '7' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc18:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '8' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc19:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], '9' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1A:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], 'A' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1B:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], 'B' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1C:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], 'C' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1D:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], 'D' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1E:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], 'E' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd

exc1F:
push ax
push fs
push ebp
push edi
mov ax, gdt.cswr-gdt
mov fs, ax
mov byte [fs:textenum], '1' 
mov byte [fs:textenum+1], 'F' 
mov ebp, texte
mov edi, visizew*vibytes*(visizeh/2)
int 0x32
pop edi
pop ebp
pop fs
pop ax
iretd
