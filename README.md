# Freezer Labs - Interactive Rhythm Composer for VCV Rack

![Interactive Rhythm Composer](Interactive-Rhythm-Composer.png)

6 row drum sequencer with per-row rush/drag controls, built-in clock, genre-based pattern generation, and per-step probability.

Based on the **James** module by [Daniel Davies](https://github.com/danieldavies99).

Modified and maintained by [AndyClotta](https://github.com/AndyClotta).

## Features

- 6 independent gate rows (16 steps each)
- Per-row Rush/Drag controls for swing and timing offsets
- Built-in internal clock with speed control
- External clock input with passthrough output
- Reset input
- Gate/Trigger mode switch
- Per-step probability (right-click any gate switch)
- Genre-based pattern generation (11 genres)
- Random pattern generation
- Random CV input for external randomization

## Usage

### Right-click menu (module panel)

Right-click anywhere on the module panel to open the context menu:

| Action | Description |
|--------|-------------|
| **Randomize all gates** | Randomizes all 96 gate switches (6 rows × 16 steps) |
| **Randomize Gates for row 1–6** | Randomizes a single row independently |
| **Generate by genre** | Generates a pattern based on one of 11 genres: House, Techno, Trap, Drum & Bass, Minimal, Afro, Electro, Ambient, Glitch, Braindance, Markovian |
| **Clear all probabilities** | Resets all per-step probabilities to 100% (deterministic) |

![Right-click menu](right-click-meniu.png)

### Per-step probability

Right-click any gate switch to set its trigger probability. The switch LED changes color to indicate the probability level:

| Probability | Switch LED |
|-------------|------------|
| 100% | Yellow (deterministic) |
| 75% / 50% / 25% | Red (probabilistic) |
| 0% | Off (never triggers) |

Active steps also show a yellow indicator light behind the switch.

![Per-step probability](probability.png)

## License

GPL-3.0-or-later. See [LICENSE](LICENSE) for details.

## Building

### Prerequisites

- VCV Rack SDK
- MSYS2 (Windows) or build tools (Linux/Mac)

### Build

```bash
export RACK_DIR=/path/to/Rack-SDK
make install
```

The built plugin will be in the `./dist` directory.

Copy the contents of `./dist` to your VCV Rack plugins directory and launch Rack.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## Credits

- Original James module by [Daniel Davies](https://github.com/danieldavies99)
- Genre-based pattern generation adapted from the "Beat Agent" concept