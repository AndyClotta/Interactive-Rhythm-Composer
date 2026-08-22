# System Patterns: Interactive Rhythm Composer

## Architecture
- Single module (`James`) extending `Module` base class
- Single widget (`JamesWidget`) extending `ModuleWidget`
- Model registered via `createModel<James, JamesWidget>("InteractiveRhythmComposer")`

## Key Components

### Module (James)
- **Params**: 96 gate switches + 6 rush knobs + step count + clock speed + gate mode + random button = 105 params
- **Inputs**: Clock, Reset, Random CV (3 inputs)
- **Outputs**: 6 gate rows + clock passthrough (7 outputs)
- **Lights**: 96 gate LEDs + 16 step indicators + 2 mode lights = 114 lights
- **State**: `gateProbabilities[96]`, `selectedGenre`, clock tracker state

### Widget (JamesWidget)
- Panel: dual SVG (light/dark) via `ThemedScrew`/`createPanel`
- Custom widgets: `ProbableGateSwitch` (extends `CKD6Latch`), `SteppedRedKnob`, `RedKnob`
- Grid layout: 6 rows × 16 columns of gate switches + lights
- Right column: 6 gate outputs
- Left column: 6 rush knobs
- Top row: clock/step controls, inputs/outputs

### Custom Widgets
- `ProbableGateSwitch`: CKD6 latch with right-click probability menu + custom LED color (green for probabilistic, red for deterministic)
- `SteppedRedKnob`: stepped knob for rush/drag and step count
- `RedKnob`: continuous knob for clock speed

## Design Patterns
- **Clock tracking**: `JamesClockTracker` manages step counting, rush/drag timing, clock division
- **Pattern generation**: `JamesPatternGenerator` with `GEN_RES[]` array of 11 genre definitions
- **Ignore clock after reset**: `IgnoreClockAfterResetTimer` prevents false triggers on reset
- **Gate pulse generation**: `dsp::PulseGenerator[6]` for trigger mode
- **Per-step probability**: `gateProbabilities[96]` array, checked at clock tick with `random::uniform()`

## Critical Implementation Paths
1. Clock processing: external/internal → clockTracker.nextClock() → shouldPulseThisClock() → probability check → gatePulseGenerators[i].trigger()
2. Rush/Drag: getRushValForRow() → clockTracker.setRushForRow() → shouldPulseThisClock() compares rush value with clocksSinceLastStep
3. Genre generation: generateGatesForGenre() → generatePatternForGenre() → set params
4. Gate mode: trigger (pulse) vs continuous (sustained while step active)
5. Panel sync: SVG coordinates (mm) → mm2px(Vec(x, y)) in C++

## Data Flow
```
Clock Input → clockTracker.nextClock()
  → for each row: shouldPulseThisClock(row)?
    → check gate switch state at current/next step
    → check rush/drag timing
    → check probability
    → trigger gatePulseGenerator[row]
  → update step indicator lights
  → handle gate mode (trigger/continuous)
  → set output voltages