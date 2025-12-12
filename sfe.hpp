#include <Core/core.hpp>
#include <Input/input.hpp>
#include <Input/input_glfw.hpp>
#include <Platform/platform.hpp>
#include <Renderer/renderer.hpp>

// you could do like #if defined(SFE_INCLUDE_GLFW) for input_glfw.hpp

// Vendor
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>