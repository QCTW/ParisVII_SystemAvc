BITS 32 

global _start 

segment .text 

_start: 
	jmp short donnees 
rmdir: 	pop ebx 
	xor eax, eax 
	mov al, 0x28 ; syscall rmdir 
	int 0x80 
	xor eax, eax 
	mov al, 0x01 ; syscall exit 
	xor ebx, ebx 
	int 0x80 
donnees: call rmdir 
	 nom db "testdir"
