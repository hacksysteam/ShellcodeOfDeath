/* 
                  _     __             _____                     
  /\  /\__ _  ___| | __/ _\_   _ ___  /__   \___  __ _ _ __ ___  
 / /_/ / _` |/ __| |/ /\ \| | | / __|   / /\/ _ \/ _` | '_ ` _ \ 
/ __  / (_| | (__|   < _\ \ |_| \__ \  / / |  __/ (_| | | | | | |
\/ /_/ \__,_|\___|_|\_\\__/\__, |___/  \/   \___|\__,_|_| |_| |_|
                           |___/                                 

                                http://hacksys.vfreaks.com/
                                  hacksysteam@hotmail.com

Module Name:

    Dump Shellcode In C Style

Abstract:
    
    A simple program that will generate shellcode from source file. 
    Using pure assembly is prefered.
    the binary path of gcc and objcopy must add to the windows environment.
    This code is only work for windows platform not linux. ;)
    by MicroMike
    
    Fixed by Ashfaq Ansari
    
IDE: 
     
    Dev-C++ 4.9.9.2 (Windows XP SP3)
        
Compiler: 
          
    gcc 3.4.2

*/

#include <stdio.h>
#include <stdlib.h>

void create_obj_file(const char *filename){
	char command[100];
	sprintf(command, "gcc -c -o temp.o %s", filename);
	system(command);
	/* strip the trash section such as debug */
	//system("objcopy --strip-debug --strip-unneeded temp.o");
	system("objcopy --strip-all temp.o");
}

void dump_shellcode(FILE *optr){
	unsigned int loop, counter=1, ch=1, nop_counter=0, null_counter=0;
	unsigned char header[20];
	unsigned short *SectionNum;
	int buffer;
	unsigned char str_to_write[50];
	FILE *iptr;
	iptr = fopen("temp.o", "rb");
	if(iptr == NULL){
		printf("Can not open file, is the file name correct??\n");
		exit(1);
	}
	/* reading header */
	for(loop=0; loop<20; loop++){
		header[loop] = fgetc(iptr);
	}
	/* This is the field I care for ~~ */
	/* this value should only be one, only has .text section */
	SectionNum = (unsigned short*)&(header[2]);
	
	/* each section header has 40 bytes*/
	fseek(iptr, 40*(*SectionNum), SEEK_CUR);
	
	/* the following is just some file operations */
	fputs("unsigned char *shellcode=\n\"", optr);
	
	printf("\nExtracting the shellcode:\n\n");
	
	printf("unsigned char *shellcode=\n\"");
	
    while(ch != feof(iptr)){

             memset(str_to_write, 0, 50);
             buffer = fgetc(iptr);
                  
             /* GCC issue on Windows: 
                    Why does GCC pad functions with NOPs?
                    http://stackoverflow.com/questions/7912464/why-does-gcc-pad-functions-with-nops    
              */
              if(buffer == 0x90){
                        nop_counter++;
                        }
              if(nop_counter > 1){
                             break;
                             }          
              /* Null byte counter */               
              if(buffer == 0x00){
                        null_counter++;
                        }              
              sprintf(str_to_write, "\\x%02x", buffer);
              printf("%s", str_to_write);
              fputs(str_to_write, optr);
              if(counter++ > 10){
                           printf("\"\n\"");
                           fputs("\"\n\"",optr);
                           counter = 1;
                           }
              ch++;
              }

	printf("\"\n");
	fputs("\"\n", optr);
	printf("\nLength of Shellcode: %d", ch-1);
	printf("\nNumber of NULL's: %d\n", null_counter);
	fclose(iptr);
}

int main(int argc, char **argv){
	FILE *optr;
	if(argc < 2){
		printf("\nUsage: DumpShellcode.exe <Source assembly file> <Output dump file>\n");
		exit(1);
	}	
	create_obj_file(argv[1]);
	
	optr = fopen(argv[2], "w");	
	if(optr == NULL){
		printf("Can not create new file: %s", argv[2]);
		exit(1);
	}
	dump_shellcode(optr);
	fclose(optr);
	system("del temp.o");
	
}
