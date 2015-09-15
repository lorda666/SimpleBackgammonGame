#include "TextManager.h"

TextManager::TextManager(void)
{
    this->m_TextWidth = 0.0f;
    this->m_TextHeight = 0.0f;

    this->m_ScaleX = 1.0f;
    this->m_ScaleY = 1.0f;
}

TextManager::~TextManager(void)
{

}

bool TextManager::Init(void)
{
    this->m_Error = FT_Init_FreeType(&this->m_Library);

    if(this->m_Error)
    {
        fprintf(stderr, "FAILED TO INITIALIZE FREETYPE LIBRARY!\n");
        return false;
    }

    return true;
}

bool TextManager::LoadFontFaceFromFile(string filePath, const int fontSize)
{
    this->m_Error = FT_New_Face(this->m_Library, filePath.c_str(), 0, &this->m_Face);
    if(this->m_Error == FT_Err_Unknown_File_Format)
    {
        fprintf(stderr, "ERRROR: UNKNOWN FILE FORMAT!\n");
        return false;
    }
    else if(this->m_Error)
    {
        fprintf(stderr, "ERROR LOADING FONT FROM FILE!\n");
        return false;
    }

    this->m_FontName = filePath;

    this->m_Error = FT_Set_Char_Size(this->m_Face, fontSize * 64, 0, 100, 0);
    if(this->m_Error)
    {
        fprintf(stderr, "FAILED TO SET CHARACTER SIZE!\n");
        return false;
    }

    //glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    //glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_GlyphSlot slot;
    FT_Glyph ftGlyph;

    Glyph glyph;
    int charIndex;
    size_t i;
    for(i = ' '; i <= '~'; i++)
    {
        charIndex = FT_Get_Char_Index(this->m_Face, i);

        this->m_Error = FT_Load_Glyph(this->m_Face, charIndex, FT_LOAD_RENDER);
        if(this->m_Error)
        {
            fprintf(stderr, "FAILED TO LOAD GLYPH INTO FACE!");
            return false;
        }

        this->m_Error = FT_Render_Glyph(this->m_Face->glyph, FT_RENDER_MODE_NORMAL);
        if(this->m_Error)
        {
            fprintf(stderr, "FAILED TO LOAD CHARACTER!\n");
            return false;
        }

        FT_Get_Glyph(this->m_Face->glyph, &ftGlyph);

        slot = this->m_Face->glyph;

        glGenTextures(1, &this->m_Texture);
        glBindTexture(GL_TEXTURE_2D, this->m_Texture);

        SetTextureParams();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, slot->bitmap.width, slot->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
        GLuint errorCode = glGetError();
        if(errorCode != GL_NO_ERROR)
        {
            fprintf(stderr, "Character: %c\n", i);
            fprintf(stderr, "FAILED TO CREATE TEXTURE! Error code: %d", errorCode);
            //ReleaseMemory();
            return false;
        }

        glyph.textureId = this->m_Texture;

        glBindTexture(GL_TEXTURE_2D, 0);

        glyph.glyphMetrics.pixelsInRow = slot->bitmap.width;
        glyph.glyphMetrics.rows = slot->bitmap.rows;
        glyph.glyphMetrics.advanceX = (slot->advance.x >> 6);
        glyph.glyphMetrics.bearingY = (slot->metrics.horiBearingY >> 6);

        this->m_Glyphs.insert(pair<GLchar, Glyph>(i, glyph));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenVertexArrays(1, &this->m_VAO);
    glBindVertexArray(this->m_VAO);

    glGenBuffers(1, &this->m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBuffer);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, texUV)));

    glBindVertexArray(0);

    return true;
}

bool TextManager::SetFontSize(const int fontSize)
{
    if(LoadFontFaceFromFile(this->m_FontName, fontSize) == false)
    {
        return false;
    }

    return true;
}

void TextManager::SetScaleX(const float scaleX)
{
    this->m_ScaleX = scaleX;
}

