#pragma once
namespace DeadSpaceOffsets
{
	//Read/Write Handle to the Game
	HANDLE hProcess = 0;


	//Process Name of Game and Specific Module Name 
	const wchar_t *szProcessName = L"Dead Space.exe";
	const wchar_t *szModuleName = L"Dead Space.exe";


	//Store ProcessID and ModuleBaseAddress in here
	DWORD dwProcID;
	uintptr_t uiModuleBaseAddress;

	//Strore Dynamic Addresses in Here
	uintptr_t DynamicNodes, DynamicCredits;
	uintptr_t DynamicHealth;


	//First Offset is Offset Relative to ModuleBaseAddress
	std::vector<unsigned int> NodesOffsets = { 0x00B4578C, 0x278, 0x4, 0x0, 0x594 };
	std::vector<unsigned int> CreditsOffsets = { 0x00B4578C , 0x278, 0x38, 0x24, 0xA8, 0x370, 0x5C };
	std::vector<unsigned int> HealthOffsets = { 0x00B4578C, 0x278, 0x38, 0x3E0 };

	//Read/Write Memory using these  addresses
	int uiCreditsValue, uiNodesValue;

	//Decrement Ammo
	uintptr_t AmmoDecrement = 0x000F872E;
	BYTE AmmoDecOFF[] = { 0x89, 0xBE, 0x84, 0x06, 0x00, 0x00 };
	BYTE AmmoDecON[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	unsigned int AmmoDecSize = 6;


	//Health Decrement
	uintptr_t HealthDecrement = 0x0005D1DD;
	BYTE HealthDecOFF[] = { 0xF3, 0x0F, 0x11, 0x87, 0x20, 0x01, 0x00, 0x00 };
	BYTE HealthDecON[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	unsigned int HealthDecSize = 8;


	//Air Decrememnt
	uintptr_t AirDecrement = 0x0013481A;
	BYTE AirDecOFF[] = { 0xF3, 0x0F, 0x11, 0x46, 0x70 };
	BYTE AirDecON[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	unsigned int AirDecSize = 5;


	//Statis Decrement
	uintptr_t StatisDecrement = 0x0012735F;
	BYTE StatisDecOFF[] = { 0xF3, 0x0F, 0x11, 0x86, 0x28, 0x01, 0x00, 0x00 };
	BYTE StatisDecON[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	unsigned int StatisDecSize = 8;

}