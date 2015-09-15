#pragma once

#include "Util.h"

using namespace std;

struct AppInfo
{
    string      windowTitle;
    int         windowWidth;
    int         windowHeight;
    int         majorVersion;
    int         minorVersion;
    int         samples;
    int         isResizable;

    union
    {
        struct
        {
            unsigned int fullscreen : 1;
            unsigned int vsync : 1;
            unsigned int cursor : 1;
            unsigned int stereo : 1;
            unsigned int debug : 1;
        };
        unsigned int all;
    } flags;
};

class GLBase
{
    public:
        GLBase(void);
        virtual ~GLBase(void);

        virtual bool Init(void);

        virtual bool LoadContent(void);

        virtual void Update(float deltaTime);

        virtual void Render(void);

        virtual void ReleaseObjects(void);

        virtual void OnResize(int width, int height);

        virtual void OnKey(int key, int scancode, int action, int mods);

        virtual void OnMouseButton(int button, int action, int mods);

        virtual void OnCursorPos(double x, double y);

        virtual void OnDebugMessage(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length, const GLchar* message);

        static void GetMousePosition(GLFWwindow* window, double& x, double& y);

        void Run(void);

        //static GLBase& GetInstance(void);
        static GLBase* glBase;

    protected:
        AppInfo         m_appInfo;
        GLFWwindow*     m_pWindow;

        static void glfwResizeCallback(GLFWwindow* window, int width, int height);

        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode,
                                    int action, int mods);

        static void glfwMouseButtonCallback(GLFWwindow* window, int button,
                                            int action, int mods);

        static void glfwCursorPosCallback(GLFWwindow* window, double x, double y);

        static void glfwErrorCallback(int error, const char* description);

        void SetVsync(bool enabled);

        glm::vec2 GetCursorPosition(void);

    private:
        bool InitGLFW(void);

        bool InitGlew(void);
};
