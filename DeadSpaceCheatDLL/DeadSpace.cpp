#include "stdafx.h"
#include "DeadSpace.h"
#include "hackProcess.h"
#include "Offsets.h"

DWORD jmpBackAddy = 0x0045D1DD + 0x8;
DWORD dwGodmode = 0, dwOneShotKill = 0;

void __declspec(naked) ourFunc()
{
	__asm
	{
		//Compare Godmode and OneShotKill
		push eax
		push ebx

		mov eax,0x00400000
		add eax,0x00B4578C

		mov eax,[eax]
		add eax,0x278

		mov eax,[eax]
		add eax,0x38

		mov eax,[eax]
		add eax,0x3E0  //Static Health Address

		mov ebx,edi // Store Current Health Address In EBX
		add ebx,0x120

		cmp eax,ebx // Compare Current Health Address With Player Health
		je PLAYER
		jmp ENTITY


	PLAYER:
		mov eax,dwGodmode
		mov ebx, 1
		cmp eax, ebx
		je GODMODE
		movss [edi + 0x00000120],xmm0
		jmp EXIT
	GODMODE:
		mov dword ptr [edi+0x120],0x43480000
		jmp EXIT

	ENTITY:
		mov eax,dwOneShotKill
		mov ebx, 1
		cmp eax,ebx
		je ONESHOTKILL
		movss[edi + 0x00000120],xmm0
		jmp EXIT
	ONESHOTKILL:
		mov dword ptr[edi + 0x120],0x00000000
		jmp EXIT

	EXIT:
		pop ebx
		pop eax
		jmp [jmpBackAddy]
		

	}
}

VOID DeadSpace::CreateWindowGUI(HWND hWnd, HDC hdc) noexcept
{
	//Window Background 
	RECT rc;
	GetClientRect(hWnd, &rc);
	FillRect(hdc, &rc, (HBRUSH)CreateSolidBrush(RGB(211, 211, 211)));
	
	//Change Font
	HFONT hFont = CreateFont(36, 0, 0, 0,
		FW_DONTCARE,FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		VARIABLE_PITCH, TEXT("Arial"));
	SelectObject(hdc, hFont);

	//Text Coordinates
	RECT rect;
	SetRect(&rect, 106, 68, 390, 108);
	FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 0)));
	SetRect(&rect, 108, 70, 388, 106);
	FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
	SetTextColor(hdc, RGB(255, 0, 0));
	SetBkColor(hdc, RGB(255, 255, 255));
	DrawText(hdc, L"Dead Space Trainer", -1, &rect, DT_NOCLIP);
	DeleteObject(hFont);

	//Box for Cheats and Misc.
	RECT Cheat, Misc;
	SetRect(&Cheat, 18, 198, 242, 482);
	FillRect(hdc, &Cheat, CreateSolidBrush(RGB(0, 0, 0)));
	SetRect(&Cheat, 20, 200, 240, 480);
	FillRect(hdc, &Cheat, CreateSolidBrush(RGB(192, 192, 192)));


	SetRect(&Cheat, 258, 198, 482, 482);
	FillRect(hdc, &Cheat, CreateSolidBrush(RGB(0, 0, 0)));
	SetRect(&Misc, 260, 200, 480, 480);
	FillRect(hdc, &Misc, CreateSolidBrush(RGB(192, 192, 192)));

	//Change Font
	hFont = CreateFont(20, 0, 0, 0,
		FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		VARIABLE_PITCH, TEXT("Arial"));
	SelectObject(hdc, hFont);

	//Cheat and Box Text
	SetRect(&Cheat, 25, 190, 100, 210);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	DrawText(hdc, L" Cheat ", -1, &Cheat, DT_NOCLIP);

	SetRect(&Misc, 265, 190, 300, 210);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	DrawText(hdc, L" Misc ", -1, &Misc, DT_NOCLIP);


	DeleteObject(hFont);
	hackProcess::Hook((void *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::HealthDecrement), ourFunc, DeadSpaceOffsets::HealthDecSize);

	//ReleaseDC(hWnd, hdc);
	return VOID();
}

