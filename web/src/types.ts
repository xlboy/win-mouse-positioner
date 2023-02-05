import type { defaultNormalCodes, defaultModifierCodes } from "super-hotkey";

type DefaultNormalCode = typeof defaultNormalCodes[number];
export type DefaultModifierCode = typeof defaultModifierCodes[number];

export type DefaultCodes = DefaultModifierCode | DefaultNormalCode;

// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
export type VKCode = number;

export interface ExternalHotkey {
  modifiers: VKCode[];
  normal: VKCode;
}

export interface InsideHotkey {
  modifiers: DefaultModifierCode[];
  normal: DefaultNormalCode | DefaultModifierCode | undefined;
}
