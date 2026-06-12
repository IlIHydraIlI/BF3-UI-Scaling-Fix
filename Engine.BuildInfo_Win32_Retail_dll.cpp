#include <windows.h>
#include <stdio.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <string.h>


HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR mProcs[1] = {0};

static DWORD CALLBACK LoadExtraDllWorker(PVOID lpParameter) {
	Sleep(15000);
	char path[MAX_PATH] = {0};
	if (GetModuleFileNameA(mHinst, path, MAX_PATH)) {
		char* lastSlash = strrchr(path, '\\');
		if (lastSlash) {
			*(lastSlash + 1) = '\0';
			strcat_s(path, MAX_PATH, "gm_fix.dll");
			LoadLibraryA(path);
		}
	}
	return 0;
}

//def
void InjectUIScaleLimit(void);

LPCSTR mImportNames[] = {"getBuildInfo"};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		mHinstDLL = LoadLibrary( "ori_Engine.BuildInfo_Win32_Retail_dll.dll" );
		if ( !mHinstDLL )
			return ( FALSE );
		for ( int i = 0; i < 1; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );

		// Apply UI patch first, then load the extra DLL afterward
		InjectUIScaleLimit();
		QueueUserWorkItem(LoadExtraDllWorker, NULL, WT_EXECUTEDEFAULT);
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}

	return ( TRUE );
}

extern "C" __declspec(naked) void __stdcall getBuildInfo_wrapper(){__asm{jmp mProcs[0*4]}}

