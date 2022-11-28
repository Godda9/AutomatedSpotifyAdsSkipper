#include <iostream>
#include <windows.h>
#include <vector>
#include <TlHelp32.h>
#include <winuser.h>
#include <conio.h>
#include <sstream>


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


void SendCommand(DWORD dwProcessID) {
    HWND hwnd = NULL;
    do {
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
        DWORD dwPID = 0;
        GetWindowThreadProcessId(hwnd, &dwPID);
        if (dwPID == dwProcessID) {
            cout << "[DESC]: " << hwnd << " [ID]: " << dwPID << '\n';
            HWND EditClass = FindWindowEx(hwnd, NULL, L"Chrome_RenderWidgetHostHWND", NULL);

            PostMessage(EditClass, WM_APPCOMMAND, 0, MAKELPARAM(0, APPCOMMAND_MEDIA_PLAY_PAUSE));
            // PostMessage(EditClass, WM_APPCOMMAND, 0, MAKELPARAM(0, APPCOMMAND_MEDIA_NEXTTRACK));
        }
    } while (hwnd != 0);
}


int main() {
    cout << "[INFO]: Use R_SHIFT to skip. Do NOT close this window!" << '\n';

    // Cmd
    string command;
    vector<DWORD> app_ids;

    while (true) {
        if (GetAsyncKeyState(0xA1)) {
            system("cls");
            system("Color 7");
            Sleep(100);

            command = "taskkill /f /im Spotify.exe";
            system(command.c_str());

            Sleep(100);
            system("cls");
            cout << "[INFO]: Processing spotify: ";

                            /* [!] CHANGE THIS COMMAND [!] */
            //command = "start %appdata%\\Spotify\\Spotify.exe --minimized";
            command = "start spotify --minimized";
                            /* [!] CHANGE THIS COMMAND [!] */

            system(command.c_str());


            while (true) {
                app_ids = vector<DWORD>(getProc());
                if (app_ids.size() > 4) {
                    cout << '\n';
                    Sleep(3500);
                    for (auto& id : app_ids) {
                        SendCommand(id);
                    }

                    system("cls");
                    system("Color A");
                    cout << "[INFO]: SUCCESS!" << '\n';
                    break;
                }
                else {
                    Sleep(50);
                    cout << ".";
                }
            }
        }
    }
}
