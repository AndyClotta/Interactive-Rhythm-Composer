# Product Context: Interactive Rhythm Composer

## Why This Project Exists
- Fork of Daniel Davies' "James" module, modified and maintained by AndyClotta
- Extends the original with genre-based pattern generation, per-step probability, and improved UI
- Designed for live performance and studio sequencing in VCV Rack

## Problems It Solves
- Complex drum sequencing with per-row timing offsets (rush/drag) without external clock manipulation
- Quick pattern generation via genre presets (11 genres)
- Probabilistic variation per step without external random sources
- Built-in clock eliminates need for separate clock module

## How It Should Work
- 6 independent gate rows, each with 16 steps
- Each row has a Rush/Drag knob (-15 to +15) for timing offset
- Gate switches support left-click toggle and right-click probability menu (0/25/50/75/100%)
- Gate/Trigger mode: continuous gate output vs trigger pulse
- Internal clock with speed control, or external clock with passthrough
- Reset input resets step counter
- Random button/CV generates new pattern variation for selected genre
- Step indicator lights show current position

## User Experience Goals
- Intuitive grid layout: 6 rows × 16 columns of gate switches
- Visual feedback: yellow LEDs for active gates, step indicator lights
- Dark/light panel themes for different environments
- Right-click probability on any gate switch without menu diving
- Genre generation via context menu for quick pattern inspiration