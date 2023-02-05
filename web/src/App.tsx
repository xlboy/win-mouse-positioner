import { Kbd } from "@mantine/core";
import { useRef, useState } from "react";
import { tw } from "twind";
import { InsideHotkey } from "./types";
import { useSetState } from "ahooks";
import { motion } from "framer-motion";

interface HotkeyBlockProps {
  defaultHotkey: InsideHotkey;
  onChange(hotkey: InsideHotkey): void;
}
function HotkeyBlock(): JSX.Element {
  const [{ visible, currentShortcut }, setStates] = useSetState({
    visible: false,
    currentShortcut: {
      modifiers: [],
      normal: undefined,
    } as InsideHotkey,
  });

  const cacheData = useRef({
    hasReleasedAnyKey: true
  });

  return (
    <div
      tabIndex={0}
      data-autofocus
      onKeyDown={({ nativeEvent }) => handleMonitorInputKeyDown(nativeEvent)}
      // onKeyUp={({ nativeEvent }) => handleMonitorInputKeyUp(nativeEvent)}
    >
      {(() => {
        const { modifiers, normal } = currentShortcut;
        const hasModifiers = modifiers.length > 0;
        const hasNormal = !!normal;

        return [
          modifiers.map((modifier) => (
            <Kbd className={tw`mr-[2px]`} key={modifier}>
              {modifier}
            </Kbd>
          )),
          hasModifiers && hasNormal && (
            <span className={tw`mx-[3px]`} key="separate">
              +
            </span>
          ),
          hasNormal && (
            <Kbd className={tw`mr-[2px]`} key={normal}>
              {normal.toLocaleLowerCase()}
            </Kbd>
          ),
        ];
      })()}

      <motion.span
        className={tw`hidden w-[2px] h-[60%] bg-[#333]`}
        animate={{ opacity: [0, 1] }}
        transition={{ duration: 0.5, repeat: Infinity, repeatType: "reverse" }}
      />
    </div>
  );

  function handleMonitorInputKeyDown(event: KeyboardEvent) {
    event.stopPropagation();
    event.preventDefault();
  }
}

function App() {
  const [count, setCount] = useState(0);

  return (
    <div className={tw``}>
      <HotkeyBlock />
    </div>
  );
}

export default App;