void InjectUIScaleLimit(void) {
	int value = 0xFF ;
	//UI Detect the address of the resolution
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x01766A9A, (LPVOID)&value, 1, NULL);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x01766AA2, (LPVOID)&value, 1, NULL);
	//the key prompts the address of the detection resolution
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0094FC63, (LPVOID)&value, 1, NULL);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0094FC8B, (LPVOID)&value, 1, NULL);
	//minimap Scale
	value = 0x8E;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x00936733, (LPVOID)&value, 1, NULL);
	//fixes UI flickering
	short shortValue = 0x0008;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0177060B, (LPVOID)&shortValue, 2, NULL);
	//killfeed scaling fix
	value = 0xEB;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0092D6D6, (LPVOID)&value, 1, NULL);

	//fix flag capture progress
	DWORD oldProtect;
	VirtualProtect(reinterpret_cast<void*>(0x00948263), sizeof(double*), PAGE_EXECUTE_READWRITE, &oldProtect);
	double* pointerValue = reinterpret_cast<double*>(0x020712F8);
	*reinterpret_cast<double**>(0x00948263) = pointerValue;

	//function to scale nametags and 3d icons
	unsigned char jump_nametag_function [] = {0xE9, 0x6B, 0xD0, 0x00, 0x01, 0x90};
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x00952650, (LPVOID)&jump_nametag_function, 6, NULL);

	unsigned char nametag_function[] = {
		0xF3, 0x0F, 0x10, 0x8F, 0x84, 0x03, 0x00, 0x00, //movss xmm1, dword ptr ds:[edi+0x384]
		0xF3, 0x0F, 0x10, 0x15, 0x00, 0xBE, 0x15, 0x02, //movss xmm2, dword ptr ds:[0x0215BE00]
		0xF3, 0x0F, 0x5E, 0xCA, //divss xmm1, xmm2
		0xF3, 0x0F, 0x59, 0xC1, //mulss xmm0, xmm1
		0xF3, 0x0F, 0x11, 0x44, 0x24, 0x24, //moves dword ptr ss:[esp+0x24], xmm0
		0xE9, 0x73, 0x2F, 0xFF, 0xFE //jmp 0x00952656
	};
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0195F6C0, (LPVOID)&nametag_function, 35, NULL);

	//function to scale the Commo Rose down. Check the Commo Rose object magic number 0x66673E529569DEB44E99D107C9639154. If true, calculate a relative scale to the 1280x720 resolution.
    unsigned char jump_commorose_function [] = {0xE8, 0xCC, 0xF5, 0x66, 0x00};
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x012F011F, (LPVOID)&jump_commorose_function, 5, NULL);

    unsigned char commorose_function[] = {
        0x85, 0xDB, //test ebx, ebx
        0x0F, 0x84, 0x7F, 0x00, 0x00, 0x00, //jz 0x0195F777
        0xB8, 0x54, 0x91, 0x63, 0xC9, //mov eax,0xC9639154
        0x3B, 0x03, //cmp eax,dword ptr ds:[ebx]
        0x75, 0x76, //jne 0x195F777
        0xB8, 0x07, 0xD1, 0x99, 0x4E, //mov eax,0x4E99D107
        0x3B, 0x43, 0x04, //cmp eax,dword ptr ds:[ebx+0x4]
        0x75, 0x6C, //jne 0x195F777
        0xB8, 0xB4, 0xDE, 0x69, 0x95, //mov eax,0x9569DEB4
        0x3B, 0x43, 0x08, //cmp eax,dword ptr ds:[ebx+0x8]
        0x75, 0x62, //jne 0x195F777


        0xB8, 0x52, 0x3E, 0x67, 0x66, //mov eax,0x66673E52
        0x3B, 0x43, 0x0C, //cmp eax,dword ptr ds:[ebx+0xC]
        0x75, 0x58, //jne 0x195F777
        0x0F, 0x57, 0xC0, //xorps xmm0,xmm0
        0x0F, 0x57, 0xC9, //xorps xmm1,xmm1
        0x8B, 0x1D, 0xD4, 0x77, 0x35, 0x02, //mov ebx,dword ptr ds:[0x23577D4]
        0x0F, 0xB7, 0x83, 0xA4, 0x01, 0x00, 0x00, //movzx eax,word ptr ds:[ebx+0x1A4]
        0xF2, 0x0F, 0x2A, 0xC8, //cvtsi2sd xmm1,eax
        0xB8, 0x00, 0x05, 0x00, 0x00, //mov eax,0x500
        0xF2, 0x0F, 0x2A, 0xC0, //cvtsi2sd xmm0,eax
        0xF2, 0x0F, 0x5E, 0xC1, //divsd xmm0,xmm1
        0xF2, 0x0F, 0x59, 0x05, 0x88, 0x08, 0x07, 0x02, //mulsd xmm0,qword ptr ds:[0x2070888]
        0xF2, 0x0F, 0x11, 0x44, 0x24, 0x1C, //movsd qword ptr ss:[esp+0x1C],xmm0
        0x0F, 0xB7, 0x83, 0xA6, 0x01, 0x00, 0x00, //movzx eax,word ptr ds:[ebx+0x1A6]
        0xF2, 0x0F, 0x2A, 0xC8, //cvtsi2sd xmm1,eax
        0xB8, 0xD0, 0x02, 0x00, 0x00, //mov eax,0x2D0
        0xF2, 0x0F, 0x2A, 0xC0, //cvtsi2sd xmm0,eax
        0xF2, 0x0F, 0x5E, 0xC1, //divsd xmm0,xmm1


        0xF2, 0x0F, 0x59, 0x05, 0x88, 0x08, 0x07, 0x02, //mulsd xmm0,qword ptr ds:[0x2070888]
        0xF2, 0x0F, 0x11, 0x44, 0x24, 0x24, //movsd qword ptr ss:[esp+0x24],xmm0
        0x8B, 0x5D, 0x0C, //mov ebx,dword ptr ss:[ebp+0xC]
        0xFF, 0xE2 //jmp edx
    };
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0195F6F0, (LPVOID)&commorose_function, 140, NULL);

	//fix the side effects introduced by the nametag scaling on >720p resolutions. 1. Removes the gap between nametags and soldier icons. 2. Disables the horizontal scaling of the health bar.
	int nop = 0x90909090;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x009187A2, (LPVOID)&nop, 4, NULL);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0077B86D, (LPVOID)&nop, 4, NULL);
}