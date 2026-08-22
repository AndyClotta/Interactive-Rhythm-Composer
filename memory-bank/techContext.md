# Tech Context: Interactive Rhythm Composer

## Technologies
- **Language**: C++17
- **Framework**: VCV Rack SDK 2.x
- **Build system**: Makefile (via `make install`)
- **Panel design**: SVG (Inkscape), dual light/dark themes
- **Dependencies**: VCV Rack SDK (`RACK_DIR`), MSYS2 (Windows)

## Development Setup
- **OS**: Windows 11
- **IDE**: Cursor
- **Shell**: cmd.exe
- **Build command**: `make install` (outputs to `./dist`)
- **Install path**: `AppData\Local\Rack2\plugins-win-x64`

## Technical Constraints
- VCV Rack module API: `Module`/`ModuleWidget` base classes
- SVG panel rendering via nanosvg (no `inkscape:path-effect`, no `transform="scale(0)"`, no `display:none` layers)
- Coordinates in mm, converted via `mm2px(Vec(x, y))`
- Custom widgets must extend Rack's widget classes (e.g., `CKD6Latch`, `SvgKnob`)
- `dsp::PulseGenerator`, `dsp::Timer` for timing

## Dependencies
- VCV Rack SDK (external, set via `RACK_DIR` env var)
- No external libraries beyond Rack SDK

## Tool Usage Patterns
- `createParamCentered<T>(mm2px(Vec(x, y)), module, PARAM_ID)` for params
- `createInputCentered<T>(mm2px(Vec(x, y)), module, INPUT_ID)` for inputs
- `createOutputCentered<T>(mm2px(Vec(x, y)), module, OUTPUT_ID)` for outputs
- `createLightCentered<T>(mm2px(Vec(x, y)), module, LIGHT_ID)` for lights
- `configSwitch()`, `configParam()`, `configButton()`, `configInput()`, `configOutput()` in constructor
- Panel sync workflow: edit SVG → extract cx/cy → update C++ mm2px values → compile → copy to plugins dir