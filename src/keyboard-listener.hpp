#include <functional>
#include <stdlib.h>
#include <thread>
#include <windows.h>

using namespace std;

namespace {
  typedef function<void(DWORD vkCode, bool isDown)> KeyboardEvent;
  typedef function<bool(DWORD vkCode, bool isDown)> HasMatchedKey;

  HHOOK gKeyboardHook;
  KeyboardEvent gKeyboardEvent;
  HasMatchedKey gHasMatchedKey;

  LRESULT WINAPI keyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pStrucjt = (KBDLLHOOKSTRUCT *)lParam;
    DWORD vkCode = pStrucjt->vkCode;
    bool isKeyDown = !(pStrucjt->flags & LLKHF_UP);
    bool isMatched = gHasMatchedKey(vkCode, isKeyDown);

    thread th(gKeyboardEvent, vkCode, isKeyDown);
    th.detach();

    return isMatched ? 1 : CallNextHookEx(gKeyboardHook, nCode, wParam, lParam);
  }
} // namespace

class KeyboardListener {
  private:
    void hookInstall() {
      gKeyboardHook =
        SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookCallback, NULL, 0);

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
    KeyboardListener(HasMatchedKey hasMatchedKey, KeyboardEvent _event) {
      gHasMatchedKey = hasMatchedKey;
      gKeyboardEvent = _event;
      hookInstall();
    }
    ~KeyboardListener() {
      uninstallHook();
    }
};