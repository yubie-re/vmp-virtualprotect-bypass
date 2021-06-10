#pragma once
#include <Windows.h>

void vmp_virtualprotect_check_disable()
{
    DWORD old_protect = 0;
    auto ntdll = GetModuleHandleA("ntdll.dll");
#if _WIN64
    BYTE callcode = ((BYTE *)GetProcAddress(ntdll, "NtQuerySection"))[4] - 1; // Since the syscall code is partially corrupted, we have to figure out what it is (always 1 less than NtQuerySection) since it changes based on windows version.
    BYTE restore[] = {0x4C, 0x8B, 0xD1, 0xB8, callcode}; // x64 ntdll
#else
    BYTE callcode = ((BYTE *)GetProcAddress(ntdll, "NtQuerySection"))[1] - 1;
    BYTE restore[] = {0xB8, callcode, 0x00, 0x00, 0x00}; // x86 ntdll
#endif
    auto nt_vp = (BYTE *)GetProcAddress(ntdll, "NtProtectVirtualMemory");
    VirtualProtect(nt_vp, sizeof(restore), PAGE_EXECUTE_READWRITE, &old_protect); // They don't even check if we are vping vp 👎😹👎
    memcpy(nt_vp, restore, sizeof(restore));
    VirtualProtect(nt_vp, sizeof(restore), old_protect, &old_protect);
}