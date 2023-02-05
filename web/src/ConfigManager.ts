import {
  fileOpen,
  fileSave,
  FileWithHandle,
  supported,
} from "browser-fs-access";
import { DefaultModifierCode, ExternalHotkey, InsideHotkey } from "./types";
import { getVKCodeByWebKCode, getWebKCodeByVKCode } from "./utils/vk-codes";

interface ExternalConfig {
  selectToogle: ExternalHotkey;
  customToggleConfigs: Array<ExternalHotkey & { monitorIndex: number }>;
}

interface InsideConfig {
  selectToogle: InsideHotkey;
  customToggleConfigs: Array<InsideHotkey & { monitorIndex: number }>;
}

class ConfigManager {
  #fileType = "application/json";
  private externalConfigBlob: FileWithHandle | null = null;
  private insideConfig: InsideConfig | null = null;

  constructor() {}

  async save() {
    if (this.externalConfigBlob === null) return;

    const externalConfig: ExternalConfig = (() => {
      const { selectToogle, customToggleConfigs } = this.insideConfig!;

      const convertToExternalHotkey = (hotkey: InsideHotkey) => ({
        modifiers: hotkey.modifiers.map((m) => getVKCodeByWebKCode(m)!),
        normal: getVKCodeByWebKCode(hotkey.normal!)!,
      });

      return {
        selectToogle: convertToExternalHotkey(selectToogle),
        customToggleConfigs: customToggleConfigs.map((config) => ({
          ...convertToExternalHotkey({
            modifiers: config.modifiers,
            normal: config.normal,
          }),
          monitorIndex: config.monitorIndex,
        })),
      };
    })();

    const toSaveConfigBlob = new Blob(
      [JSON.stringify(externalConfig, null, 2)],
      {
        type: this.#fileType,
      }
    );

    const saveResult = await fileSave(
      toSaveConfigBlob,
      void 0,
      this.externalConfigBlob.handle
    );

    if (saveResult === null) {
      throw "保存失败";
    }
  }

  async open(): Promise<false | undefined> {
    this.externalConfigBlob = await fileOpen({
      mimeTypes: [this.#fileType],
    });

    if (this.externalConfigBlob === null) {
      return false;
    }

    try {
      const convertToInsideHotkey = (hotkey: ExternalHotkey) => ({
        modifiers: hotkey.modifiers.map(
          (m) => getWebKCodeByVKCode<DefaultModifierCode>(m)!
        ),
        normal: getWebKCodeByVKCode(hotkey.normal)!,
      });
      const externalConfig = JSON.parse(
        await this.externalConfigBlob.text()
      ) as ExternalConfig;

      this.insideConfig = {
        selectToogle: convertToInsideHotkey(externalConfig.selectToogle),
        customToggleConfigs: externalConfig.customToggleConfigs.map(
          (config) => ({
            ...convertToInsideHotkey({
              modifiers: config.modifiers,
              normal: config.normal,
            }),
            monitorIndex: config.monitorIndex,
          })
        ),
      };
    } catch (error) {
      throw error;
    }
  }

  getConfig() {
    return this.insideConfig;
  }

  updateConfig(config: InsideConfig) {
    this.insideConfig = config;
  }

  get supported() {
    return !!supported;
  }
}

export const configManager = new ConfigManager();
