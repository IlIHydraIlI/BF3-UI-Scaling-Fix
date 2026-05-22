# bf3_fix
Fix Battlefield 3 UI scaling issue on high resolution screens
![4K](preview.png)


## How to use
1. Clone this project and build OR download prebuilt version from [Release](https://github.com/GlacierLab/bf3_fix/releases/tag/Prebuilt)  
2. Rename `Engine.BuildInfo_Win32_Retail_dll` in game folder to `ori_Engine.BuildInfo_Win32_Retail_dll`  
3. Put new `Engine.BuildInfo_Win32_Retail_dll` in game folder  
4. Launch game as normal  

## Build environment
Visual Studio 2022 with C++ workload installed  

## Principle
Developers at EA are idiots that they just hardcoded the maximum resolution for UI scaling to 1280x720, so UI scaling won't work on resolutions higher than that.  
This project dynamically modifies memory to replace the logic that checks if resolution is between 1280x720 and the maximum resolution, so UI scaling is always enabled.  
Why must it be dynamic memory modification? Because the game is packed with protector, and I'm not interested in dumping it, so DLL hook injection is simple and effective method to do this.  

## Defects
- Some texts in settings menu will flicker  
- Only tested in single player mode, not guaranteed to not affect anti-cheat, please consider whether to use in multiplayer mode by your own  

## License
MIT or [Qinlili Standard License](https://zhuanlan.zhihu.com/p/7134329439)  

## Credits
https://github.com/SeanPesce/DLL_Wrapper_Generator
