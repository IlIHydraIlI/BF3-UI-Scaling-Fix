# bf3_fix
A fork of [GlacierLab's](github.com/GlacierLab/bf3_fix) BF3 UI scaling fix, with additional fixes for flickering text. Optional godmode revive bug fix from [FlashHit](https://github.com/FlashHit) - fixes the "crabwalk" bug which occasionally happens from many consecutive revives.

![1440p](ScreenshotWin32-0007.png)
![1440p](ScreenshotWin32-0008.png)


## How to use
1. Download the `Engine.BuildInfo_Win32_Retail_dll` and optionally `gm_fix.dll` from [Releases](https://github.com/IlIHydraIlI/BF3-UI-Scaling-Fix/releases)
2. Rename `Engine.BuildInfo_Win32_Retail_dll` in your BF3 game folder to `ori_Engine.BuildInfo_Win32_Retail_dll`  
3. Put new `Engine.BuildInfo_Win32_Retail_dll` and optionally `gm_fix.dll` in BF3 game folder  
4. Launch game as normal  

## Build environment
Visual Studio 2022 with C++ workload installed  

## Principle
Developers at EA are idiots that they just hardcoded the maximum resolution for UI scaling to 1280x720, so UI scaling won't work on resolutions higher than that.  
This project dynamically modifies memory to replace the logic that checks if resolution is between 1280x720 and the maximum resolution, so UI scaling is always enabled.  
Why must it be dynamic memory modification? Because the game is packed with protector, and I'm not interested in dumping it, so DLL hook injection is simple and effective method to do this.  

## Note
I have used these fixes extensively in a multiplayer environment, should cause no issues related to punkbuster/anticheat. 

## License
MIT or [Qinlili Standard License](https://zhuanlan.zhihu.com/p/7134329439)  

## Credits
https://github.com/SeanPesce/DLL_Wrapper_Generator
github.com/GlacierLab/bf3_fix for the original UI fix
https://github.com/FlashHit for the God mode fix and help with memory addresses
aquamarine2000 for the flickering fix
