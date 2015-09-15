#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
    m_Shader = 0;
}

ShaderManager::~ShaderManager()
{

}

bool ShaderManager::LoadFileContent(string fileName, string& fileContent)
{
    ifstream fin(fileName);

    if(!fin)
    {
        return false;
    }
    string line = "", content = "";
    while(!fin.eof())
    {
        getline(fin, line);
        content += line;
        content += "\n";
        line = "";
    }

    fin.close();

    fileContent = content;

    return true;
}

bool ShaderManager::LoadShader(GLenum shaderType, string fileName)
{
    GLint errorCode = glGetError();
    this->m_Shader = glCreateShader(shaderType);
    errorCode = glGetError();
    if(errorCode != 0)
    {
        fprintf(stderr, "FAILED TO CREATE SHADER! Error code: %d\n", errorCode);
        return false;
    }

    string fileContent = "";
    if(LoadFileContent(fileName, fileContent) == false)
    {
        return false;
    }

    const char* content = fileContent.c_str();
    glShaderSource(this->m_Shader, 1, &content, nullptr);

    errorCode = glGetError();
    if(errorCode != 0)
    {
        fprintf(stderr, "FAILED TO LOAD SHADER! Error code: %d", errorCode);
        return false;
    }

    return true;
}

bool ShaderManager::CompileShader(void)
{
    glCompileShader(this->m_Shader);

    GLint errorCode = glGetError();
    if(errorCode != 0)
    {
        fprintf(stderr, "ERROR COMPILING SHADER! Error code: %d", errorCode);
        GLint logSize = 0;
        glGetShaderiv(this->m_Shader, GL_INFO_LOG_LENGTH, &logSize);
        GLchar* errorLog;
        glGetShaderInfoLog(this->m_Shader, logSize, &logSize, errorLog);

        fprintf(stderr, "%s", errorLog);

        return false;
    }

    return true;
}

GLuint ShaderManager::GetShader(void)
{
    return this->m_Shader;
}

void ShaderManager::DeleteShader(void)
{
    glDeleteShader(this->m_Shader);
}
