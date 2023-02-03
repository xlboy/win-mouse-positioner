#include <iostream>
#include <windows.h>

using namespace std;

namespace GTestUtils {
  namespace Keyboard {
    void downKey(DWORD keyCode) {
      keybd_event(keyCode, 0, 0, 0);
    }
    void upKey(DWORD keyCode) {
      keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
    };
  } // namespace Keyboard

  namespace Mouse {
    void pressButton(DWORD dwFlags) {
      mouse_event(dwFlags, 0, 0, 0, 0);
      mouse_event(dwFlags, 0, 0, 0, 0);
    }
  } // namespace Mouse
} // namespace GTestUtils