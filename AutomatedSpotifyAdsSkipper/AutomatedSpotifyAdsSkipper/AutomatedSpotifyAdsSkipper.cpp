#include <iostream>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <winuser.h>


using namespace std;


vector<DWORD> getProc() {
    vector<DWORD> pids;
    // process name
    wstring targetProcessName = L"Spotify.exe";

    // all processes
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //current process
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof entry;

    // if (!Process32FirstW(snap, &entry)) return false;

    do {
        if (wstring(entry.szExeFile) == targetProcessName) {
            pids.push_back(entry.th32ProcessID);
        }
    } while (Process32NextW(snap, &entry)); //keep going until end of snapshot 

    return pids;
}


void SendCommand(DWORD dwProcessID, int switch_arg) {
    HWND hwnd = NULL;
    do {
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
        DWORD dwPID = 0;
        GetWindowThreadProcessId(hwnd, &dwPID);
        if (dwPID == dwProcessID) {
            // cout << "[DESC]: " << hwnd << " [ID]: " << dwPID << '\n';
            HWND EditClass = FindWindowEx(hwnd, NULL, L"Chrome_RenderWidgetHostHWND", NULL);
            switch (switch_arg)
            {
                case 1:
                    PostMessage(EditClass, WM_APPCOMMAND, 0, MAKELPARAM(0, APPCOMMAND_MEDIA_PREVIOUSTRACK));
                    break;
                case 2:
                    PostMessage(EditClass, WM_APPCOMMAND, 0, MAKELPARAM(0, APPCOMMAND_MEDIA_PLAY_PAUSE));
                    break;
                case 3:
                    PostMessage(EditClass, WM_APPCOMMAND, 0, MAKELPARAM(0, APPCOMMAND_MEDIA_NEXTTRACK));
                    break;
            }
        }
    } while (hwnd != 0);
}


int main() {
    string msg = "1) SHIFT + F1:\tPrevious track\n2) SHIFT + F2:\tPlay/Pause\n3) SHIFT + F3:\tNext track\n4) SHIFT + F4:\tSkip ads\n";
    string exec_spotify = "IF EXIST %appdata%\\Spotify\\Spotify.exe (start %appdata%\\Spotify\\Spotify.exe --minimized) ELSE (start spotify --minimized)";
    vector<DWORD> app_ids;

    cout << msg;
    while (true) {
        if (GetAsyncKeyState(0xA0)) {
            // F1 Previous track
            if (GetAsyncKeyState(0x70)) {
                app_ids = vector<DWORD>(getProc());
                if (app_ids.size() > 4) for (auto& id : app_ids) SendCommand(id, 1);
                Sleep(500);
            }
            // F2 Pause
            if (GetAsyncKeyState(0x71)) {
                app_ids = vector<DWORD>(getProc());
                if (app_ids.size() > 4) for (auto& id : app_ids) SendCommand(id, 2);
                Sleep(500);
            }
            // F3 Next track
            if (GetAsyncKeyState(0x72)) {
                app_ids = vector<DWORD>(getProc());
                if (app_ids.size() > 4) for (auto& id : app_ids) SendCommand(id, 3);
                Sleep(500);
            }
            // F4 Skip ads
            if (GetAsyncKeyState(0x73)) {
                system("cls");
                system("Color 7");
                cout << "Processing Spotify";
                system("taskkill /im Spotify.exe >nul 2>&1");
                Sleep(1000);
                system(exec_spotify.c_str());

                while (true) {
                    app_ids = vector<DWORD>(getProc());
                    if (app_ids.size() > 4) {
                        cout << '\n';
                        Sleep(3500);

                        for (auto& id : app_ids) SendCommand(id, 2);

                        system("cls");
                        system("Color A");

                        for (int x = 3; x > 0; x--) {
                            cout << "SUCCESS! This msg will disappear in " << x << " seconds...";
                            Sleep(1000);
                            system("cls");
                        }

                        system("Color 7");
                        cout << msg;
                        break;
                    }
                    else {
                        Sleep(50); cout << ".";
                    }
                }
            }
        }
    }
}
