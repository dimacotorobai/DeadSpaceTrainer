#pragma once
class hackProcess
{
public:
	//External Functions
	static DWORD GetProcessId(const TCHAR *szProcessName);
	static uintptr_t GetModuleBaseAddress(const TCHAR *szModuleName, DWORD dwProcessId);

	static void PatchEx(HANDLE hProcess, BYTE *dst, BYTE *src, unsigned int size);
	static void NopEx(HANDLE hProcess, BYTE *dst, unsigned int size);
	static uintptr_t FindDynamicAddressEx(HANDLE hProcess, uintptr_t uiBaseAddress, std::vector<unsigned int>Offset);

	//Internal Functions
	static void Patch(BYTE *dst, BYTE *src, unsigned int size);
	static void Nop(BYTE *dst, unsigned int size);
	static uintptr_t FindDynamicAddress(uintptr_t uiBaseAddress, std::vector<unsigned int> Offset);
	static bool Hook(void *toHook, void *ourFunc, int len);
};

