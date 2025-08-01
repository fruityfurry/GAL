# GL Abstraction Layer (GAL) V0.2

A personal project to learn about every nook and cranny of OpenGL, and eventually facilitate easier OpenGL programming for myself in the future.
This is currently under development and not even remotely a finished product.

GAL aims to be a thin abstraction layer over OpenGL, packing objects into neat classes that handle things for you in a simpler manner than the raw OpenGL API.
That said, GAL doesn't intend to cover ALL the functionality of OpenGL (not yet, anyway). So, expect to use native OpenGL functions if you want to do some of the more granular stuff. \
Due to the deliberate design of GAL to just be a thin abstraction layer and little more, you do still have to have knowledge of OpenGL to use it, it's not a plug-and-play kind of thing.

## Installation:
Copy all the files wherever you like to keep your libraries and include gal.hpp at the top of your file. \
C++17 and OpenGL 4.5 or later required.
## Dependencies:
- GLAD
- GLFW3
- GLM

All these files should be accessible directly by their header file names (e.g., #include <glfw3.h> should work) and not through an extra directory (so not #include <glfw3/glfw3.h>), except in the case of glm, which should be accessible like #include <glm/glm.hpp>. See the GAL_NO_*_INCLUDE macros to get around this.

###### I've never made a proper header only library I have no idea how you're meant to elegantly handle external dependencies

## Optional Macros:
- `GAL_NO_GLAD_INCLUDE`: Don't include GLAD in gal.hpp. This means it's now your responsibility to include it before you include gal.hpp.
- `GAL_NO_GLFW_INCLUDE`: Don't include GLFW in gal.hpp. This means it's now your responsibility to include it before you include gal.hpp.
- `GAL_NO_GLM_INCLUDE`: Don't include GLM in gal.hpp. This means it's now your responsibility to include it (ideally by including detail/glmIncludes.hpp, as it includes all the modules GAL needs) before you include gal.hpp.
- `GAL_SUPPRESS_LOGS`: Suppresses all console logging GAL does.
- `GAL_DISABLE_MIN_GL_VERSION`: Disable check for minimum OpenGL version (4.5) when calling setOpenGLVersion(). It is NOT recommended to do this, and the behaviour of GAL when this macro is defined is undocumented, but this option is here as a last resort.

## Additional Notes:
Some member methods may be appended with the suffix `AB` or `NB`. These stand for Auto-Bind and No-Bind respectively, and indicate whether this function binds the object before performing its function or does not, in which case you are assumed to have bound the object before calling the method. \
Methods not marked with `AB` or `NB` can be assumed to use DSA functions equivalent to those that require binding beforehand, so they don't bind anything.
