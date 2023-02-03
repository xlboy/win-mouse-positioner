#pragma once

#include <functional>
#include <map>
#include <windows.h>
#include <vector>

namespace Hotkey {
  struct Entity {
      struct Code {
          DWORD normal;
          std::vector<DWORD> modifiers;
      } code;
      std::function<void(bool isDown)> callback;
  };
  template <typename K>
  class Manager {
    private:
      std::map<K, Entity> hotkeyMap;

    private:
      bool contain(const K id) {
        return hotkeyMap.find(id) != hotkeyMap.end();
      }

    public:
      void add(const K id, Entity hotkey) {
        hotkeyMap[id] = hotkey;
      };

      void clear() {
        hotkeyMap.clear();
      }

      void remove(const K id) {
        hotkeyMap.erase(id);
      }

      bool update(const K id, Entity hotkey) {
        if (!contain(id)) return false;
        hotkeyMap[id] = hotkey;
        return true;
      }

      bool handle(DWORD normalCode, bool isDown) {
        bool hasHandled = false;
        for (const auto &[id, content] : hotkeyMap) {
          if (normalCode != content.code.normal) continue;

          bool isModifierPressed = true;
          for (auto &modifier : content.code.modifiers) {
            if (!(GetKeyState(modifier) < 0)) {
              isModifierPressed = false;
              break;
            }
          }
          if (isModifierPressed) {
            content.callback(isDown);
            hasHandled = true;
          }
        }
        
        return hasHandled;
      }
  };
}; // namespace Hotkey
