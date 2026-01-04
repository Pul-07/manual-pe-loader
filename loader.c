#include <windows.h>
#include <winternl.h>

// --- 1. PEB PORTABLE (Via structures internes) ---
void FixPEB(PVOID newBase) {
    PROCESS_BASIC_INFORMATION pbi;
    ULONG len;
    // Utilisation de NtQueryInformationProcess (plus sûr que les offsets Reserved)
    typedef NTSTATUS (NTAPI *pNtQuery)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
    pNtQuery ntQ = (pNtQuery)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
    
    if (ntQ(GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof(pbi), &len) == 0) {
        pbi.PebBaseAddress->ImageBaseAddress = newBase;
    }
}

// --- 2. PROTECTION FINALE DES SECTIONS ---
void FinalizeMemoryProtections(PVOID localImage, PIMAGE_NT_HEADERS nt) {
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        DWORD oldProt;
        DWORD newProt = 0;
        
        // Déduction de la protection Windows à partir des caractéristiques du PE
        BOOL r = (section[i].Characteristics & IMAGE_SCN_MEM_READ) != 0;
        BOOL w = (section[i].Characteristics & IMAGE_SCN_MEM_WRITE) != 0;
        BOOL x = (section[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;

        if (x && w && r) newProt = PAGE_EXECUTE_READWRITE;
        else if (x && r) newProt = PAGE_EXECUTE_READ;
        else if (w && r) newProt = PAGE_READWRITE;
        else if (r)      newProt = PAGE_READONLY;
        else             newProt = PAGE_NOACCESS;

        VirtualProtect((PVOID)((ULONG_PTR)localImage + section[i].VirtualAddress), 
                       section[i].SizeOfRawData, newProt, &oldProt);
    }
}

// --- 3. LOGIQUE D'ENTRYPOINT (EXE vs DLL) ---
void ExecuteEntry(PVOID localImage, PIMAGE_NT_HEADERS nt) {
    ULONG_PTR oep = (ULONG_PTR)localImage + nt->OptionalHeader.AddressOfEntryPoint;

    if (nt->FileHeader.Characteristics & IMAGE_FILE_DLL) {
        // C'est une DLL
        typedef BOOL(WINAPI *DllMain_t)(HINSTANCE, DWORD, LPVOID);
        DllMain_t pDllMain = (DllMain_t)oep;
        pDllMain((HINSTANCE)localImage, DLL_PROCESS_ATTACH, NULL);
    } else {
        // C'est un EXE
        typedef void(WINAPI *ExeMain_t)();
        ExeMain_t pExeMain = (ExeMain_t)oep;
        pExeMain();
    }
}
