#pragma once

#define CREDIT_SET	100
#define NODE_SET	101

#define GODMODE_SET	102
#define AMMO_SET	103
#define ONESHOT_SET 104
#define AIR_SET		105
#define STASIS_SET	106

class DeadSpace
{
public:
	HWND tbCredits, tbNodes;
	HWND ebCredits, ebNodes;
	HWND bbCredits, bbNodes;

	//Checkboxes
	HWND cbGodmode, cbOneShotKill, cbAmmo, cbStatis, cbAir;

	//Process Name
	const TCHAR* szDLLName = TEXT("DeadSpaceCheatDLL.dll");

public:
	static VOID CreateWindowGUI(HWND hWnd, HDC hdc) noexcept;
	VOID CreateWindowControls(HWND hWnd) noexcept;
	VOID RunWindow(HWND hWnd, WPARAM wParam, LPARAM lParam) noexcept;
	static VOID CloseWindowGUI(HWND hWnd) noexcept;
};
