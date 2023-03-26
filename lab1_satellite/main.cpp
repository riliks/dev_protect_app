#include <iostream>
#include <windows.h>

#pragma comment(lib, "advapi32.lib")

using namespace std;

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wchar_t cmdLine[] = L"C:\\projects\\build-lab1-Desktop_Qt_6_4_2_MSVC2019_64bit-Debug\\debug\\lab1.exe";

    /*cmdLine, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi*/
    if (CreateProcess(cmdLine, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
        cout << "Success!" << endl;
        cout << "pid = " << dec << pi.dwProcessId << endl;
    } else {
        DWORD lastError = GetLastError();
        cout << "Failed!" << endl;
        cout << lastError;
    }

//    HANDLE h_proc = pi.hProcess;
//    cout << "h_proc = " << h_proc << endl;

//    TOKEN_PRIVILEGES NewState;
//    NewState.PrivilegeCount = 1;
//    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &NewState.Privileges[0].Luid)) {
//        clog << "No debug-privilege name: " << GetLastError() << endl;
//        return EXIT_FAILURE;
//    }

//    HANDLE token;
//    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token)) {
//        clog << "No token: " << GetLastError() << endl;
//        return EXIT_FAILURE;
//    }

//    // TOKEN_PRIVILEGES NewState;
//    NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//    if (!AdjustTokenPrivileges(token, FALSE, &NewState, sizeof(NewState), nullptr, nullptr)) {
//        clog << "No privileges: " << GetLastError() << endl;
//        return EXIT_FAILURE;
//    }

    bool isAttached = DebugActiveProcess(pi.dwProcessId);
    if (isAttached) {
        DWORD lastError = GetLastError();
        cout << hex << "DebugActiveProcess() Failed!" << endl;
        cout << "GetLastError() = " << lastError << endl;
    } else {
        cout << "DebugActiveProcess() Success!" << endl;
    }

    DEBUG_EVENT debugEvent;
    while (true) {
        bool result1 = WaitForDebugEvent(&debugEvent, INFINITE);
        bool result2 = ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE);
    }
    return 0;
}
