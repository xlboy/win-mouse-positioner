#include <functional>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <windows.h>

using namespace std;

namespace {
  typedef function<void(DWORD vkCode, bool isDown)> KeyboardEvent;

  HHOOK gKeyboardHook;
  KeyboardEvent gKeyboardEvent;

  LRESULT WINAPI keyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    CallNextHookEx(gKeyboardHook, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT *pStrucjt = (KBDLLHOOKSTRUCT *)lParam;
    DWORD vkCode = pStrucjt->vkCode;
    bool isKeyDown = !(pStrucjt->flags & LLKHF_UP);

    thread th(gKeyboardEvent, vkCode, isKeyDown);
    th.detach();

    return NULL;
  }
} // namespace

class KeyboardListener {
  private:
    void hookInstall() {
      gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookCallback, NULL, 0);

      MSG msg;
      while (!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }

      uninstallHook();
    }

    void uninstallHook() {
      UnhookWindowsHookEx(gKeyboardHook);
    }

  public:
    KeyboardListener(KeyboardEvent _event) {
      gKeyboardEvent = _event;
      hookInstall();
    }
    ~KeyboardListener() {
      uninstallHook();
    }
};