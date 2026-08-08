# System Patterns: DanielDaviesVCV

## Arquitectura General

El plugin sigue el patrón estándar de VCV Rack: cada módulo se compone de un `Module` (lógica DSP) y un `ModuleWidget` (presentación/UI), registrados mediante `createModel<Module, Widget>("Slug")`.

```
src/
├── DanielDavies.hpp      # Cabecera global: declara Plugin *pluginInstance y Model *modelXxx
├── DanielDavies.cpp      # init(): registra los 10 modelos en el plugin
├── modules/              # Un directorio por módulo
├── utilities/            # Clases de utilidad reutilizables
└── widgets/              # Widgets de UI personalizados
```

## Patrón de Registro de Módulos

1. **`DanielDavies.hpp`** declara `extern Model *modelXxx;` para cada módulo.
2. Cada archivo de módulo (ej. `Knot.cpp`) define la struct `Module` y `ModuleWidget`, y al final crea el modelo:
   ```cpp
   Model *modelKnot = createModel<Knot, KnotWidget>("Knot");
   ```
3. **`DanielDavies.cpp`** en `init()` registra todos los modelos con `p->addModel(modelXxx);`.

## Patrón Module

Cada struct `Module : ModuleWidget`:
- Define enums `ParamId`, `InputId`, `OutputId`, `LightId` con `_LEN` al final.
- Constructor: `config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN)` + `configParam()` / `configInput()` / `configOutput()`.
- `process(const ProcessArgs &args)` — lógica DSP por muestra.
- `dataToJson()` / `dataFromJson()` — persistencia de estado del módulo.

## Patrón ModuleWidget

Cada struct `ModuleWidget`:
- `setModule(module)` y `setPanel(createPanel(light, dark))` — soporta tema claro/oscuro.
- `addChild(createWidget<ThemedScrew>)` — tornillos temáticos.
- `addParam` / `addInput` / `addOutput` / `addChild(createLightCentered)` — con `mm2px(Vec(...))` para posicionamiento.
- Posibilidad de `appendContextMenu(Menu *menu)` para opciones adicionales.

## Utilidades Reutilizables (`src/utilities/`)

| Archivo | Propósito |
|---------|-----------|
| `GateDetector.hpp/.cpp` | Detecta flancos de subida (gates) para control por pulsos |
| `IgnoreClockAfterResetTimer.hpp/.cpp` | Temporizador para ignorar reloj tras un reset |
| `Quantize.hpp/.cpp` | Cuantización con máscaras de teclas (KeyMask de 12 bits) |
| `SafeMenuItem.hpp/.cpp` | Items de menú contextual seguros |
| `SequelClockTracker.hpp/.cpp` | Seguimiento de reloj para secuenciadores Sequel |
| `JamesClockTracker.hpp/.cpp` | Seguimiento de reloj para el secuenciador James |

## Widgets Personalizados (`src/widgets/`)

- **Displays:** `DigitDisplay`, `CharacterDisplay`, `KeyboardDisplay`, `LineDisplay`, `OledPixelDisplay`, `TextInputDisplay`, `LengthValuesDisplay`
- **Controles:** `Knobs.hpp`, `Buttons.hpp`, `Sliders.hpp`
- Los displays reciben punteros a estado del módulo (ej. `value`, `inXconnected`, `outputXIndex`) para reflejar el estado en tiempo real.

## Patrón de Tema Claro/Oscuro

- Paneles SVG en `res/panels/light/` y `res/panels/dark/`.
- `createPanel(asset::plugin(pluginInstance, "res/panels/light/X.svg"), asset::plugin(pluginInstance, "res/panels/dark/X.svg"))`.
- Widgets temáticos: `ThemedScrew`, `ThemedPJ301MPort`.

## Persistencia de Estado (JSON)

Cada módulo sobreescribe `dataToJson()` / `dataFromJson()` para guardar/restaurar estado. Ej.:
- **Knot:** guarda `undoStack`, `redoStack`, `patternState`, `inputMap`.
- **Bend:** guarda `interpolationMode`.

## Caso Especial: Knot

- Es un **switch secuencial 4x4**: mapea 4 entradas a 4 salidas mediante índices.
- Incluye **pilas de undo/redo** (máx. 128 estados) con `KnotUndoState` (definido en `DanielDavies.hpp`).
- **128 patrones presets** hardcodeados en el array `patterns[]` (generados por `KnotPatternBuilder/patternBuilder.py`).
- Clase `KnotPattern` (en `KnotPattern.hpp`) interpreta códigos de control (ej. `"5U"` = swap_13 + undo).
- `GateDetector` para entradas de control manuales y de patrones.
- `LineDisplay` visualiza la conexión entrada↔salida con coincidencia de colores de cable (opción de menú "Match cable colors").

## Caso Especial: Bend

- **Oscilador de modulación de fase** con 4 salidas: square, sin, tri, noise.
- Usa **SIMD** (`simd::float_4`) para procesamiento polifónico (4 canales por iteración).
- Tablas de onda (`BendWavetables`), glissando (`BendGlideCalculator`), oscilador SIMD (`BendOscillatorSimd`) y antialiasing (`BendMinBlep`).
- Modo LFO con parámetro de frecuencia adaptativa (`FrequencyQuantity`).
- Menú contextual para seleccionar modo de interpolación (Lineal / Ninguno; Cubic spline comentado).