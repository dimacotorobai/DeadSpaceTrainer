#include "stdafx.h"
#include "hackProcess.h"


DWORD hackProcess::GetProcessId(const TCHAR * szProcessName)
{
	DWORD dwProcessId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 ProcessEntry;
		ProcessEntry.dwSize = sizeof(ProcessEntry);

		if (Process32First(hSnapshot, &ProcessEntry))
		{
			do
			{
				if (!lstrcmp(szProcessName, ProcessEntry.szExeFile))
				{
					dwProcessId = ProcessEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &ProcessEntry));
		}

	}

	CloseHandle(hSnapshot);
	return dwProcessId;
}

uintptr_t hackProcess::GetModuleBaseAddress(const TCHAR * szModuleName, DWORD dwProcessId)
{
	uintptr_t uiModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry;
		ModuleEntry.dwSize = sizeof(ModuleEntry);

		if (Module32First(hSnapshot, &ModuleEntry))
		{
			do
			{
				if (!lstrcmp(szModuleName, ModuleEntry.szModule))
				{
					uiModuleBaseAddress = (uintptr_t)ModuleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry));
		}

	}

	CloseHandle(hSnapshot);
	return uiModuleBaseAddress;
}

void hackProcess::PatchEx(HANDLE hProcess, BYTE * dst, BYTE * src, unsigned int size)
{
	DWORD dwOldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	WriteProcessMemory(hProcess, dst, src, size, 0);
	VirtualProtectEx(hProcess, dst, size, dwOldProtect, &dwOldProtect);
}

void hackProcess::NopEx(HANDLE hProcess, BYTE * dst, unsigned int size)
{
	BYTE *src = new BYTE[size];
	memset(src, 0x90, size);

	PatchEx(hProcess, dst, src, size);
	delete[] src;
}

uintptr_t hackProcess::FindDynamicAddressEx(HANDLE hProcess, uintptr_t uiBaseAddress, std::vector<unsigned int> Offset)
{
	uintptr_t uiDynamicAddress = uiBaseAddress + Offset[0];

	for (int i = 1; i < Offset.size(); i++)
	{
		ReadProcessMemory(hProcess, (LPCVOID)uiDynamicAddress, &uiDynamicAddress, sizeof(uiDynamicAddress), 0);
		uiDynamicAddress += Offset[i];
	}

	return uiDynamicAddress;
}

void hackProcess::Patch(BYTE * dst, BYTE * src, unsigned int size)
{
	DWORD dwOldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, dwOldProtect, &dwOldProtect);
}

void hackProcess::Nop(BYTE * dst, unsigned int size)
{
	DWORD dwOldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, dwOldProtect, &dwOldProtect);
}

uintptr_t hackProcess::FindDynamicAddress(uintptr_t uiBaseAddress, std::vector<unsigned int> Offset)
{
	uintptr_t uiDynamicAddress = uiBaseAddress + Offset[0];

	for (int i = 1; i < Offset.size(); i++)
	{
		uiDynamicAddress = *(uintptr_t *)uiDynamicAddress;
		uiDynamicAddress += Offset[i];
	}

	return uiDynamicAddress;
}

bool hackProcess::Hook(void *toHook, void *ourFunc, int len)
{
	if (len < 5)
		return false;

	DWORD ourProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &ourProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunc - (DWORD)toHook - 5);

	*(BYTE *)toHook = 0xE9;
	*(DWORD *)((DWORD)toHook + 1) = relativeAddress;


	VirtualProtect(toHook, len, ourProtection, &ourProtection);
	return true;
}
