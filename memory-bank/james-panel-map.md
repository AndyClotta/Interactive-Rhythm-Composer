# James — Mapa de sincronización panel (SVG ↔ C++)

Sincronización del panel de James entre el SVG (editado en Inkscape) y `src/modules/James/James.cpp` (`JamesWidget`). Al mover un elemento en Inkscape, su nueva posición `cx`/`cy` (capa `components`) debe reflejarse en el `mm2px(Vec(x, y))` del C++.

## Convención
- Los círculos de guía viven en la capa `components` (`<g id="layer2">`).
- Las coordenadas del SVG están en **mm**; el C++ usa `mm2px(Vec(x_mm, y_mm))`.
- Una posición de círculo `cx="77.362" cy="16.145"` equivale a `mm2px(Vec(77.362, 16.145))`.

## Controles individuales (knobs / switches / jacks)

| SVG label | C++ (James::) | x (mm) | y (mm) |
|-----------|---------------|--------|--------|
| `knob_step_count` | `KNOB_STEP_COUNT_PARAM` | 77.362 | 16.145 |
| `knob_clock_speed` | `KNOB_CLOCK_SPEED_PARAM` | 117.661 | 16.145 |
| `switch_gate_mode` | `SWITCH_GATE_MODE_PARAM` | 157.961 | 16.145 |
| `switch_random` | `SWITCH_RANDOM_PARAM` | 178 | 16.145 |
| `knob_rush_r0` | `KNOB_RUSH_R0_PARAM` | 13.135 | 33.874 |
| `knob_rush_r1` | `KNOB_RUSH_R1_PARAM` | 13.135 | 48.056 |
| `knob_rush_r2` | `KNOB_RUSH_R2_PARAM` | 13.135 | 62.238 |
| `knob_rush_r3` | `KNOB_RUSH_R3_PARAM` | 13.135 | 76.419 |
| `knob_rush_r4` | `KNOB_RUSH_R4_PARAM` | 13.135 | 90.601 |
| `knob_rush_r5` | `KNOB_RUSH_R5_PARAM` | 13.135 | 104.783 |
| `in_clock` | `IN_CLOCK_INPUT` | 57.212 | 16.145 |
| `in_reset` | `IN_RESET_INPUT` | 137.811 | 16.145 |
| `in_random` | `IN_RANDOM_INPUT` | 190 | 16.145 |
| `out_clock` | `OUT_CLOCK_OUTPUT` | 97.511 | 16.145 |
| `out_gate_r0` | `OUT_GATE_R0_OUTPUT` | 201.628 | 33.874 |
| `out_gate_r1` | `OUT_GATE_R1_OUTPUT` | 201.628 | 48.056 |
| `out_gate_r2` | `OUT_GATE_R2_OUTPUT` | 201.628 | 62.238 |
| `out_gate_r3` | `OUT_GATE_R3_OUTPUT` | 201.628 | 76.419 |
| `out_gate_r4` | `OUT_GATE_R4_OUTPUT` | 201.628 | 90.601 |
| `out_gate_r5` | `OUT_GATE_R5_OUTPUT` | 201.628 | 104.783 |

## Grilla de switches de gate (6 filas × 16 columnas)

- `switch_gate_r{row}_c{col}` → `SWITCH_GATE_R{row}_C{col}_PARAM`
- x de columna: `26.987 + col*10.075` (col 0..15)
- y de fila: `33.867 + row*14.189` (row 0..5)

## Luces (solo posición visual; van con `createLightCentered`)

| SVG label | C++ (James::) | x (mm) | y (mm) |
|-----------|---------------|--------|--------|
| `light_gate_mode_continuous` | `LIGHT_GATE_MODE_CONTINUOUS_LIGHT` | 164.679 | 11.551 |
| `light_gate_mode_trigger` | `LIGHT_GATE_MODE_TRIGGER_LIGHT` | 164.679 | 20.902 |
| `light_step_indicator_c{col}` | `LIGHT_STEP_INDICATOR_C{col}_LIGHT` | `26.987 + col*10.075` | 114.3 |

## Flujo de trabajo de sincronización

1. En Inkscape, editar `res/panels/light/James.svg` y `res/panels/dark/James.svg` (mover círculos de la capa `components`, cambiar fondo/gráficos). Mantener el `id`/label de cada círculo.
2. Avisar "listo". Yo extraigo `cx`/`cy` de cada círculo y actualizo el C++ correspondiente con `replace_in_file`, sin releer archivos completos.
3. Compilar una sola vez (`make install`) y copiar a `AppData\Local\Rack2\plugins-win-x64`.

## Advertencias nanosvg (importante al exportar desde Inkscape)
- NO usar `inkscape:path-effect` (fillet_chamfer) → usar `Path > Object to Path`/`Simplify`.
- NO dejar `transform="scale(0)"`.
- NO dejar capas con `display:none` (no aparecen en el browser).
- Guardar como "SVG plano" si es posible (sin atributos `inkscape:`/`sodipodi:`).