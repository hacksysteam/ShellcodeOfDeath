#include <stdio.h>
#include <stdlib.h>

/* 
                  _     __             _____                     
  /\  /\__ _  ___| | __/ _\_   _ ___  /__   \___  __ _ _ __ ___  
 / /_/ / _` |/ __| |/ /\ \| | | / __|   / /\/ _ \/ _` | '_ ` _ \ 
/ __  / (_| | (__|   < _\ \ |_| \__ \  / / |  __/ (_| | | | | | |
\/ /_/ \__,_|\___|_|\_\\__/\__, |___/  \/   \___|\__,_|_| |_| |_|
                           |___/                                 

                                http://hacksys.vfreaks.com/
                                  hacksysteam@hotmail.com

Thanks to:
       
    Ruei-Min Jiang (@mike820324) a.k.a MicroMike
    respect to you bro.

Module Name:

    Hash Calculator

Abstract:
    
    This function calculates the Function hash according
    to its name.

IDE: 
     
    Dev-C++ 4.9.9.2 (Windows XP SP3)
        
Compiler: 
          
    gcc 3.4.2

*/

unsigned int hash_function(const char* string){
         unsigned int value = 0;
         __asm__("xor %%ebx, %%ebx;\n\t\
                  loop:\n\t\
                  xor %%eax, %%eax;\n\t\
                  lodsb;\n\t\
                  rol $5, %%ebx;\n\t\
                  addl %%eax, %%ebx;\n\t\
                  cmp $0, %%eax;\n\t\
                  jnz loop;\n\t\
                  ror $5, %%ebx;\n\t\
                  movl %%ebx, %0;\n\t"
                  :"=m" (value)
                  :"S" (string)
                  :
                  );
         printf("%s: 0x%x\n",string,value);
}

int main(){  
    hash_function("LoadLibraryA");
    hash_function("CloseHandle");
    hash_function("CreateFileA");
    hash_function("ExitProcess"); 
    hash_function("FormatEx");
    hash_function("DeviceIoControl");
    hash_function("Sleep");
    system("pause");	
}
