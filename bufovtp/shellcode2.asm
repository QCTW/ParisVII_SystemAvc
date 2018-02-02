BITS 32 

global _start 

segment .text 

_start: 
	push byte 0x50
	xor [esp], byte 0x50
	push byte 0x72
	push word 0x6964
	push dword 0x74736574

	mov ebx, esp
	xor eax, eax 
	mov al, 0x28 ; syscall rmdir 
	int 0x80 
	xor eax, eax 
	mov al, 0x01 ; syscall exit 
	xor ebx, ebx 
	int 0x80 

