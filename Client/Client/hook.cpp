#include "hook.h"
#include <QDebug>
#include "signalemitter.h"
#include "initialisation.h"

HHOOK hHook = NULL;
HWINEVENTHOOK winEventHook = NULL;

void UpdateKeyState(BYTE *keystate, int keycode)
{
    keystate[keycode] = GetKeyState(keycode);
}

LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //WPARAM is WM_KEYDOWn, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
    //LPARAM is the key information

    if (wParam == WM_KEYUP) { //Note: KEYUP not KEYDOWN
        //Get the key information
        KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

        wchar_t buffer[5];

        //get the keyboard state
        BYTE keyboard_state[256];
        GetKeyboardState(keyboard_state);
        UpdateKeyState(keyboard_state, VK_SHIFT);
        UpdateKeyState(keyboard_state, VK_CAPITAL);
        UpdateKeyState(keyboard_state, VK_CONTROL);
        UpdateKeyState(keyboard_state, VK_MENU);

        //Get keyboard layout
        HKL keyboard_layout = GetKeyboardLayout(0);

        //Get the name
        char lpszName[0X100] = {0};

        DWORD dwMsg = 1;
        dwMsg += cKey.scanCode << 16;
        dwMsg += cKey.flags << 24;

        GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);

        //Try to convert the key information
        ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
        buffer[4] = L'\0';

        //Print the output
//        qDebug() << "Key: " << cKey.vkCode << " " << QString::fromUtf16((ushort*)buffer) << " " << QString::fromUtf16((ushort*)lpszName);

        Emitter::Instance()->keyPressed(QString::fromUtf16((ushort*)lpszName));
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(wParam == WM_LBUTTONUP) {
        Emitter::Instance()->leftClicked();
    }
    if(wParam == WM_RBUTTONUP) {
        Emitter::Instance()->rightClicked();
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void CALLBACK MyWinEventProc(HWINEVENTHOOK/* hWinEventHook*/, DWORD dwEvent, HWND hwnd, LONG/* idObject*/, LONG/* idChild*/, DWORD/* dwEventThread*/, DWORD/* dwmsEventTime*/)
{
    if(dwEvent == EVENT_SYSTEM_FOREGROUND) {
        wchar_t wnd_title[256];

        hwnd = GetForegroundWindow();

        HWND hParent = GetWindow(hwnd, GW_OWNER);

        if(hParent == NULL) {
            GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
//            std::wcout << wnd_title << std::endl;
            qDebug() << "Child:" << QString::fromUtf16((ushort*)wnd_title);
        }
        else {
            GetWindowText(hParent, wnd_title, sizeof(wnd_title));
            qDebug() << "Parent:" << QString::fromUtf16((ushort*)wnd_title);
        }

    }
}

Hook::Hook()
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
    hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    if (hHook == NULL) {
        qDebug() << "Hook Failed";
    }

    MSG msg;
    winEventHook = ::SetWinEventHook(EVENT_MIN, EVENT_MAX, NULL, MyWinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    if (winEventHook == NULL) {
        qDebug() << "Win Event Hook failed";
    }
    while(GetMessageW(&msg, 0, 0, 0)) {
        DispatchMessageW(&msg);

        if(Initialisation::quit) {
            PostQuitMessage(0);
            break;
        }
    }
}
