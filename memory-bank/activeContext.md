# Active Context: Interactive Rhythm Composer

## Current Work Focus
- Memory bank initialization and documentation
- No active development changes in progress

## Recent Changes
- Memory bank files created (projectbrief, productContext, systemPatterns, techContext, activeContext, progress)
- Existing `james-panel-map.md` documents SVG ↔ C++ coordinate sync

## Next Steps
- No pending tasks identified. Module appears feature-complete at version 2.4.1.

## Active Decisions
- Panel sync workflow documented in `james-panel-map.md`: edit SVG → extract coordinates → update C++ → compile
- `ProbableGateSwitch` uses custom LED override (green for probabilistic, dark for 0%, red default for 100%)
- Gate probability stored in `gateProbabilities[96]` array, default 1.0 (100% deterministic)
- Genre generation uses `GEN_RES[]` array with 11 genre definitions

## Important Patterns
- Grid coordinate formulas for gate switches: x = 26.987 + col*10.075, y = 33.867 + row*14.189
- Step indicator lights at y = 114.3, x = 26.987 + col*10.075
- Gate output jacks at x = 201.628, y = 33.867 + row*14.189
- Rush knobs at x = 13.135, y = 33.874 + row*14.182

## Learnings
- `getRushValForRow()` negates the param value: `-static_cast<int>(params[NUM_GATE_SWITCHES + row].getValue())`
- `shouldPulseThisClock()` checks both current step (rush) and next step (drag) logic
- `getButtonId(row, col) = col + (row * 16)` — same formula for lights
- `getStepLedId(step) = 96 + step` (after gate switches)
- `getGateOutputId(row) = row` (0-5)