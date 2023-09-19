segment code
..start:
; iniciar os registros de segmento DS e SS e o ponteiro de pilha SP
mov ax,data
mov ds,ax
mov ax,stack
mov ss,ax
mov sp,stacktop
mov bx,three_chars
mov ah,1
int 21h ; função do dos de entrada de carcater. Retorna em AL
dec al
mov [bx],al
inc bx
int 21h
dec al
mov [bx],al
inc bx
int 21h
dec al
mov [bx],al
mov dx,display_string
mov ah,9
int 21h
; Terminar o programa e voltar para o sistema operacional
mov ah,4ch
int 21h
segment data
CR equ 0dh
LF equ 0ah
display_string db CR,LF
three_chars resb 3
db '$'
segment stack stack
 resb 256
stacktop: