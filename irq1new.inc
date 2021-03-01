; Читается скан-код клавиши, который служит для перехода
; Индексом служат только младшие 7 бит
; т.к. старший бит обозначает статус клавиши
irq1:
pushad
push es
in al, 0x60
xor ebx, ebx
mov bl, al
and bl, 01111111b
lea ebx, [ebx*4+keysjumptable]
jmp near [cs:ebx]
; Оригинальный скан-код остается в "al"
; Обработчики разных клавиш:
key0:
jmp keyend 
key1:
jmp keyend
key2:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+0*16+0]
add ebx, [es:apptable+0*16+4]
mov ecx, [es:apptable+0*16+8]
mov edx, [es:apptable+0*16+12]
int 0x30
jmp keyend
key3:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+1*16+0]
add ebx, [es:apptable+1*16+4]
mov ecx, [es:apptable+1*16+8]
mov edx, [es:apptable+1*16+12]
int 0x30
jmp keyend
key4:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+2*16+0]
add ebx, [es:apptable+2*16+4]
mov ecx, [es:apptable+2*16+8]
mov edx, [es:apptable+2*16+12]
int 0x30
jmp keyend
key5:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+3*16+0]
add ebx, [es:apptable+3*16+4]
mov ecx, [es:apptable+3*16+8]
mov edx, [es:apptable+3*16+12]
int 0x30
jmp keyend
key6:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+4*16+0]
add ebx, [es:apptable+4*16+4]
mov ecx, [es:apptable+4*16+8]
mov edx, [es:apptable+4*16+12]
int 0x30
jmp keyend
key7:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+5*16+0]
add ebx, [es:apptable+5*16+4]
mov ecx, [es:apptable+5*16+8]
mov edx, [es:apptable+5*16+12]
int 0x30
jmp keyend
key8:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+6*16+0]
add ebx, [es:apptable+6*16+4]
mov ecx, [es:apptable+6*16+8]
mov edx, [es:apptable+6*16+12]
int 0x30
jmp keyend
key9:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+7*16+0]
add ebx, [es:apptable+7*16+4]
mov ecx, [es:apptable+7*16+8]
mov edx, [es:apptable+7*16+12]
int 0x30
jmp keyend
keyA:
test al, 10000000b
jz near keyend
cmp byte [cs:keybufferstatus], 1
jnz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov byte [es:keybufferstatus], 0
mov eax, [es:basecode]
mov ebx, eax
add eax, [es:apptable+8*16+0]
add ebx, [es:apptable+8*16+4]
mov ecx, [es:apptable+8*16+8]
mov edx, [es:apptable+8*16+12]
int 0x30
jmp keyend
keyB:
jmp keyend
keyC:
jmp keyend
keyD:
jmp keyend
keyE:
jmp keyend
keyF:
jmp keyend
key10:
test al, 10000000b
jz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
mov dword [es:tsswrite], 0
mov byte [es:counttask], 0
mov byte [es:curtask], 0
mov al, 0x20
out 0x20, al
test byte [es:gdt.tss+5], 10b
jnz .next
jmp gdt.tss-gdt:0
.next:
pop es
popad
iretd
key11:
test al, 10000000b
jz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
cmp dword [es:tsswrite], 0
jz near keyend
sub dword [es:tsswrite], sumsize
dec byte [es:counttask]
dec byte [es:curtask]
jmp keyend
key12:
jmp keyend
key13:
test al, 10000000b
jz near keyend
mov ax, gdt.cswr-gdt
mov es, ax
test byte [es:keystatus], 00001000b
jz near keyend
mov byte [es:keybufferstatus], 1
jmp keyend
key14:
jmp keyend
key15:
jmp keyend
key16:
jmp keyend
key17:
jmp keyend
key18:
jmp keyend
key19:
jmp keyend
key1A:
jmp keyend
key1B:
jmp keyend
key1C:
jmp keyend
key1D:
mov bx, gdt.cswr-gdt
mov es, bx
or al, 01111111b
not al
shr al, 4
and byte [es:keystatus], 11110111b
or byte [es:keystatus], al
jmp keyend
key1E:
jmp keyend
key1F:
jmp keyend
key20:
jmp keyend
key21:
jmp keyend
key22:
jmp keyend
key23:
jmp keyend
key24:
jmp keyend
key25:
jmp keyend
key26:
jmp keyend
key27:
jmp keyend
key28:
jmp keyend
key29:
jmp keyend
key2A:
mov bx, gdt.cswr-gdt
mov es, bx
or al, 01111111b
not al
shr al, 6
and byte [es:keystatus], 11111101b
or byte [es:keystatus], al
jmp keyend
key2B:
jmp keyend
key2C:
jmp keyend
key2D:
jmp keyend
key2E:
jmp keyend
key2F:
jmp keyend
key30:
jmp keyend
key31:
jmp keyend
key32:
jmp keyend
key33:
jmp keyend
key34:
jmp keyend
key35:
jmp keyend
key36:
mov bx, gdt.cswr-gdt
mov es, bx
or al, 01111111b
not al
shr al, 7
and byte [es:keystatus], 11111110b
or byte [es:keystatus], al
jmp keyend
key37:
jmp keyend
key38:
mov bx, gdt.cswr-gdt
mov es, bx
or al, 01111111b
not al
shr al, 5
and byte [es:keystatus], 11111011b
or byte [es:keystatus], al
jmp keyend
key39:
jmp keyend
key3A:
test al, 10000000b
jz near keyend
mov bx, gdt.cswr-gdt
mov es, bx
mov bl, [es:keystatus]
or bl, 11011111b
not bl
and byte [es:keystatus], 11011111b
or byte [es:keystatus], bl
jmp keyend
key3B:
jmp keyend
key3C:
jmp keyend
key3D:
jmp keyend
key3E:
jmp keyend
key3F:
jmp keyend
key40:
jmp keyend
key41:
jmp keyend
key42:
jmp keyend
key43:
jmp keyend
key44:
jmp keyend
key45:
test al, 10000000b
jz near keyend
mov bx, gdt.cswr-gdt
mov es, bx
mov bl, [es:keystatus]
or bl, 10111111b
not bl
and byte [es:keystatus], 10111111b
or byte [es:keystatus], bl
jmp keyend
key46:
test al, 10000000b
jz near keyend
mov bx, gdt.cswr-gdt
mov es, bx
mov bl, [es:keystatus]
or bl, 11101111b
not bl
and byte [es:keystatus], 11101111b
or byte [es:keystatus], bl
jmp keyend
key47:
jmp keyend
key48:
jmp keyend
key49:
jmp keyend
key4A:
jmp keyend
key4B:
jmp keyend
key4C:
jmp keyend
key4D:
jmp keyend
key4E:
jmp keyend
key4F:
jmp keyend
key50:
jmp keyend
key51:
jmp keyend
key52:
jmp keyend
key53:
jmp keyend
key54:
jmp keyend
key55:
jmp keyend
key56:
jmp keyend
key57:
jmp keyend
key58:
jmp keyend
key59:
jmp keyend
key5A:
jmp keyend
key5B:
jmp keyend
key5C:
jmp keyend
key5D:
jmp keyend
key5E:
jmp keyend
key5F:
jmp keyend
key60:
jmp keyend
key61:
jmp keyend
key62:
jmp keyend
key63:
jmp keyend
key64:
jmp keyend
key65:
jmp keyend
key66:
jmp keyend
key67:
jmp keyend
key68:
jmp keyend
key69:
jmp keyend
key6A:
jmp keyend
key6B:
jmp keyend
key6C:
jmp keyend
key6D:
jmp keyend
key6E:
jmp keyend
key6F:
jmp keyend
key70:
jmp keyend
key71:
jmp keyend
key72:
jmp keyend
key73:
jmp keyend
key74:
jmp keyend
key75:
jmp keyend
key76:
jmp keyend
key77:
jmp keyend
key78:
jmp keyend
key79:
jmp keyend
key7A:
jmp keyend
key7B:
jmp keyend
key7C:
jmp keyend
key7D:
jmp keyend
key7E:
jmp keyend
key7F:
keyend:
mov al, 0x20
out 0x20, al
pop es
popad
iretd

