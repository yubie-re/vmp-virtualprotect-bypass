#include "vp-patch.hpp"

DWORD WINAPI main_thread(PVOID module)
{
    vmp_virtualprotect_check_disable();
    FreeLibraryAndExitThread((HMODULE)module, 0);
    return 1;
}

// Entrypoint
BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        CreateThread(nullptr, 0, &main_thread, (void *)module, 0, nullptr);
    }
    return TRUE;
}