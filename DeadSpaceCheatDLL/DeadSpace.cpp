#include "stdafx.h"
#include "DeadSpace.h"
#include "hackProcess.h"
#include "Offsets.h"

void __declspec(naked) ourFunc()
{
	__asm
	{
	newmem:
		cmp dword ptr[edi+0x12C],0
		je Entity

	Player:
		cmp DeadSpaceOffsets::dwGodmode,0
		je code
		push eax
		mov eax,[edi+0x12C]
		movd xmm0,eax
		pop eax
		jmp code

	Entity:
		cmp DeadSpaceOffsets::dwOneShotKill,0
		je code
		push eax
		mov eax,0
		movd xmm0,eax
		pop eax
		jmp code

	code:
		movss[edi+0x120],xmm0
		jmp[DeadSpaceOffsets::HeathDecremenntJumpBackAddr]
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
	DrawText(hdc, TEXT("Dead Space Trainer"), -1, &rect, DT_NOCLIP);
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
	DrawText(hdc, TEXT(" Cheat "), -1, &Cheat, DT_NOCLIP);

	SetRect(&Misc, 265, 190, 300, 210);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	DrawText(hdc, TEXT(" Misc "), -1, &Misc, DT_NOCLIP);


	DeleteObject(hFont);
	hackProcess::Hook((void *)(DeadSpaceOffsets::uiModuleBaseAddress + DeadSpaceOffsets::HealthDecrement), ourFunc, DeadSpaceOffsets::HealthDecSize);

	//ReleaseDC(hWnd, hdc);
	return VOID();
}

VOID DeadSpace::CreateWindowControls(HWND hWnd) noexcept
{

	//Create Credit Windows
	tbCredits = CreateWindow(TEXT("STATIC"), TEXT("Credits"), WS_CHILD |WS_VISIBLE|WS_BORDER|SS_CENTER,
		265, 255, 65, 20,
		hWnd, nullptr, GetModuleHandle(szDLLName), nullptr
	);
	ebCredits = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE|ES_RIGHT,
		335, 255, 100, 20,
		hWnd, nullptr, GetModuleHandle(szDLLName), nullptr
	);
	tbCredits = CreateWindow(TEXT("BUTTON"), TEXT("SET"), WS_CHILD | WS_VISIBLE | WS_BORDER| BS_CENTER,
		440, 255, 35, 20,
		hWnd, (HMENU)CREDIT_SET, GetModuleHandle(szDLLName), nullptr
	);

	//Create Node Windows
	tbNodes = CreateWindow(TEXT("STATIC"), TEXT("Nodes"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
		265, 220, 65, 20,
		hWnd, nullptr, GetModuleHandle(szDLLName), nullptr
	);
	ebNodes = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT,
		335, 220, 100, 20,
		hWnd, nullptr, GetModuleHandle(szDLLName), nullptr
	);
	tbNodes = CreateWindow(TEXT("BUTTON"), TEXT("SET"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CENTER,
		440, 220, 35, 20,
		hWnd, (HMENU)NODE_SET, GetModuleHandle(szDLLName), nullptr
	);

	//Create Checkboxes
	cbGodmode = CreateWindow(TEXT("BUTTON"), TEXT("Godmode"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX| BS_CENTER,
		75, 220, 120, 20,
		hWnd, (HMENU)GODMODE_SET, GetModuleHandle(szDLLName), nullptr);
	cbAmmo = CreateWindow(TEXT("BUTTON"), TEXT("Infinite Ammo"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 260, 120, 20,
		hWnd, (HMENU)AMMO_SET, GetModuleHandle(szDLLName), nullptr);
	cbOneShotKill = CreateWindow(TEXT("BUTTON"), TEXT("OneShotKill"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 300, 120, 20,
		hWnd, (HMENU)ONESHOT_SET, GetModuleHandle(szDLLName), nullptr);
	cbAir = CreateWindow(TEXT("BUTTON"), TEXT("Infinite Air"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 340, 120, 20,
		hWnd, (HMENU)AIR_SET, GetModuleHandle(szDLLName), nullptr);
	cbStatis = CreateWindow(TEXT("BUTTON"), TEXT("Infinite Stasis"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CHECKBOX | BS_CENTER,
		75, 380, 120, 20,
		hWnd, (HMENU)STASIS_SET, GetModuleHandle(szDLLName), nullptr);

	//Dynamic Addresses
	DeadSpaceOffsets::uiModuleBaseAddress = (uintptr_t)GetModuleHandle(DeadSpaceOffsets::szModuleName);

	return VOID();
}
VOID DeadSpace::RunWindow(HWND hWnd, WPARAM wParam, LPARAM lParam) noexcept
{
	UINT bChecked;
	TCHAR szBuff[16];
	switch (wParam)
	{
	case GODMODE_SET:
		bChecked = IsDlgButtonChecked(hWnd, GODMODE_SET);
		if (bChecked == BST_UNCHECKED)
		{
			CheckDlgButton(hWnd, GODMODE_SET, BST_CHECKED);
			DeadSpaceOffsets::dwGodmode = 1;

		}
		else if (bChecked == BST_CHECKED)
		{
			CheckDlgButton(hWnd, GODMODE_SET, BST_UNCHECKED);
			DeadSpaceOffsets::dwGodmode = 0;
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
			CheckDlgButton(hWnd, ONESHOT_SET, BST_CHECKED);
			DeadSpaceOffsets::dwOneShotKill = 1;
		}
		else if (bChecked == BST_CHECKED)
		{
			DeadSpaceOffsets::dwOneShotKill = 0;
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
		*(uintptr_t *)hackProcess::FindDynamicAddress(DeadSpaceOffsets::uiModuleBaseAddress, DeadSpaceOffsets::CreditsOffsets) = std::stoi(szBuff);
		break;

	case NODE_SET:
		GetWindowText(ebNodes, szBuff, 15);
		*(uintptr_t *)hackProcess::FindDynamicAddress(DeadSpaceOffsets::uiModuleBaseAddress, DeadSpaceOffsets::NodesOffsets) = std::stoi(szBuff);
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

 