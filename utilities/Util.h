#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdio>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#pragma once
struct floatRect
{
    float top;
    float left;
    float bottom;
    float right;
};

#pragma once
struct Vertex
{
    glm::vec3 position;
    glm::vec2 texUV;
};

#pragma once
struct ColorVertex
{
    glm::vec3 position;
    glm::vec4 color;
};
