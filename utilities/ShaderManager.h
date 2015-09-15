#pragma once

#include "Util.h"

using namespace std;

class ShaderManager
{
    public:
        ShaderManager(void);
        virtual ~ShaderManager(void);

        bool LoadShader(GLenum shaderType, string fileName);

        bool CompileShader(void);

        GLuint GetShader(void);

        void DeleteShader(void);

    protected:
    private:
        GLuint  m_Shader;
        bool LoadFileContent(string fileName, string& fileContent);
};
