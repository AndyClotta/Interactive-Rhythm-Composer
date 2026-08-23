# Active Context: Interactive Rhythm Composer

## Current Work Focus
- Version 1.1.0 stable — feature-complete
- Panel SVGs renamed from `James.svg` to `Interactive-Rhythm-Composer.svg` (light/dark)
- Makefile fixed for macOS cross-platform build (explicit SOURCES instead of `find`)
- README updated with Clock & Synchronization section

## Recent Changes
- **Panel rename**: `res/panels/light/James.svg` → `res/panels/light/Interactive-Rhythm-Composer.svg`, `res/panels/dark/James.svg` → `res/panels/dark/Interactive-Rhythm-Composer.svg`
- **James.cpp**: updated `setPanel()` path to use new SVG filenames
- **Makefile**: replaced `$(shell find src -name "*.cpp")` with explicit SOURCES list for macOS compatibility
- **IgnoreClockAfterResetTimer.cpp**: added `#include "plugin.hpp"` first to resolve include circular
- **README**: added "Clock & Synchronization" section explaining 64 PPQN resolution and micro-timing
- **GenreRule refactored**: removed `chokeOpenToClosed` (now global), added `has_ratcheting` (Trap), `jitterAmount` (Glitch/Braindance)
- **GEN_RES[] updated**: House/Techno snare backbeat boosted, D&B 2-step kick, Minimal forced four-on-the-floor, Afro euclid 3/8 on kick, Electro euclid 5/16 on PERC1
- **ProbableGateSwitch::draw()**: green fill circle → red stroke ring (2.0f width) for probabilistic steps
- **Random button/CV**: now resets all `gateProbabilities[]` to 1.0f before generating new pattern
- **Branch**: migrated from `master` to `main`
- **.gitignore**: added `*-bk*.svg` pattern

## Next Steps
- No pending tasks. Module is feature-complete.

## Active Decisions
- Choke is global and unconditional: if HHABT (row 3) fires, HHCL (row 2) is silenced — applies to ALL genres
- Probability reset on Random ensures clean patterns without leftover per-step settings
- Red ring (stroke) instead of green circle for probabilistic visual feedback
- Ratcheting stored as `2.0f` marker in pattern array (engine reads it as double-trigger)
- Jitter amount stored as fraction of step (0.005 = 5ms, 0.010 = 10ms) — processed by clock engine
- Euclidean now supports two target rows: kick (row 0) and percussion (rows 4/5)
- Panel SVGs named after the module slug (`Interactive-Rhythm-Composer.svg`) instead of internal codename (`James.svg`)

## Important Patterns
- Grid coordinate formulas for gate switches: x = 26.987 + col*10.075, y = 33.867 + row*14.189
- Step indicator lights at y = 114.3, x = 26.987 + col*10.075
- Gate output jacks at x = 201.628, y = 33.867 + row*14.189
- Rush knobs at x = 13.135, y = 33.874 + row*14.182
- Ratcheting steps: 3, 7, 11, 15 (offbeats)

## Learnings
- `getRushValForRow()` negates the param value: `-static_cast<int>(params[NUM_GATE_SWITCHES + row].getValue())`
- `shouldPulseThisClock()` checks both current step (rush) and next step (drag) logic
- `getButtonId(row, col) = col + (row * 16)` — same formula for lights
- `getStepLedId(step) = 96 + step` (after gate switches)
- `getGateOutputId(row) = row` (0-5)
- `ProbableGateSwitch` renders BEFORE `SmallSimpleLight<YellowLight>` in draw order → yellow LED visible on top of red ring
- Euclidean on kick (Afro 3/8) and PERC1 (Electro 5/16) distinguished by `euclidK` value check
- `$(shell find ...)` in Makefile doesn't work reliably on macOS BSD find → use explicit SOURCES
- `#include "plugin.hpp"` must come before local headers to avoid include circular with `rack.hpp`