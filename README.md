# Battlefield 3 UI Scaling Fix

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Releases](https://img.shields.io/github/v/release/IlIHydraIlI/BF3-UI-Scaling-Fix)](https://github.com/IlIHydraIlI/BF3-UI-Scaling-Fix/releases)

A fork of [GlacierLab's bf3_fix](https://github.com/GlacierLab/bf3_fix) that fixes Battlefield 3 UI scaling at high resolutions, ensuring the HUD matches the intended 720p proportions. This fork introduces additional fixes for text flickering, flag capture icon progression, and critical multiplayer gameplay bugs.

---

## 📁 File Overview

| File | Purpose |
|------|---------|
| `Engine.BuildInfo_Win32_Retail_dll` | **Required** – Core UI scaling fixes (HUD, minimap, nametags, commo rose) |
| `gm_fix.dll` | **Optional** – Gameplay/chat fixes (Crabwalk, Chat Lag, Unkillable Player) |

---

## 🚀 Features

### Core UI Improvements
- **High-Resolution Scaling:** Dynamically scales the HUD for 1080p, 1440p, 4K, and ultra-wide displays.
- **Text Flickering Fix:** Resolves the HUD text shaking/flickering issues present in GlacierLab's original implementation.
- **Flag Capture Progress Fix:** Fixes the bug where the flag capture icon visual progression freezes.

### Optional Gameplay & Performance Fixes (by [FlashHit](https://github.com/FlashHit))
- **[Crabwalk Bug Fix](https://www.youtube.com/watch?v=S9q62Ii3ObM):** Fixes the bug where players become unable to move after being revived multiple times in quick succession.
- **[Chat Lag Fix](https://www.youtube.com/watch?v=NSkG4IXkPHY):** Fixes severe FPS drops triggered when the client receives 200+ chat messages.
- **[Unkillable Player Bug Fix](https://www.youtube.com/watch?v=vm98Y7Ee1A0):** Fixes a vanilla bug where enemy players occasionally become completely immune to normal gunfire after consecutive revives (previously only killable with a defibrillator).

---

## 📸 Previews (1440p)

<p align="center">
  <img src="ScreenshotWin32-0007.png" alt="1440p UI Scaling Preview 1" width="45%" />
  <img src="ScreenshotWin32-0008.png" alt="1440p UI Scaling Preview 2" width="45%" />
</p>

---

## 🛠️ Installation

1. Go to the [Releases](https://github.com/IlIHydraIlI/BF3-UI-Scaling-Fix/releases) page and download:
   - `Engine.BuildInfo_Win32_Retail_dll` **(Required)**
   - `gm_fix.dll` **(Optional)**

2. Navigate to your Battlefield 3 installation folder (e.g., `...\Battlefield 3\`).

3. **Rename the original file:**
   - Locate `Engine.BuildInfo_Win32_Retail.dll`
   - Rename it to `ori_Engine.BuildInfo_Win32_Retail_dll`
   
   > This step is **mandatory**—the mod works as a proxy DLL and must load the original file under this specific name.

4. Drop the newly downloaded `Engine.BuildInfo_Win32_Retail_dll` into the game folder.

5. _(Optional)_ If you want the gameplay fixes, drop `gm_fix.dll` into the same folder.

6. Launch the game normally.

> [!NOTE]  
> **Anti-Cheat Safety:** These fixes have been extensively tested in multiplayer environments. Because they only alter specific memory logic locally, they will **not** trigger PunkBuster or server-side anti-cheat bans.

---

## 🗑️ Uninstall

Simply delete `Engine.BuildInfo_Win32_Retail_dll` and `gm_fix.dll` from your game folder, then rename `ori_Engine.BuildInfo_Win32_Retail_dll` back to `Engine.BuildInfo_Win32_Retail.dll`.

---

## ⚠️ Known Issues & Troubleshooting

### Known Issues
- **Killfeed Freezing:** The killfeed may occasionally stop updating.
  - *Workaround:* Alt-Tab out of the game and back in to reset it.

### Troubleshooting

| Issue | Solution |
|-------|----------|
| **Mod not loading** | Ensure you renamed the original file to `ori_Engine.BuildInfo_Win32_Retail_dll` exactly. |
| **Game crashing on startup** | If building from source, verify you're compiling as 32-bit (x86) – BF3 is a 32-bit application. |
| **Killfeed frozen** | Alt-Tab out and back in to reset it. |

---

## 🧠 How It Works (Technical Principle)

The developers hardcoded the maximum resolution for UI scaling calculation to `1280x720`, meaning the HUD becomes progressively smaller and unreadable as your resolution increases.

Because the Battlefield 3 executable is packed with a digital protector, modifying the static binary directly is impractical. Instead, this project uses a **DLL hook injection method**. Upon launch, the wrapper dynamically intercepts and modifies the memory logic responsible for checking the resolution boundaries, tricking the engine into scaling the UI universally across all higher resolutions.

---

## 💻 Build Environment

- **IDE:** Visual Studio 2022
- **Workload:** Desktop development with C++
- **Platform:** x86 (32-bit) – BF3 is a 32-bit application

---

## 🤝 Credits & Acknowledgments

- [SeanPesce / DLL_Wrapper_Generator](https://github.com/SeanPesce/DLL_Wrapper_Generator) - Base wrapper generation tool.
- [GlacierLab / bf3_fix](https://github.com/GlacierLab/bf3_fix) - Original UI scaling implementation.
- [FlashHit](https://github.com/FlashHit) - God mode / revive fixes and assistance with memory addresses.
- **aquamarine2000** - Contributions toward various aspect ratio scaling adjustments.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).