#include "GLBase.h"

GLBase::GLBase(void)
{
    this->m_pWindow = nullptr;
    this->m_appInfo.windowWidth = 640;
    this->m_appInfo.windowHeight = 480;
    this->m_appInfo.windowTitle = "OpenGL App";
    this->m_appInfo.majorVersion = 4;
    this->m_appInfo.minorVersion = 4;
    this->m_appInfo.samples = 4;
    this->m_appInfo.isResizable = GL_TRUE;
}

GLBase::~GLBase(void)
{

}

bool GLBase::InitGLFW(void)
{
    if(glfwInit() == false)
    {
        fprintf(stderr, "FAILED TO INITIALIZE GLFW!\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->m_appInfo.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->m_appInfo.minorVersion);


#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // _DEBUG
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, this->m_appInfo.samples);
    glfwWindowHint(GLFW_RESIZABLE, this->m_appInfo.isResizable);

    m_appInfo.flags.stereo = (glfwGetWindowAttrib(this->m_pWindow, GLFW_STEREO) ? 1 : 0);
    glfwWindowHint(GLFW_STEREO, this->m_appInfo.flags.stereo ? GL_TRUE : GL_FALSE);

    glfwSetErrorCallback(glfwErrorCallback);

    this->m_pWindow = glfwCreateWindow(this->m_appInfo.windowWidth, this->m_appInfo.windowHeight,
                              this->m_appInfo.windowTitle.c_str(), nullptr, nullptr);
    if(!this->m_pWindow)
    {
        fprintf(stderr, "FAILED TO CREATE GLFW WINDOW!\n");
        glfwTerminate();

        return false;
    }

    glfwMakeContextCurrent(this->m_pWindow);

    glfwSetWindowTitle(this->m_pWindow, this->m_appInfo.windowTitle.c_str());
    glfwSetWindowSizeCallback(this->m_pWindow, glfwResizeCallback);
    glfwSetKeyCallback(this->m_pWindow, glfwKeyCallback);
    glfwSetMouseButtonCallback(this->m_pWindow, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(this->m_pWindow, glfwCursorPosCallback);

    glfwSetInputMode(this->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return true;
}

bool GLBase::InitGlew(void)
{
    glewExperimental = GL_TRUE;

    glewInit();

    glGetError();

    return true;
}

bool GLBase::LoadContent(void)
{
    return true;
}

bool GLBase::Init(void)
{
    glBase = this;
    if(InitGLFW() == false)
    {
        return false;
    }

    if(InitGlew() == false)
    {
        glfwTerminate();
        return false;
    }

    return LoadContent();
}

void GLBase::Update(float deltaTime)
{

}

void GLBase::Render()
{

}

void GLBase::Run(void)
{
    do
    {
        Update(0.0f);
        Render();

        glfwSwapBuffers(this->m_pWindow);
        glfwPollEvents();
    }
    while(glfwWindowShouldClose(this->m_pWindow) == false);

    ReleaseObjects();
}

void GLBase::ReleaseObjects(void)
{
    glfwDestroyWindow(this->m_pWindow);
    glfwTerminate();
}

void GLBase::OnResize(int width, int height)
{
    this->m_appInfo.windowWidth = width;
    this->m_appInfo.windowHeight = height;

    glViewport(0, 0, this->m_appInfo.windowWidth, this->m_appInfo.windowHeight);
}

void GLBase::OnKey(int key, int scancode, int action, int mods)
{

}

void GLBase::OnMouseButton(int button, int action, int mods)
{

}

void GLBase::OnCursorPos(double x, double y)
{

}

void GLBase::OnDebugMessage(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length, const GLchar* message)
{
    //OutputDebugStringA(message);
    //OutputDebugStringA("\n");
}

void GLBase::GetMousePosition(GLFWwindow* window, double& x, double& y)
{
    glfwGetCursorPos(window, &x, &y);
}

void GLBase::SetVsync(bool enabled)
{
    m_appInfo.flags.vsync = (enabled ? 1 : 0);
    glfwSwapInterval((int)m_appInfo.flags.vsync);
}

void GLBase::glfwResizeCallback(GLFWwindow* window, int width, int height)
{
    glBase->OnResize(width, height);
}

void GLBase::glfwKeyCallback(GLFWwindow* window, int key, int scancode,
                             int action, int mods)
{
    glBase->OnKey(key, scancode, action, mods);
}

void GLBase::glfwMouseButtonCallback(GLFWwindow* window, int button, int action,
                                     int mods)
{
    glBase->OnMouseButton(button, action, mods);
}

void GLBase::glfwCursorPosCallback(GLFWwindow* window, double x, double y)
{
    glBase->OnCursorPos(x, y);
}

void GLBase::glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "%d ", error);
    fprintf(stderr, "%s\n", description);
}

glm::vec2 GLBase::GetCursorPosition(void)
{
    glm::dvec2 cursorPosition;
    glfwGetCursorPos(m_pWindow, &cursorPosition.x, &cursorPosition.y);

    return cursorPosition;
}

GLBase* GLBase::glBase;
