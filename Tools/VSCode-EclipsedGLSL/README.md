# Eclipsed GLSL

VS Code language support for Eclipsed Engine shaders.

## File types

The first letter of the extension gives the shader stage:

| Extension | Stage |
|---|---|
| `.vglsl` | Vertex |
| `.pglsl` / `.fglsl` | Pixel (fragment) |
| `.gglsl` | Geometry |
| `.cglsl` | Compute |
| `.tglsl` / `.hglsl` | Tessellation control (hull) |
| `.eglsl` / `.dglsl` | Tessellation evaluation (domain) |
| `.glsl` | Guessed from the contents (or `eclipsedGlsl.defaultStage`) |

Standard `.vert/.frag/.geom/.comp/.tesc/.tese` also work.

## Features

- Syntax highlighting (GLSL 4.60 types, qualifiers, layout qualifiers, built-ins, swizzles) plus semantic highlighting of your structs, blocks, functions, parameters and macros.
- Completion: keywords, types, built-in functions and `gl_*` variables filtered by stage, your structs, functions, variables, uniform-block members and macros.
  - Member completion after `.`: struct fields, block members, swizzles (`xyzw/rgba/stpq`), `gl_in[i].`, `.length()` on arrays.
  - Inside `layout(`: layout qualifiers. After `#`: directives and `#version` profiles.
  - Typing `in ` in a pixel/geometry shader suggests the previous stage's outputs.
- Signature help and hover docs for built-in and user functions; hover shows declarations, block/binding info, and where an `in` is written in the previous stage.
- Go to definition, find references, highlight and rename. For stage `in`/`out` variables these work **across** the paired files (e.g. `DefaultSprite.vglsl` <-> `DefaultSprite.pglsl`).
- Outline / breadcrumbs, folding (braces, block comments, `// region`).
- Diagnostics:
  - Cross-stage linkage: `in` without a matching `out`, type mismatches, missing `flat` on integer inputs, unused outputs (faded), uniform blocks with mismatched members/bindings between stages.
  - Full compiler errors from **glslangValidator** if installed (Vulkan SDK, or set `eclipsedGlsl.validator.path`). Without it only basic bracket/#version checks run; the status bar shows a warning.
- Status bar shows the stage; click it (or `Alt+O`) to jump to the paired stage file.
- Commands: *Open Paired Shader Stage*, *New Vertex/Pixel Shader Pair* (also on folder right-click), *Re-run Validation*.
- Snippets: `vert`, `pixel`, `geom`, `compute`, `ubo`, `ssbo`, `camerabuffer`, `material`, `lin`, `lout`, `rot2`, `region`, ...

## Build / install

```
cd Tools/VSCode-EclipsedGLSL
npx @vscode/vsce package --skip-license
code --install-extension eclipsed-glsl-0.1.0.vsix
```
