#pragma once

#include "Util.h"
#include <IL/il.h>

using namespace std;

struct Image
{
    int width;
    int height;
    int format;
    int type;
    int bpp;
    ILubyte* data;
};

class TextureManager
{
    public:
        TextureManager(void);
        virtual ~TextureManager(void);

        bool LoadTextureImage(string fileName);

        bool CreateTexture(void);

        bool CreateRectangleTexture(void);

        void ReleaseMemory(void);

        GLuint GetTextureId(void);

        const int GetImageWidth(void);

        const int GetImageHeight(void);
    private:
        GLuint      m_TextureId;
        ILuint      m_ImageId;
        Image       m_ImageInfo;
};
