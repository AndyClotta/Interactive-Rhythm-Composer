# Product Context: DanielDaviesVCV

## ¿Por qué existe este proyecto?

Este plugin nace para ofrecer una colección de módulos de síntesis modular con un enfoque particular en **secuenciación y generación rítmica**, desarrollados por Daniel Davies. El objetivo es proporcionar herramientas creativas y accesibles para músicos y artistas que usan VCV Rack.

## Problemas que resuelve

- **Secuenciación melódica y rítmica:** Los secuenciadores Sequel (8 y 16 pasos) permiten crear patrones melódicos de 3 filas con divisor de reloj integrado.
- **Cuantización musical:** Quantify permite ajustar voltajes CV a escalas musicales con control fino (nudge, transposición).
- **Generación rítmica basada en lenguaje:** Samuel convierte código Morse en ritmos, un enfoque creativo y poco común.
- **Secuenciación de batería intuitiva:** James ofrece 6 filas de batería con control de rush/drag para humanizar los ritmos.
- **Síntesis de sonido flexible:** Bend es un oscilador de modulación de fase con múltiples salidas (onda cuadrada, seno, triangular, ruido) y modo LFO.
- **Enrutamiento creativo de señales:** Knot es un switch secuencial 4x4 con patrones programables, undo/redo y randomización.
- **Persistencia de estado:** SequelSave permite guardar y restaurar el estado de los secuenciadores.

## ¿Cómo debería funcionar?

- Cada módulo sigue los estándares de VCV Rack: entradas, salidas, parámetros y luces.
- Los módulos deben ser **polifónicos** donde tenga sentido (ej. Bend soporta múltiples canales, Knot maneja múltiples canales por entrada).
- La interfaz debe ser **intuitiva y visualmente coherente**, con tema claro y oscuro.

## Experiencia de Usuario

- **Panel visual atractivo:** Paneles SVG con estética clara y oscura, widgets personalizados (displays de dígitos, teclados musicales, líneas de conexión).
- **Feedback visual:** Luces LED para estados activos (ej. luz LFO en Bend, display de valor en Knot).
- **Control granular:** Knobs, sliders y switches personalizados con estética roja coherente.
- **Menús contextuales:** Opciones adicionales vía menú contextual (ej. modo de interpolación en Bend, coincidencia de colores de cable en Knot).

## Contexto Musical

- **Sequel 8/16:** Ideales para secuencias melódicas y progresiones.
- **Quantify:** Para asegurar que las secuencias suenen en escala.
- **Samuel:** Para ritmos experimentales basados en código Morse.
- **James:** Para patrones de batería con sensación humana (rush/drag).
- **Bend:** Para sonidos modulados, drones y efectos de fase.
- **Knot:** Para ruteo creativo y aleatorización controlada.