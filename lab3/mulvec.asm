segment code
..start:

mov ax,data
mov ds,ax
mov ax,stack
mov ss,ax
mov sp,stacktop
xor si,si
xor di,di
mov cl,5

volta:
    mov ax,word[first_vector + si]
    mov bx,word[second_vector + si]
    mul bx
    mov word[third_vector + di],ax
    mov word[third_vector + di + 2],dx
    add si,2
    add di,4

int 3

mov ah,4ch
int 21h

segment data
first_vector dw 1111,2222,3333,4444,5555
second_vector dw 2,2,2,2,2
third_vector dw 0,0,0,0,0

segment stack stack
    resb 256
stacktop:


