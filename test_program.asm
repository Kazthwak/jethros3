[bits 32]
[org 0x400]

mov eax, 0
mov ah, 4
mov ebx, 0
int 0x30

mov eax, 0
mov ah, 3
int 0x30

mov eax, 0
mov ebx, str
int 0x30

mov ecx, 0
push ecx




loop:
mov ebx, 0
mov eax, 2
shl eax, 16
mov ah, 0
int 0x30
cmp ebx, 0
je loop
mov eax, 2
shl eax, 16
mov ah, 1
int 0x30

;ebx has key press
cmp bl, 0
je loop

pop ecx
inc ecx
push ecx

print:
call clear_screen	
pop ecx
mov ebx, ecx
mov eax, 0
mov ah, 5
mov al, 7
int 0x30
push ecx
jmp loop

clear_screen:
mov eax, 0x0
mov ah, 2
push ebx
push ecx
mov ebx, 0
mov ecx, 2
int 0x30
pop ecx
pop ebx
ret

hang:
jmp hang

str:
db "Welcome to ---GARBAGE CLICKER---",0x0a, "Pressing any key will increment your score by one", 0