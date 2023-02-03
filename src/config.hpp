#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "./hotkey/index.hpp"

namespace {
  const std::string CONFIG_FILE_PATH = "./config.json";
  struct CustomToggleConfig : public Hotkey::Entity::Code {
      int monitorIndex;
  };

  typedef std::vector<CustomToggleConfig> CustomToggleConfigs;
} // namespace

class Config {
    using json = nlohmann::json;

  private:
    std::string configFilePath = CONFIG_FILE_PATH;
    struct {
        Hotkey::Entity::Code selectToogle;
        CustomToggleConfigs customToggleConfigs;
    } content;

  private:
    void init() {
      std::ifstream f(configFilePath);
      json config = json::parse(f);
      content.selectToogle = {
        .normal = config["selectToogle"]["normal"],
        .modifiers = config["selectToogle"]["modifiers"]
      };

      for (const auto &customToggleConfig : config["customToggleConfigs"]) {
        CustomToggleConfig c;
        c.monitorIndex = customToggleConfig["monitorIndex"].get<int>();
        c.normal = customToggleConfig["normal"].get<DWORD>();
        c.modifiers = customToggleConfig["modifiers"].get<std::vector<DWORD>>();

        content.customToggleConfigs.push_back(c);
      }
    };

  public:
  public:
    Config(std::string configFilePath = CONFIG_FILE_PATH) :
    configFilePath(configFilePath) {
      init();
    };

    Hotkey::Entity::Code &getSelectToogle() {
      return content.selectToogle;
    }

    CustomToggleConfigs &getCustomToggleConfigs() {
      return content.customToggleConfigs;
    }

    void refresh() {
      init();
    }
};