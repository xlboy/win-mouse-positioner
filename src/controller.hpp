#include <iostream>
#include <map>
#include <string>
#include <thread>
#include "./hotkey/index.hpp"
#include "./config.hpp"
#include "./keyboard-listener.hpp"
#include "./monitor.hpp"
#include "./utils/vk-codes.h"

namespace {
  enum class HotkeyAction {
    SelectToogle
  };

  const map</* VK Code */ DWORD, /* Monitor Number */ DWORD> keyboardNumberMap = {
    { VK_KEY_1, 0 },
    { VK_KEY_2, 1 },
    { VK_KEY_3, 2 },
    { VK_KEY_4, 3 },
    { VK_KEY_5, 4 },
    { VK_KEY_6, 5 },
    { VK_KEY_7, 6 },
    { VK_KEY_8, 7 },
    { VK_KEY_9, 8 },
  };

} // namespace

class Controller {
  private:
    Monitor monitor;
    Hotkey::Manager<string> hotkeyManager;
    Config config;
    bool isSelecting = false;
    Monitor::Position position = Monitor::Position::CenterCenter;

  private:
    void initHotkey() {
      hotkeyManager.add(
        "selectToogle",
        { config.getSelectToogle(),
          [this](bool isDown) {
            if (isDown) {
              isSelecting = true;
              std::cout << "SelectToogle" << std::endl;
            }
          } }
      );

      for (const auto &customToggleConfig : config.getCustomToggleConfigs()) {
        hotkeyManager.add(
          "customToggle" + to_string(customToggleConfig.monitorIndex),
          { customToggleConfig,
            [&](bool isDown) {
              if (isDown) {
                POINT point;
                monitor.getPoint(customToggleConfig.monitorIndex, position, point);
                SetPhysicalCursorPos(point.x, point.y);
                SetPhysicalCursorPos(point.x, point.y);
              }
            } }
        );
      }
    };

    void handleKeyboardEvent(DWORD vkCode, bool isDown) {
      bool hasHandled = hotkeyManager.handle(vkCode, isDown);

      // 如果当前按下的键与 `hotkeyManager` 中存储的热键匹配，则不进入下面的“其他处理”
      if (hasHandled) return;

      // #region  //*=========== handle-other-pressed-action ===========
      if (isDown && isSelecting) {
        if (keyboardNumberMap.find(vkCode) == keyboardNumberMap.end()) return;

        int curPressedMonitorNumber = keyboardNumberMap.at(vkCode);
        int monitorCount = monitor.getCount();

        if (curPressedMonitorNumber >= monitorCount || curPressedMonitorNumber < 0) return;

        POINT point;

        monitor.getPoint(curPressedMonitorNumber, position, point);
        SetPhysicalCursorPos(point.x, point.y);
        SetPhysicalCursorPos(point.x, point.y);

        isSelecting = false;
      }
      // #endregion  //*======== handle-other-pressed-action ===========
    };

  public:
    Controller() {
      initHotkey();

      std::thread([this]() {
        new KeyboardListener(
          [this](DWORD vkCode, bool isDown) {
            handleKeyboardEvent(vkCode, isDown);
          }
        );
      }).detach();
    };
};