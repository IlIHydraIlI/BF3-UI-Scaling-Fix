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

	//fix the side effects introduced by the nametag scaling on >720p resolutions. 1. Removes the gap between nametags and soldier icons. 2. Disables the horizontal scaling of the health bar.
	int nop = 0x90909090;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x009187A2, (LPVOID)&nop, 4, NULL);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x0077B86D, (LPVOID)&nop, 4, NULL);
}