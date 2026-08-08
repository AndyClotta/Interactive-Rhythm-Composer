# Project Brief: DanielDaviesVCV

## Visión General

**DanielDaviesVCV** es un plugin de módulos para [VCV Rack](https://vcvrack.com/), el sintetizador modular virtual de código abierto. El plugin está desarrollado en **C++** y publicado bajo la licencia **GPL-3.0-or-later**.

- **Slug:** `SequelSequencers`
- **Marca/Autor:** Daniel Davies
- **Versión:** 2.4.1
- **Repositorio:** https://github.com/danieldavies99/DanielDaviesVCV.git
- **Email de contacto:** danieldavies99@gmail.com

## Propósito

Proporcionar una colección de módulos de síntesis modular con enfoque en secuenciadores, cuantización, generación rítmica y oscilación, con una estética visual coherente que soporta tema claro y oscuro.

## Módulos (10)

| Slug | Nombre | Tipo | Descripción |
|------|--------|------|-------------|
| `Sequel8` | Sequel 8 | Secuenciador | 3 filas / 8 pasos con divisor de reloj integrado |
| `Sequel16` | Sequel 16 | Secuenciador | 3 filas / 16 pasos con divisor de reloj integrado |
| `Quantify` | Quantify | Cuantizador | 3x cuantizador con entradas atenuvertidas, nudge y transposición |
| `Samuel` | Samuel | Generador rítmico | Generador de ritmo basado en código Morse |
| `Blank3` | Blank3 | Panel en blanco | 3HP |
| `Blank5` | Blank5 | Panel en blanco | 5HP |
| `SequelSave` | Sequel Save | Expander | Guardado de estado para Sequel 8 & 16 |
| `Bend` | Bend | Oscilador | Modulación de fase (square, sin, tri, noise) con modo LFO |
| `James` | James | Secuenciador de batería | 6 filas con control rush/drag por fila |
| `Knot` | Knot | Switch secuencial | 4x4, con patrones presets, undo/redo |

## Alcance

- Desarrollo de módulos DSP para VCV Rack
- Paneles SVG con tema claro y oscuro
- Manual de usuario en PDF (generado desde SVGs)
- Herramientas auxiliares en Python (generación de patrones Knot, compilación del manual)