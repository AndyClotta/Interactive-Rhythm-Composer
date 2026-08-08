# Progress: DanielDaviesVCV

## Estado Actual

El proyecto está **completo y funcional** como plugin de VCV Rack con 10 módulos registrados y versionados (v2.4.1). Recientemente se ha llevado a cabo un análisis integral del directorio y se ha establecido el Memory Bank.

## Qué Funciona

- **Los 10 módulos** están implementados y registrados en `plugin.json` y `DanielDavies.cpp`:
  - Sequel 8, Sequel 16, Quantify, Samuel, Blank3, Blank5, SequelSave, Bend, James, Knot.
- **Tema claro y oscuro:** Paneles SVG en `res/panels/light/` y `res/panels/dark/`.
- **Widgets personalizados:** Displays (dígitos, caracteres, teclado, líneas, OLED, texto) y controles (knobs, botones, sliders).
- **Utilidades reutilizables:** GateDetector, Quantize, SafeMenuItem, ClockTrackers, IgnoreClockAfterResetTimer.
- **Persistencia JSON:** Módulos clave (Knot, Bend) guardan/restauran estado.
- **Herramientas Python:** Generación de patrones Knot y compilación del manual a PDF.
- **Manual de usuario:** PDF generado desde SVGs individuales.

## Qué Falta o Pendiente

### Por Verificar
- [ ] **`SetCableColorMenu.hpp`:** Verificar si existe en `src/modules/Blanks/` (referenciado por Blank3/Blank5 pero no listado).

### Mejoras Opcionales
- [ ] Mover patrones de Knot a un archivo JSON externo (idea del propio autor).
- [ ] Portabilizar rutas hardcodeadas del Makefile (targets `run`, `copy`, `copy-pro`).
- [ ] Completar/implementar el modo de interpolación CUBIC_SPLINE en Bend (actualmente comentado).

## Funcionalidades Recientes

### Generador de género en James (implementado) ✓
- [x] Nuevo archivo `src/modules/James/JamesPatternGenerator.hpp` con 8 géneros (House, Techno, Trap, D&B, Minimal, Afro, Electro, Ambient).
- [x] Método `generateGatesForGenre(int)` en `James.cpp` que aplica el patrón generado a los gates.
- [x] Sección de menú contextual "Generate by genre".
- [x] Botón "Random" en el panel (derecha del Gate Mode) que genera variaciones del género seleccionado.
- [x] CHECKMARK en el menú para indicar el género activo.

### Motor generativo avanzado (Fase 1 + Fase 3) ✓
- [x] Refinado a **11 estilos** (House, Techno, Trap, D&B, Minimal, Afro, Electro, Ambient + Glitch, Braindance, Markovian).
- [x] **Euclidean sequencing** (`euclideanPattern`) aplicado automáticamente a la percusión según el estilo (House E(3,16), Techno E(7,16), Minimal E(3,16), Afro E(5,16)).
- [x] **Choke logic**: Open HH apaga Closed HH en estilos que lo usan (House, Techno).
- [x] **Markov como estilo seleccionable**: matriz de transición 6×6 basada en el instrumento del paso anterior.
- [x] Compilado exitosamente.

### Cambio de slug (implementado) ✓
- [x] `plugin.json`: slug `SequelSequencers` → `SequelSequencersDev`, nombre "Daniel Davies Dev".

## Problemas Conocidos

- **Rutas hardcodeadas** en el Makefile (específicas de Windows y del entorno del autor).
- **`CUBIC_SPLINE` deshabilitado** en Bend (código comentado en el menú contextual).
- **Posible referencia a archivo inexistente** (`SetCableColorMenu.hpp` en Blanks) — pendiente de verificación.

## Evolución de Decisiones del Proyecto

- **Estructura modular:** Cada módulo en su propia carpeta (`src/modules/<Modulo>/`), separando lógica (`Module`) y presentación (`ModuleWidget`).
- **Reutilización de código:** Utilidades y widgets compartidos en `src/utilities/` y `src/widgets/`.
- **Tema dual:** Se adoptó un sistema de tema claro/oscuro coherente en todos los paneles y widgets.
- **Persistencia de estado:** Se usa JSON (integrado en VCV Rack) para guardar el estado de los módulos.
- **Herramientas de generación:** Se desarrollaron scripts Python tanto para los patrones de Knot como para el manual.

## Historia de la Memoria

- **2026-08-08:** Creación inicial del Memory Bank tras un análisis completo del directorio del proyecto.