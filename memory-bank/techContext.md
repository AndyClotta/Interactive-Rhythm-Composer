# Tech Context: Interactive Rhythm Composer

## Technologies
- **Language**: C++11 (compiled with `-std=c++11`)
- **Framework**: VCV Rack SDK 2.x
- **Build system**: Makefile (via `make install`), MSYS2 on Windows
- **Panel design**: SVG (Inkscape), dual light/dark themes, nanosvg renderer
- **Dependencies**: VCV Rack SDK (`RACK_DIR`), MSYS2 (Windows), jq (for plugin.json parsing), zstd (for .vcvplugin packaging)

## Development Setup
- **OS**: Windows 11
- **IDE**: Cursor
- **Shell**: cmd.exe
- **Make path**: `C:\msys64\usr\bin\make.exe`
- **Build command**: `C:\msys64\usr\bin\env.exe PATH="/C/msys64/usr/bin:/C/msys64/mingw64/bin:$PATH" C:\msys64\usr\bin\make.exe install`
- **Output**: `./dist/FreezerLabs/` and `FreezerLabs-1.1.0-win-x64.vcvplugin`
- **Install path**: `C:\Users\djand\AppData\Local\Rack2\plugins-win-x64`

## Technical Constraints
- VCV Rack module API: `Module`/`ModuleWidget` base classes
- SVG panel rendering via nanosvg (no `inkscape:path-effect`, no `transform="scale(0)"`, no `display:none` layers)
- Coordinates in mm, converted via `mm2px(Vec(x, y))`
- Custom widgets must extend Rack's widget classes (e.g., `CKD6Latch`, `SvgKnob`)
- `dsp::PulseGenerator`, `dsp::Timer` for timing
- `random::uniform()` for probability checks (Rack's DSP-safe random)
- No heap allocations in `process()` callback (DSP real-time safe)
- Render order: `addParam()` widgets draw before `addChild()` widgets

## Dependencies
- VCV Rack SDK (external, set via `RACK_DIR` env var, default: `C:/Dev/Rack-SDK`)
- No external libraries beyond Rack SDK

## Tool Usage Patterns
- `createParamCentered<T>(mm2px(Vec(x, y)), module, PARAM_ID)` for params
- `createInputCentered<T>(mm2px(Vec(x, y)), module, INPUT_ID)` for inputs
- `createOutputCentered<T>(mm2px(Vec(x, y)), module, OUTPUT_ID)` for outputs
- `createLightCentered<T>(mm2px(Vec(x, y)), module, LIGHT_ID)` for lights
- `configSwitch()`, `configParam()`, `configButton()`, `configInput()`, `configOutput()` in constructor
- Panel sync workflow: edit SVG → extract cx/cy → update C++ mm2px values → compile → copy to plugins dir
- Genre rules defined as `static const GenreRule GEN_RES[]` in `JamesPatternGenerator.hpp`

## .gitignore
```
/build
/dist
/plugin.so
/plugin.dylib
/plugin.dll
.DS_Store
*-bk*.svg
```

## Build Configuration
- **Makefile SOURCES**: explicit list (no `$(shell find ...)`) for cross-platform compatibility:
  - `src/plugin.cpp`
  - `src/modules/James/James.cpp`
  - `src/modules/James/JamesClockTracker.cpp`
  - `src/utilities/IgnoreClockAfterResetTimer.cpp`
- **RACK_DIR**: default `/path/to/Rack-SDK`, override via env var
- **Panel SVGs**: `res/panels/light/Interactive-Rhythm-Composer.svg` and `res/panels/dark/Interactive-Rhythm-Composer.svg`