VOID DeadSpace::CreateWindowControls(HWND hWnd) noexcept
{

	//Create Credit Windows
	tbCredits = CreateWindow(L"STATIC", L"Credits", WS_CHILD |WS_VISIBLE|WS_BORDER|SS_CENTER,
		265, 255, 65, 20,
		hWnd, nullptr, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr
	);
	ebCredits = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE|ES_RIGHT,
		335, 255, 100, 20,
		hWnd, nullptr, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr
	);
	tbCredits = CreateWindow(L"BUTTON", L"SET", WS_CHILD | WS_VISIBLE | WS_BORDER| BS_CENTER,
		440, 255, 35, 20,
		hWnd, (HMENU)CREDIT_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr
	);

	//Create Node Windows
	tbNodes = CreateWindow(L"STATIC", L"Nodes", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
		265, 220, 65, 20,
		hWnd, nullptr, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr
	);
	ebNodes = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT,
		335, 220, 100, 20,
		hWnd, nullptr, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr
	);
	tbNodes = CreateWindow(L"BUTTON", L"SET", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CENTER,
		440, 220, 35, 20,
		hWnd, (HMENU)NODE_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr
	);

	//Create Checkboxes
	cbGodmode = CreateWindow(L"BUTTON", L"Godmode", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX| BS_CENTER,
		75, 220, 120, 20,
		hWnd, (HMENU)GODMODE_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr);
	cbAmmo = CreateWindow(L"BUTTON", L"Infinite Ammo", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 260, 120, 20,
		hWnd, (HMENU)AMMO_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr);
	cbOneShotKill = CreateWindow(L"BUTTON", L"OneShotKill", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 300, 120, 20,
		hWnd, (HMENU)ONESHOT_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr);
	cbAir = CreateWindow(L"BUTTON", L"Infinite Air", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 340, 120, 20,
		hWnd, (HMENU)AIR_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr);
	cbStatis = CreateWindow(L"BUTTON", L"Infinite Stasis", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 380, 120, 20,
		hWnd, (HMENU)STASIS_SET, GetModuleHandle(L"DeadSpaceCheatDLL.dll"), nullptr);

	//Dynamic Addresses
	DeadSpaceOffsets::uiModuleBaseAddress = (uintptr_t)GetModuleHandle(L"Dead Space.exe");;

	return VOID();
}
VOID DeadSpace::RunWindow(HWND hWnd, WPARAM wParam, LPARAM lParam) noexcept
{
	UINT bChecked;
	wchar_t szBuff[16];
	switch (wParam)
	{
	case GODMODE_SET:
		bChecked = IsDlgButtonChecked(hWnd, GODMODE_SET);
		if (bChecked == BST_UNCHECKED)
		{
			CheckDlgButton(hWnd, GODMODE_SET, BST_CHECKED);
			dwGodmode = 1;

		}
		else if (bChecked == BST_CHECKED)
		{
			CheckDlgButton(hWnd, GODMODE_SET, BST_UNCHECKED);
			dwGodmode = 0;
		}
		break;

	case AMMO_SET:
		bChecked = IsDlgButtonChecked(hWnd, AMMO_SET);
		if (bChecked == BST_UNCHECKED)
		{
			CheckDlgButton(hWnd, AMMO_SET, BST_CHECKED);
			hackProcess::Nop((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::AmmoDecrement), DeadSpaceOffsets::AmmoDecSize);

		}
		else if (bChecked == BST_CHECKED)
		{
			CheckDlgButton(hWnd, AMMO_SET, BST_UNCHECKED);
			hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::AmmoDecrement), DeadSpaceOffsets::AmmoDecOFF, DeadSpaceOffsets::AmmoDecSize);

		}
		break;

	case ONESHOT_SET:
		bChecked = IsDlgButtonChecked(hWnd, ONESHOT_SET);
		if (bChecked == BST_UNCHECKED)
		{
			SetWindowText(hWnd, L"OneShotKill Checked!");
			CheckDlgButton(hWnd, ONESHOT_SET, BST_CHECKED);
			dwOneShotKill = 1;
		}
		else if (bChecked == BST_CHECKED)
		{
			dwOneShotKill = 0;
			SetWindowText(hWnd, L"OneShotKill Unchecked!");
			CheckDlgButton(hWnd, ONESHOT_SET, BST_UNCHECKED);
		}
		break;

	case AIR_SET:
		bChecked = IsDlgButtonChecked(hWnd, AIR_SET);
		if (bChecked == BST_UNCHECKED)
		{
			CheckDlgButton(hWnd, AIR_SET, BST_CHECKED);
			hackProcess::Nop((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::AirDecrement), DeadSpaceOffsets::AirDecSize);
		}
		else if (bChecked == BST_CHECKED)
		{
			CheckDlgButton(hWnd, AIR_SET, BST_UNCHECKED);
			hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::AirDecrement), DeadSpaceOffsets::AirDecOFF, DeadSpaceOffsets::AirDecSize);
		}
		break;

	case STASIS_SET:
		bChecked = IsDlgButtonChecked(hWnd, STASIS_SET);
		if (bChecked == BST_UNCHECKED)
		{
			CheckDlgButton(hWnd, STASIS_SET, BST_CHECKED);
			hackProcess::Nop((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::StatisDecrement), DeadSpaceOffsets::StatisDecSize);
		}
		else if (bChecked == BST_CHECKED)
		{
			CheckDlgButton(hWnd, STASIS_SET, BST_UNCHECKED);
			hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::StatisDecrement), DeadSpaceOffsets::StatisDecOFF, DeadSpaceOffsets::StatisDecSize);
		}
		break;

	case CREDIT_SET:
		GetWindowText(ebCredits, szBuff, 15);
		*(uintptr_t *)hackProcess::FindDynamicAddress(DeadSpaceOffsets::uiModuleBaseAddress, DeadSpaceOffsets::CreditsOffsets) = _wtoi(szBuff);
		break;

	case NODE_SET:
		GetWindowText(ebNodes, szBuff, 15);
		*(uintptr_t *)hackProcess::FindDynamicAddress(DeadSpaceOffsets::uiModuleBaseAddress, DeadSpaceOffsets::NodesOffsets) = _wtoi(szBuff);
		break;
	}

	return VOID();
}

VOID DeadSpace::CloseWindowGUI(HWND hWnd) noexcept
{
	hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::HealthDecrement), DeadSpaceOffsets::HealthDecOFF, DeadSpaceOffsets::HealthDecSize);
	hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::AmmoDecrement), DeadSpaceOffsets::AmmoDecOFF, DeadSpaceOffsets::AmmoDecSize);
	hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::AirDecrement), DeadSpaceOffsets::AirDecOFF, DeadSpaceOffsets::AirDecSize);
	hackProcess::Patch((BYTE *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::StatisDecrement), DeadSpaceOffsets::StatisDecOFF, DeadSpaceOffsets::StatisDecSize);
	return VOID();
}

 