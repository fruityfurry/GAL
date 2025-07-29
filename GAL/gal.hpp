#ifndef GAL_GAL_HPP
#define GAL_GAL_HPP

#ifndef GAL_NO_GLAD_INCLUDE
#include <glad.h>
#endif
#ifndef GAL_NO_GLFW_INCLUDE
#include <glfw3.h>
#endif
#ifndef GAL_NO_GLM_INCLUDE
#include "detail/glmIncludes.hpp"
#endif

#include "detail/Buffer.hpp"
#include "detail/debug.hpp"
#include "detail/enums.hpp"
#include "detail/GALException.hpp"
#include "detail/glParams.hpp"
#include "detail/init.hpp"
#include "detail/keyboard.hpp"
#include "detail/ResourceTracker.hpp"
#include "detail/ShaderProgram.hpp"
#include "detail/shapes.hpp"
#include "detail/Texture.hpp"
#include "detail/vertex.hpp"
#include "detail/VertexArray.hpp"
#include "detail/Window.hpp"

#endif