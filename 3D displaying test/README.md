# 3D Model Viewer

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg)](https://www.opengl.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)](https://www.microsoft.com/windows)

---

## About the Project

**3D Model Viewer** is a real-time 3D model visualization tool built from scratch in C++20 with OpenGL 3.3. It loads STL models via Assimp, renders them using custom GLSL shaders, and presents everything inside a Dear ImGui interface with off-screen framebuffer rendering.

### Main Features

- **Multiple shader modes** — Gradient (distance-based coloring) and Phong Lighting (ambient + diffuse + specular)
- **Rendering mode switching** — Filled, Wireframe, and Point Cloud views
- **Real-time light controls** — Adjustable ambient, diffuse, and specular strengths via ImGui sliders
- **Element usage slider** — Progressively reveal model geometry from 0% to 100%
- **Orbital camera** — Automated rotation around the model with toggle on/off
- **Off-screen rendering** — Scene rendered to FBO and displayed as a texture inside an ImGui window

---

## Technologies

| Category | Technology |
|----------|------------|
| **Language** | C++20 (MSVC v143) |
| **Graphics API** | OpenGL 3.3 Core Profile |
| **Windowing** | GLFW 3 |
| **GL Loader** | GLAD |
| **GUI** | Dear ImGui |
| **3D Import** | Assimp |
| **Math** | GLM |
| **Image Processing** | OpenCV |
| **Build** | MSBuild / Visual Studio 2022 |

---

## Architecture

```
3D displaying test/
??? Main.cpp                 # Application entry point, render loop, ImGui setup
??? Model.h / .cpp           # STL loading (Assimp), VAO/VBO, model transforms
??? Camera.h / .cpp          # Euler-angle camera, LookAt matrix
??? Shaders.h / .cpp         # Shader compilation, linking, uniform setters
??? Object3DDisplayer.h / .cpp   # FBO renderer, polygon mode, partial draws
??? DisplayManager.h / .cpp  # Display mode strategy manager
??? IImageModeDisplayer.h / .cpp # 2D image display interface (OpenCV)
??? shaders/
    ??? Gradient_vertex_shader.glsl      # Distance-based color gradient
    ??? Gradient_fragment_shader.glsl
    ??? ligthing_vertex_shader.glsl      # Phong illumination model
    ??? ligthing_fragment_shader.glsl
    ??? cube_vertex_shader.glsl          # Light source cube (flat white)
    ??? cube_fragment_shader.glsl
```

### Key Design Patterns

- **Inheritance & Polymorphism** — Base `ShaderProgram` class with specialized subclasses (`GradientShaderProgram`, `ShadowShaderProgram`, `CubeShaderProgram`) encapsulating uniform setup per technique
- **Strategy Pattern** — `IDisplayer` interface branching into `IImageModeDisplayer` and `IObject3DDisplayer`, managed by `DisplayManager` for runtime switching between 2D and 3D display modes
- **Factory-like Construction** — `Cube` class extends `Model` by injecting procedurally generated vertex data via base constructor

---

## Building

### Prerequisites
- **Windows 10/11**
- **Visual Studio 2022** with C++ Desktop workload
- **OpenCV** — set `MULTIOPTICALS_DEV_DIR` environment variable so that `%MULTIOPTICALS_DEV_DIR%\OpenCV\opencv\build\include` is valid

### Build (Visual Studio)

```powershell
# Clone the repository
git clone https://github.com/PiotrMen/3D-displaying-test.git
cd "3D-displaying-test"
```

1. Open `3D displaying test/3D displaying test.sln` in Visual Studio 2022
2. Select **x64 | Debug** or **x64 | Release** configuration
3. Build and run (**F5**)

> All core dependencies (GLFW, GLAD, GLM, ImGui, Assimp) are vendored in the `Dependencies/` directory. Only OpenCV requires an external installation.

### 3D Model Files

Place `.stl` files in `3d files/` relative to the working directory. The default model is `3d files/figure_Hollow_Supp.stl`.

---

## Controls

| Key / UI Element | Action |
|------------------|--------|
| `ESC` | Close the application |
| Shader radio buttons | Switch between Gradient and Lighting mode |
| Rendering radio buttons | Switch between Filled / Wireframe / Point Cloud (Gradient mode) |
| Element Usage slider | Control percentage of rendered vertices (0–100%) |
| Ambient / Diffuse / Specular sliders | Adjust light properties (Lighting mode) |
| Camera Rotation button | Toggle automated orbital camera |

---

## TODO / Future Plans

- [ ] Mouse-controlled camera (drag to orbit, scroll to zoom)
- [ ] Support for additional 3D formats (OBJ, FBX)
- [ ] Multiple light sources
- [ ] Texture mapping support
- [ ] Model file browser (runtime loading via file dialog)
- [ ] Skybox / environment mapping

---

## Author

- GitHub: [@PiotrMen](https://github.com/PiotrMen)
