# Tech Context: DanielDaviesVCV

## Tecnologías

- **Lenguaje:** C++ (con estándares de VCV Rack)
- **Framework:** VCV Rack SDK (plugin development)
- **Build system:** Make (framework `plugin.mk` de VCV Rack)
- **Persistencia:** JSON (jansson, integrado en VCV Rack)
- **Aceleración:** SIMD (`simd::float_4` — usado en Bend)
- **Python:** Herramientas auxiliares (generación de patrones Knot, compilación del manual)
- **SVG:** Paneles e imágenes de widgets (tema claro/oscuro)

## Estructura del Build

**`Makefile`:**
```makefile
RACK_DIR ?= ../..
FLAGS +=
CFLAGS += -Isrc
CXXFLAGS += -Isrc
SOURCES += $(shell find src -name "*.cpp")
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
include $(RACK_DIR)/plugin.mk
```

- Requiere la variable de entorno `RACK_DIR` apuntando al SDK de VCV Rack.
- Compila recursivamente todos los `.cpp` en `src/`.
- Targets propios (específicos de Windows, rutas hardcodeadas):
  - `run` — ejecuta Rack Free
  - `copy` — copia el build al directorio de plugins (free)
  - `copy-pro` — copia el build al directorio de plugins (pro)
  - `build-and-run` — `make install && make copy && make run`

## Estructura de Directorios

```
├── Makefile
├── plugin.json              # Metadatos del plugin (slug, versión, módulos)
├── README.md
├── KnotPatternBuilder/
│   └── patternBuilder.py    # Genera códigos de patrones Knot
├── manual/
│   ├── daniel_davies_modular_manual.pdf
│   ├── generator/
│   │   └── svg2pdf.py       # Compila SVGs a PDF del manual
│   └── src/                 # Páginas SVG del manual (Cover, Samuel, Quantify, Sequel, SequelSave, James, Bend, Knot)
├── res/
│   ├── panels/light/        # Paneles SVG tema claro
│   ├── panels/dark/         # Paneles SVG tema oscuro
│   └── widgets/             # SVG de widgets (dígitos, teclas, knobs)
└── src/
    ├── DanielDavies.hpp/.cpp
    ├── modules/             # Bend, Blanks, James, Knot, Quantify, Samuel, Sequel, SequelSave
    ├── utilities/           # GateDetector, Quantize, SafeMenuItem, ClockTrackers, IgnoreClockAfterResetTimer
    └── widgets/             # Displays y controles personalizados
```

## Dependencias de Módulos

- **Bend:** `BendWavetables`, `BendOscillatorSimd`, `BendGlideCalculator`, `BendMinBlep`
- **Knot:** `utilities/GateDetector`, `widgets/Knobs`, `widgets/DigitDisplay`, `widgets/LineDisplay`, `KnotPattern`
- **Sequel8/16:** `SequelClockTracker`, `SequelSaveInterface`, `IgnoreClockAfterResetTimer`, widgets
- **James:** `JamesClockTracker`, widgets
- **Quantify:** `utilities/Quantize`, widgets
- **Samuel:** `SamuelSequenceGenerator`, widgets

## Herramientas de Desarrollo

- **Build:** MSYS2 (Windows), Homebrew (Mac), apt/pacman (Linux) — ver README para detalles.
- **SDK:** VCV Rack SDK (Rack-SDK-latest-win-x64.zip para Windows x64).
- **VCV Rack:** Rack Free 2.6.4.

## Notas Técnicas

- **`DanielDavies.hpp`** define `KnotUndoState` (estado de índices de salida para undo/redo de Knot).
- **`plugin.json`** versión 2.4.1, slug `SequelSequencers`, 10 módulos.
- **Interpolación en Bend:** `BendWavetable::InterpolationMode` (LINEAR y NONE activos; CUBIC_SPLINE comentado).
- **Patrones Knot:** 128 patrones en `Knot.cpp` (array `patterns[129]`, el índice 0 es "patrón vacío/desactivado").
- **`KnotPatternBuilder/patternBuilder.py`:** mapea patrones legibles (ej. `['swap_13', 'undo']`) a códigos compactos (ej. `"5U"`) imprimiendo las líneas `KnotPattern("...")` para pegar en `Knot.cpp`.
- **`manual/generator/svg2pdf.py`:** compila SVGs individuales (A4) a PDFs y los combina en un único manual PDF.