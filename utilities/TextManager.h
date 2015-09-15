#pragma once

#include "Util.h"
#include <map>
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>

using namespace std;

class TextManager
{
    public:
        TextManager(void);
        virtual ~TextManager(void);

        bool Init(void);

        bool LoadFontFaceFromFile(string filePath, const int fontSize = 12);

        // SETTERS >>
        bool SetFontSize(const int fontSize);

        void SetScaleX(const float scaleX);

        void SetScaleY(const float scaleY);

        void SetShaderProgram(const GLuint shaderProgramId);

        void SetViewProjMatrix(const glm::mat4 mtxViewProj);
        // << SETTERS

        const float GetTextWidth(void);

        const float GetTextHeight(void);

        void CalculateTextDimensions(const string text);

        void RenderText(const string text, const float x, const float y);

        void ReleaseObjects(void);

    private:
        FT_Library  m_Library;
        FT_Face     m_Face;
        FT_Error    m_Error;
        string      m_FontName;

        GLuint      m_Texture;
        GLuint      m_VertexBuffer;
        GLuint      m_VAO;
        GLuint      m_ShaderProgram;

        glm::mat4   m_mtxViewProj;

        struct GlyphMetrics
        {
            int pixelsInRow;
            int rows;
            int advanceX;
            int bearingY;
        };

        struct Glyph
        {
            GLuint textureId;
            GlyphMetrics glyphMetrics;
        };

        map<GLchar, Glyph>  m_Glyphs;
        vector<glm::vec2>   m_GlyphPositions;

        float   m_TextWidth;
        float   m_TextHeight;

        float   m_ScaleX;
        float   m_ScaleY;

        inline void SetTextureParams(void);
};