keybufferstatus db 0

; Таблица смещений для ближних переходов:
; key#, где # соответствует младшим 7 битам скан-кода
keysjumptable:
dd key0
dd key1
dd key2
dd key3
dd key4
dd key5
dd key6
dd key7
dd key8
dd key9
dd keyA
dd keyB
dd keyC
dd keyD
dd keyE
dd keyF
dd key10
dd key11
dd key12
dd key13
dd key14
dd key15
dd key16
dd key17
dd key18
dd key19
dd key1A
dd key1B
dd key1C
dd key1D
dd key1E
dd key1F
dd key20
dd key21
dd key22
dd key23
dd key24
dd key25
dd key26
dd key27
dd key28
dd key29
dd key2A
dd key2B
dd key2C
dd key2D
dd key2E
dd key2F
dd key30
dd key31
dd key32
dd key33
dd key34
dd key35
dd key36
dd key37
dd key38
dd key39
dd key3A
dd key3B
dd key3C
dd key3D
dd key3E
dd key3F
dd key40
dd key41
dd key42
dd key43
dd key44
dd key45
dd key46
dd key47
dd key48
dd key49
dd key4A
dd key4B
dd key4C
dd key4D
dd key4E
dd key4F
dd key50
dd key51
dd key52
dd key53
dd key54
dd key55
dd key56
dd key57
dd key58
dd key59
dd key5A
dd key5B
dd key5C
dd key5D
dd key5E
dd key5F
dd key60
dd key61
dd key62
dd key63
dd key64
dd key65
dd key66
dd key67
dd key68
dd key69
dd key6A
dd key6B
dd key6C
dd key6D
dd key6E
dd key6F
dd key70
dd key71
dd key72
dd key73
dd key74
dd key75
dd key76
dd key77
dd key78
dd key79
dd key7A
dd key7B
dd key7C
dd key7D
dd key7E
dd key7F
