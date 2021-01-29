#include "stdafx.h"
#include "Window.h"
#include "DeadSpace.h"


DWORD CALLBACK DeadSpaceHackThread(HMODULE hModule)
{
	Window wWnd = Window(500, 500, L"Dead Space Trainer Internal");
	MSG Msg = { 0 };
	BOOL gResult;

	MessageBox(nullptr, L"Injected", L"Status", MB_OK);

	while ((gResult = GetMessage(&Msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);

		if (GetAsyncKeyState(VK_F1) & 0x0001)
		{
			SendMessage(wWnd.GetWindowHandle(), (UINT)WM_COMMAND, (WPARAM)GODMODE_SET, (LPARAM)0);
		}
		if (GetAsyncKeyState(VK_F2) & 0x0001)
		{
			SendMessage(wWnd.GetWindowHandle(), (UINT)WM_COMMAND, (WPARAM)AMMO_SET, (LPARAM)0);
		}
		if (GetAsyncKeyState(VK_F3) & 0x0001)
		{
			SendMessage(wWnd.GetWindowHandle(), (UINT)WM_COMMAND, (WPARAM)ONESHOT_SET, (LPARAM)0);
		}
		if (GetAsyncKeyState(VK_F4) & 0x0001)
		{
			SendMessage(wWnd.GetWindowHandle(), (UINT)WM_COMMAND, (WPARAM)AIR_SET, (LPARAM)0);
		}
		if (GetAsyncKeyState(VK_F5) & 0x0001)
		{
			SendMessage(wWnd.GetWindowHandle(), (UINT)WM_COMMAND, (WPARAM)STASIS_SET, (LPARAM)0);
		}

		Sleep(50);
	}

	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)DeadSpaceHackThread, hModule, 0, nullptr));
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

