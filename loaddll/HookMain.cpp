#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define DEF_DLL_NAME "keyhook.dll"
#define DEF_HOOKSTART "StartCatch"
#define DEF_HOOKSTOP "StopCatch"

typedef void(*PFN_HOOKSTAR)(LPWORD lpdwVirtualKey, int nLength,  HWND  pWnd);
typedef void(*PFN_HOOKSTOP)();


void main()
{ 
	DWORD dwVK[] = {'A', 'B', 'C', VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LWIN, VK_RWIN};
    int nLength = sizeof(dwVK) / sizeof(DWORD);

	HMODULE hDll = NULL;
	PFN_HOOKSTAR HookStart = NULL;
	PFN_HOOKSTOP HookStop = NULL;
	HWND hwnd;

	char ch=0;

	//����KeyHook.dll

	hDll = LoadLibraryA(DEF_DLL_NAME);

	//��ȡ����������ַ

	HookStart=(PFN_HOOKSTAR)GetProcAddress(hDll,DEF_HOOKSTART);
	HookStop =(PFN_HOOKSTOP)GetProcAddress(hDll,DEF_HOOKSTOP);

	//��ʼ��ȡ
	HookStart((LPWORD)dwVK,nLength,this->m_hWnd);

	//�ȴ�ֱ���û����룺"q"

	printf("press 'q' to quit!\n");
	while( _getch() !=  'q');

	//��ֹ��ȡ
	HookStop();

	//ж��KeyHook.dll
	FreeLibrary(hDll);

}