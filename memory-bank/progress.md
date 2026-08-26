# Progress: Interactive Rhythm Composer

## What Works
- All 105 params configured (96 gate switches + 6 rush knobs + step count + clock speed + gate mode + random)
- All 3 inputs (clock, reset, random CV)
- All 7 outputs (6 gate rows + clock passthrough)
- All 114 lights (96 gate LEDs + 16 step indicators + 2 mode lights)
- Internal clock with speed control
- External clock with passthrough
- Reset input with ignore-after-reset timer (1ms clock-ignore, now actually triggered on reset)
- First step correctly fires on clock start AND reset (fixed evaluation-order bug)
- Rush/Drag per row (-15 to +15)
- Gate/Trigger mode switch
- Per-step probability (right-click menu, 0/25/50/75/100%)
- Genre-based pattern generation (11 genres with refined rules)
- Random pattern generation (button + CV input) — resets probabilities to 100%
- Step indicator lights
- Dual SVG panel (light/dark themes) — renamed to `Interactive-Rhythm-Composer.svg`
- ProbableGateSwitch with red stroke ring for probabilistic steps
- Context menu: randomize all/row, genre generation, clear probabilities
- Global HH choke (all genres)
- Euclidean sequencing on KICK (Afro 3/8) and PERC1 (Electro 5/16)
- Ratcheting markers for Trap (HHCL on steps 3,7,11,15)
- Microtiming jitter metadata for Glitch (5ms) and Braindance (10ms)
- Cross-platform build: explicit SOURCES in Makefile (fixes macOS)
- README with Clock & Synchronization section (64 PPQN)

## What's Left to Build
- No known missing features. Module appears complete at version 2.0.0.

## Current Status
- **Version**: 2.0.0
- **Status**: Feature-complete, stable
- **Build**: Compiles with `make install`, deploys to Rack2 plugins directory
- **Branch**: `main`

## Known Issues
- None documented (first-tick bug resolved in 2.0.0).

## Evolution of Project Decisions
- Fixed "first tick" bug: trigger evaluation was moved BEFORE `nextClock()` advance; `ignoreClockAfterResetTimer.resetTriggered()` now wired into reset branch
- Forked from Daniel Davies' "James" module
- Added genre-based pattern generation (Beat Agent concept)
- Added per-step probability with custom ProbableGateSwitch widget
- Added random CV input for external randomization
- Migrated to dual SVG panel (light/dark)
- Green fill circle for probabilistic → red stroke ring
- Choke changed from per-genre flag to global unconditional
- Afro kick changed from manual probabilities to Euclidean 3/8
- Electro PERC1 changed from manual to Euclidean 5/16
- Random button/CV now clears all probabilities before generating
- Panel SVGs renamed from `James.svg` to `Interactive-Rhythm-Composer.svg`
- Makefile switched from `$(shell find ...)` to explicit SOURCES for macOS compatibility