void TextManager::SetScaleY(const float scaleY)
{
    this->m_ScaleY = scaleY;
}

void TextManager::SetShaderProgram(const GLuint shaderProgramId)
{
    this->m_ShaderProgram = shaderProgramId;
}

void TextManager::SetViewProjMatrix(const glm::mat4 mtxViewProj)
{
    this->m_mtxViewProj = mtxViewProj;
}

const float TextManager::GetTextWidth(void)
{
    return this->m_TextWidth;
}

const float TextManager::GetTextHeight(void)
{
    return this->m_TextHeight;
}

void TextManager::CalculateTextDimensions(const string text)
{
    GlyphMetrics glyphMetrics;

    glm::vec2 currentPosition = glm::vec2(0.0f, 0.0f);
    int numberOfGlyphs = 0;
    float textHeight = 0;
    float textWidth = 0;
    string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++)
    {
        glyphMetrics = this->m_Glyphs[*c].glyphMetrics;
        this->m_GlyphPositions.push_back(currentPosition);
        currentPosition.x += glyphMetrics.advanceX;

        textWidth += glyphMetrics.advanceX;

        if(textHeight < glyphMetrics.rows)
        {
            textHeight = glyphMetrics.rows;
        }

        numberOfGlyphs ++;
    }
    int i;
    /*for(i = 0; i < numberOfGlyphs; i++)
    {
        this->m_TextWidth += this->m_GlyphPositions[i].x;
    }*/
    this->m_TextWidth = textWidth;
    this->m_TextHeight = textHeight;
}

void TextManager::RenderText(const string text, const float x, const float y)
{
    glBindVertexArray(this->m_VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBuffer);
    glm::mat4 mtxWorld;
    glm::mat4 mtxWorldViewProj = this->m_mtxViewProj * mtxWorld;

    glUseProgram(this->m_ShaderProgram);

    glActiveTexture(GL_TEXTURE1);

    Vertex vertices[4];
    GlyphMetrics glyphMetrics;
    int lastAdvanceX =  x;
    float baseLineY = y;
    float width, height;

    string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++)
    {
        if(*c == '\n')
        {
            lastAdvanceX = x;
            baseLineY += this->m_Glyphs['H'].glyphMetrics.rows +
                        this->m_Glyphs['H'].glyphMetrics.bearingY;
        }
        glyphMetrics = this->m_Glyphs[*c].glyphMetrics;

        width = lastAdvanceX;
        height = glyphMetrics.rows;
        lastAdvanceX = width + glyphMetrics.advanceX;

        vertices[0].position = glm::vec3(width, baseLineY - glyphMetrics.bearingY, 1.0f);// top left
        vertices[0].texUV = glm::vec2(0.0f, 0.0f);
        vertices[1].position = glm::vec3(lastAdvanceX, baseLineY - glyphMetrics.bearingY, 1.0f);// top right
        vertices[1].texUV = glm::vec2(1.0f, 0.0f);
        vertices[2].position = glm::vec3(width, baseLineY - glyphMetrics.bearingY + height, 1.0f);// bottom left
        vertices[2].texUV = glm::vec2(0.0f, 1.0f);
        vertices[3].position = glm::vec3(lastAdvanceX, baseLineY - glyphMetrics.bearingY + height, 1.0f);// bottom right
        vertices[3].texUV = glm::vec2(1.0f, 1.0f);

        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, this->m_Glyphs[*c].textureId);

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindVertexArray(0);
}

void TextManager::ReleaseObjects(void)
{
    FT_Done_Face(this->m_Face);
    FT_Done_FreeType(this->m_Library);

    size_t i;
    for(i = ' '; i <= '~'; i++)
    {
        glDeleteTextures(1, &this->m_Glyphs[i].textureId);
    }
    glDeleteTextures(1, &this->m_Texture);

    glDeleteBuffers(1, &this->m_VertexBuffer);
    glDeleteVertexArrays(1, &this->m_VAO);
}

void TextManager::SetTextureParams(void)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
