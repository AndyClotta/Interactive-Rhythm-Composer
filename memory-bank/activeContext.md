# Active Context: DanielDaviesVCV

## Trabajo Actual

Se realizó un **análisis completo del directorio** del proyecto DanielDaviesVCV. El resultado de este análisis se ha documentado en el Memory Bank (este conjunto de archivos: projectbrief, productContext, systemPatterns, techContext, activeContext, progress).

Posteriormente se implementó una **nueva funcionalidad en el módulo James**: un generador probabilístico de patrones de batería por género (inspirado en el proyecto externo "Beat Agent").

## Cambios Recientes

- **Creación del Memory Bank** (`memory-bank/`): Se crearon los 6 archivos núcleo documentando la arquitectura, el stack técnico, el contexto de producto y el estado del proyecto.
- **Cambio de slug del plugin**: `SequelSequencers` → `SequelSequencersDev` (nombre: "Daniel Davies Dev") en `plugin.json` para no pisar los módulos originales instalados.
- **Nuevo generador de género en James**:
  - Archivo nuevo `src/modules/James/JamesPatternGenerator.hpp` con struct `GenreRule`, array `GEN_RES` (8 géneros) y función `generatePatternForGenre`.
  - Modificado `src/modules/James/James.cpp`: añadido `#include "JamesPatternGenerator.hpp"`, método `generateGatesForGenre(int)` y sección de menú contextual "Generate by genre" con 8 sub-items.

## Decisiones Activas y Consideraciones

### Hallazgos del Análisis

1. **Posible archivo faltante:** `Blanks/Blank3.cpp` y/o `Blanks/Blank5.cpp` referencian `SetCableColorMenu.hpp`, pero este archivo **no aparece listado** en la carpeta `src/modules/Blanks/` (solo `Blank3.cpp` y `Blank5.cpp`). Es necesario **verificar** si existe o si es un error de referencia.

2. **Código DSP avanzado en Bend:** El módulo Bend usa SIMD, tablas de onda, interpolación, glide y modulación de fase. Es el módulo técnicamente más complejo.

3. **Patrones Knot hardcodeados:** Los 128 patrones presets están hardcodeados en `Knot.cpp`. El autor comenta en el código que "podría ser agradable almacenar esto en JSON o algo similar más adelante" — una posible mejora futura.

4. **Rutas hardcodeadas en Makefile:** Los targets `run`, `copy`, `copy-pro` contienen rutas específicas de un sistema (Windows) y del equipo del autor. No son portables.

5. **Interpolación CUBIC_SPLINE comentada:** En Bend, el modo de interpolación "Cubic spline" está descomentado en el menú (excluido), sugiriendo que el modo no está completamente implementado o probado.

## Patrones y Preferencias

- **Idioma de la documentación:** El README y el código están en inglés. La documentación del Memory Bank se está redactando en español (según preferencia del usuario).
- **Estética:** Tema claro/oscuro coherente, knobs rojos, displays de dígitos, teclado musical.
- **Estilo de código:** Enums con sufijo `_LEN`, `mm2px(Vec(...))` para posicionamiento de widgets, `setPanel` con tema dual.

## Aprendizajes y Conocimientos del Proyecto

- El plugin soporta **polifonía** en módulos clave (Bend con SIMD, Knot con múltiples canales).
- La **persistencia de estado** usa JSON (`dataToJson`/`dataFromJson`).
- **Knot es el módulo más complejo en cuanto a lógica de control** (patrones, undo/redo, randomización).
- **Bend es el módulo más complejo en cuanto a procesamiento de audio** (SIMD, wavetables, anti-aliasing).

## Próximos Pasos

- [ ] **Compilar y probar** el nuevo generador de género en James (`make install` + copiar `dist/SequelSequencersDev`).
- [ ] Verificar la existencia de `SetCableColorMenu.hpp` en `src/modules/Blanks/`.
- [ ] (Opcional) Considerar mover los patrones de Knot a un archivo JSON externo.
- [ ] (Opcional) Portabilizar las rutas del Makefile.
- [ ] (Opcional) Completar/implementar el modo de interpolación CUBIC_SPLINE en Bend.