//CatchKey.cpp
#define _WIN32_WINNT  0x0500				// ����ϵͳ�汾, ȷ������ʹ�õײ���̹���
#define WM_MY_SHORTS (WM_USER + 105)
#include "windows.h"

// ȫ�ֱ���
LPDWORD		g_lpdwVirtualKey = NULL;		// Keycode �����ָ��
int			g_nLength = 0;					// Keycode ����Ĵ�С
HINSTANCE	g_hInstance = NULL;				// ģ��ʵ�����
HHOOK		g_hHook = NULL;					// ���Ӿ��
HWND		g_hWnd  = NULL;
// DLL ��ں���
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	// ����ģ��ʵ�����
	g_hInstance = (HINSTANCE)hModule;
	
	// �ڽ��̽������߳̽���ʱж�ع���
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL,"DLL_PROCESS_ATTACH",":)",MB_ICONINFORMATION);
		break;
	case DLL_THREAD_ATTACH:
		MessageBoxA(NULL,"DLL_THREAD_ATTACH",":)",MB_ICONINFORMATION);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_DETACH:
		delete g_lpdwVirtualKey;
		if (g_hHook != NULL) UnhookWindowsHookEx(g_hHook);
		break;
	}
    return TRUE;
}

/// �ײ���̹��Ӻ���
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION && wParam == WM_KEYUP)
	{
		BOOL bctrl   =   GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT)*8)-1); 
		KBDLLHOOKSTRUCT* pStruct = (KBDLLHOOKSTRUCT*)lParam;
		LPDWORD tmpVirtualKey = g_lpdwVirtualKey;
		if (pStruct->vkCode == 80 && bctrl)
			MessageBoxA(NULL,"WTF",":)",MB_ICONINFORMATION);
			PostMessage(g_hWnd,WM_MY_SHORTS,0,1);
			return TRUE;	
	}
	
	// ����ϵͳ�е���һ������
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL WINAPI StartCatch(LPDWORD lpdwVirtualKey, int nLength,  HWND  pWnd)
{
	g_hWnd = pWnd;
	// ����Ѿ���װ���̹����򷵻� FALSE
	if (g_hHook != NULL) return FALSE;
	// ���û������� keycode ���鱣����ȫ�ֱ�����
	g_lpdwVirtualKey = (LPDWORD)malloc(sizeof(DWORD) * nLength);
	LPDWORD tmpVirtualKey = g_lpdwVirtualKey;
	for (int i = 0; i < nLength; i++)
	{
		*tmpVirtualKey++ = *lpdwVirtualKey++;
	}
	g_nLength = nLength;
	
	// ��װ�ײ���̹���
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstance, NULL);
	if (g_hHook == NULL) return FALSE;
	return TRUE;
	
}


BOOL WINAPI StopCatch()
{
	// ж�ع���
	if (UnhookWindowsHookEx(g_hHook) == 0) return FALSE;
	g_hHook = NULL;
	return TRUE;
}