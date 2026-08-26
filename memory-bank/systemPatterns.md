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
- **State**: `gateProbabilities[96]` (default 1.0f), `selectedGenre`, clock tracker state

### Widget (JamesWidget)
- Panel: dual SVG (light/dark) via `createPanel` — SVGs named `Interactive-Rhythm-Composer.svg`
- Custom widgets: `ProbableGateSwitch` (extends `CKD6Latch`), `SteppedRedKnob`, `RedKnob`
- Grid layout: 6 rows × 16 columns of gate switches + lights
- Right column: 6 gate outputs
- Left column: 6 rush knobs
- Top row: clock/step controls, inputs/outputs

### Custom Widgets
- `ProbableGateSwitch`: CKD6 latch with right-click probability menu + red stroke ring (2px) for probabilistic steps + dark background overlay to hide underlying red LED
- `SteppedRedKnob`: stepped knob for rush/drag and step count
- `RedKnob`: continuous knob for clock speed

### Render Order (per step)
1. `ProbableGateSwitch` (addParam) — draws dark cover + red ring if probabilistic
2. `SmallSimpleLight<YellowLight>` (addChild) — renders ON TOP as yellow dot when step active

## Design Patterns
- **Clock tracking**: `JamesClockTracker` manages step counting, rush/drag timing, clock division
- **Pattern generation**: `JamesPatternGenerator` with `GEN_RES[]` array of 11 genre definitions + `GenreRule` struct
- **Global HH choke**: unconditional — if HHABT fires (row 3), HHCL (row 2) silenced
- **Euclidean sequencing**: applied to KICK row 0 (Afro 3/8) or PERC1 row 4 (Electro 5/16) or PERC2 row 5 (legacy)
- **Ratcheting**: stored as 2.0f marker on HHCL steps 3,7,11,15 for Trap
- **Microtiming jitter**: `jitterAmount` metadata in GenreRule (Glitch 0.005, Braindance 0.010)
- **Markov chain**: `MARKOV_MATRIX[6][6]` for Markovian genre transitions
- **Ignore clock after reset**: `IgnoreClockAfterResetTimer` prevents false triggers on reset
- **Gate pulse generation**: `dsp::PulseGenerator[6]` for trigger mode
- **Per-step probability**: `gateProbabilities[96]` array, checked at clock tick with `random::uniform()`, reset to 1.0f on Random
- **Cross-platform build**: explicit SOURCES in Makefile (no `$(shell find ...)`) for macOS compatibility

## GenreRule Struct
```cpp
struct GenreRule {
    const char *name;        // Genre display name
    float probs[6][16];      // Probability matrix (0.0-1.0)
    int euclidK;             // Euclidean hits
    int euclidN;             // Euclidean steps (0 = disabled)
    bool useMarkov;          // Use Markov transition matrix
    bool has_ratcheting;     // 32nd-note ratchets on HHCL
    float jitterAmount;      // Microtiming offset (0.0 = none)
};
```

## Critical Implementation Paths
1. Clock processing: external/internal → shouldPulseThisClock() evaluates triggers on CURRENT step → probability check → gatePulseGenerators[i].trigger() → then clockTracker.nextClock() advances to next sub-tick/step. (Order critical: evaluate BEFORE advance so rush=0 fires on first sub-tick.)
2. Rush/Drag: getRushValForRow() → clockTracker.setRushForRow() → shouldPulseThisClock() compares rush value with clocksSinceLastStep
3. Genre generation: generateGatesForGenre() → generatePatternForGenre() → set params
4. Random button/CV: reset gateProbabilities[] to 1.0f → generateGatesForGenre(selectedGenre)
5. Gate mode: trigger (pulse) vs continuous (sustained while step active)
6. Panel sync: SVG coordinates (mm) → mm2px(Vec(x, y)) in C++

## Data Flow
```
Clock Input (rising edge)
  → for each row: shouldPulseThisClock(row)?
    → check gate switch state at current/next step
    → check rush/drag timing
    → check probability (gateProbabilities[step])
    → trigger gatePulseGenerator[row]
  → clockTracker.nextClock() (advance AFTER evaluation)
  → update step indicator lights
  → handle gate mode (trigger/continuous)
  → set output voltages