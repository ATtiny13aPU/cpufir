#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "OpenGL/stb_image.h"
#define NOMINMAX

#include "OpenGL/gl_func.h"
#include "OpenGL/shad_primitive.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
#include <stdio.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <vector>
#include <fstream>
#include <thread>
#include <cmath>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <array>
#include <queue>

#include "Vector.hpp"
#include "OSL/include.h"

#include "world.h"