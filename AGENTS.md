# AGENTS.md

## Project overview
This repository is a minimal C++ OpenGL learning project. The main application logic lives in [main.cpp](main.cpp), and the project configuration is defined in [CMakeLists.txt](CMakeLists.txt). There is no larger engine architecture yet; prefer small, direct OpenGL examples and keep changes easy to follow.

## Build and validation
- Configure the project from the repository root with CMake.
- Typical workflow:
  - `cmake -S . -B build`
  - `cmake --build build --config Debug`
- This project targets C++23 and links against OpenGL, GLEW, GLFW, GLM, Microsoft GSL, and spdlog.
- The current setup expects a local GLEW installation at `C:/Program Files/Microsoft Visual Studio/18/Community/Libraries/glew-2.3.1`.
- If you change dependency or build settings, keep the CMake configuration explicit and minimal.

## OpenGL conventions
- Preserve the established lifecycle pattern in [main.cpp](main.cpp): initialize GLFW, configure window hints, create the window, make the context current, initialize GLEW, set the viewport, and then clean up on exit.
- Use `spdlog` for error reporting instead of ad hoc console output.
- Keep the window settings consistent with the current code: OpenGL 4.6 core profile and forward compatibility enabled.
- Maintain cleanup order: destroy the window, then terminate GLFW.
- Prefer small, self-contained rendering changes over introducing a large abstraction layer unless the task clearly requires it.

## Working style for agents
- Favor minimal, surgical edits that match the existing single-file style.
- If you add new functionality, keep it localized and document the intent briefly in code comments when the flow is non-obvious.
- Do not expand the project into a framework or multiple subsystems without explicit direction.
- When debugging runtime issues, check the initialization order and cleanup flow first; this project is sensitive to OpenGL/GLFW setup mistakes.

## Relevant files
- [README.md](README.md)
- [CMakeLists.txt](CMakeLists.txt)
- [main.cpp](main.cpp)
