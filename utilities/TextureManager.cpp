#include "TextureManager.h"

TextureManager::TextureManager(void)
{

}

TextureManager::~TextureManager(void)
{

}

bool TextureManager::LoadTextureImage(string fileName)
{
    ilGenImages(1, &m_ImageId);
    ilBindImage(m_ImageId);

    if(ilLoadImage(fileName.c_str()) == IL_FALSE)
    {
        fprintf(stderr, "FAILED TO LOAD IMAGE!\n");
        ilDeleteImages(1, &m_ImageId);
        return false;
    }

    ilConvertImage(GL_RGBA, GL_UNSIGNED_BYTE);

    m_ImageInfo.width = ilGetInteger(IL_IMAGE_WIDTH);
    m_ImageInfo.height = ilGetInteger(IL_IMAGE_HEIGHT);
    m_ImageInfo.format = ilGetInteger(IL_IMAGE_FORMAT);
    m_ImageInfo.type = ilGetInteger(IL_IMAGE_TYPE);
    m_ImageInfo.bpp = ilGetInteger(IL_IMAGE_BPP);
    m_ImageInfo.data = ilGetData();

    return true;
}

bool TextureManager::CreateTexture(void)
{
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixels are tightly packed

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ImageInfo.width, m_ImageInfo.height,
                 0, m_ImageInfo.format, m_ImageInfo.type, (void*)m_ImageInfo.data);

    GLuint errorCode = glGetError();
    if(errorCode != GL_NO_ERROR)
    {
        fprintf(stderr, "FAILED TO CREATE TEXTURE! Error code: %d", errorCode);
        ReleaseMemory();
        return false;
    }
    ilDeleteImages(1, &m_ImageId);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture interpolation method to use linear interpolation (no MIPMAPS)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}

bool TextureManager::CreateRectangleTexture(void)
{
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_RECTANGLE, m_TextureId);

    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixels are tightly packed

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, m_ImageInfo.width, m_ImageInfo.height,
                 0, m_ImageInfo.format, m_ImageInfo.type, (const void*)m_ImageInfo.data);

    GLuint errorCode = glGetError();
    if(errorCode != GL_NO_ERROR)
    {
        fprintf(stderr, "FAILED TO CREATE RECTANGLE TEXTURE! Error code: %d", errorCode);
        ReleaseMemory();
        return false;
    }
    ilDeleteImages(1, &m_ImageId);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture interpolation method to use linear interpolation (no MIPMAPS)
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}

GLuint TextureManager::GetTextureId(void)
{
    return this->m_TextureId;
}

const int TextureManager::GetImageWidth(void)
{
    return this->m_ImageInfo.width;
}

const int TextureManager::GetImageHeight(void)
{
    return this->m_ImageInfo.height;
}

void TextureManager::ReleaseMemory(void)
{
    glDeleteTextures(1, &m_TextureId);
}
