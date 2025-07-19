# GL Abstraction Layer (GAL) V0.1

A personal project to learn about every nook and cranny of OpenGL, and eventually facilitate easier OpenGL programming for myself in the future.
This is currently under development and not even remotely a finished product.

GAL aims to be a thin abstraction layer over OpenGL, packing objects into neat classes that handle things for you in a simpler manner than the raw OpenGL API.
That said, GAL doesn't intend to cover ALL the functionality of OpenGL (not yet, anyway). So, expect to use native OpenGL functions if you want to do some of the more granular stuff. \
Due to the deliberate design of GAL to just be a thin abstraction layer (except the only-one-window system which I really ought to change at some point) and little more, you do still have to have knowledge of OpenGL to use it, it's not a plug-and-play kind of thing.

## Installation:
Copy all the files wherever you like to keep your libraries and include gal.hpp at the top of your file. \
C++17 and OpenGL 4.5 or later required.
## Dependencies:
- GLFW3
- GLM
- GLAD
###### I've never made a proper header only library I have no idea how you're meant to elegantly handle external dependencies

## Optional Macros:
- GAL_SUPPRESS_LOGS: Suppresses all console logging GAL does when defined.
- GAL_WARNINGS: Enables logging of warnings to console when non-fatal errors occur.
- GAL_DISABLE_MIN_GL_VERSION: Disable check for minimum OpenGL version (4.5) when calling setOpenGLVersion(). It is NOT recommended to do this, and the behaviour of GAL when this macro is defined is undocumented, but this option is here as a last resort.
