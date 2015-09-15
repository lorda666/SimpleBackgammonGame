#include "Notification.h"

Notification::Notification(void)
{
    //ctor
}

Notification::~Notification(void)
{
    //dtor
}

void Notification::SetNotificationMessage(const string message)
{
    this->m_NotificationMessage = message;
}

void Notification::SetPosition(const glm::vec2 position)
{
    this->m_Container.SetPosition(position);
}

void Notification::SetWidth(const float width)
{
    this->m_Container.SetWidth(width);
}

void Notification::SetHeight(const float height)
{
    this->m_Container.SetHeight(height);
}

void Notification::SetViewProjMatrix(const glm::mat4 mtxViewProj)
{
    this->m_mtxViewProj = mtxViewProj;
}

void Notification::SetShaderProgram(const GLuint shaderProgramId)
{
    this->m_ColorSP = shaderProgramId;
}

void Notification::SetGlyphShaderProgram(const GLuint shaderProgramId)
{
    this->m_GlyphTexMapSP = shaderProgramId;
}

void Notification::SetScale(const float scaleX, const float scaleY)
{
    this->m_ScaleX = scaleX;
    this->m_ScaleY = scaleY;
}

void Notification::Show(void)
{
    glm::mat4 mtxWorld;
    glm::mat4 mtxWorldViewProj;

    glUseProgram(this->m_ColorSP);
    glBindVertexArray(this->m_ContainerVAO);

    mtxWorld = this->m_Container.GetModelMatrix();
    mtxWorldViewProj = this->m_mtxViewProj * mtxWorld;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mtxWorldViewProj));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);

    float textWidth;
    float textHeight;

    this->m_TextManager.CalculateTextDimensions(this->m_NotificationMessage);
    textWidth = this->m_TextManager.GetTextWidth();
    textHeight = this->m_TextManager.GetTextHeight();

    float notificationX = this->m_ContainerBoundingRect.left +
                ((this->m_Container.GetWidth() - textWidth) / 2.0f);
    float notificationY = this->m_ContainerBoundingRect.top +
                ((this->m_Container.GetHeight() - textHeight) / 2.0f);
    this->m_TextManager.RenderText(this->m_NotificationMessage,
                                   notificationX, notificationY);

    glUseProgram(this->m_ColorSP);
    glBindVertexArray(this->m_OKButtonVAO);

    mtxWorld = this->m_OKButton.GetModelMatrix();
    mtxWorldViewProj = this->m_mtxViewProj * mtxWorld;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mtxWorldViewProj));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);

    this->m_TextManager.CalculateTextDimensions("OK");
    textWidth = this->m_TextManager.GetTextWidth();
    textHeight = this->m_TextManager.GetTextHeight();

    float buttonContentX = this->m_OKButtonBoundingRect.left +
                ((this->m_OKButton.GetWidth() - textWidth) / 2.0f);
    float buttonContentY = this->m_OKButtonBoundingRect.top +
                (this->m_OKButton.GetHeight() - textHeight);
    this->m_TextManager.RenderText("OK", buttonContentX, buttonContentY);
}

bool Notification::Init(void)
{
    float halfContainerWidth = this->m_Container.GetWidth() / 2.0f;
    float halfContainerHeight = this->m_Container.GetHeight() / 2.0f;
    this->m_ContainerBoundingRect = this->m_Container.GetBoundingRectangle();

    ColorVertex containerVertices[] =
    {
        { glm::vec3(-halfContainerWidth, -halfContainerHeight, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) },
        { glm::vec3(halfContainerWidth, -halfContainerHeight, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) },
        { glm::vec3(-halfContainerWidth, halfContainerHeight, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) },
        { glm::vec3(halfContainerWidth, halfContainerHeight, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) },
    };

    glGenVertexArrays(1, &this->m_ContainerVAO);
    glBindVertexArray(this->m_ContainerVAO);

    glGenBuffers(1, &this->m_ContainerVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_ContainerVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(ColorVertex), containerVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
        reinterpret_cast<void*>(offsetof(ColorVertex, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
        reinterpret_cast<void*>(offsetof(ColorVertex, color)));

    glBindVertexArray(0);

    glGenVertexArrays(1, &this->m_OKButtonVAO);
    glBindVertexArray(this->m_OKButtonVAO);

    this->m_OKButton.SetWidth(100.0f);
    this->m_OKButton.SetHeight(50.0f);

    glm::vec2 buttonPosition;
    buttonPosition.x = this->m_ContainerBoundingRect.left + ((this->m_Container.GetWidth()) / 2.0f);
    buttonPosition.y = this->m_ContainerBoundingRect.top + (this->m_Container.GetHeight() - 30.0f);
    this->m_OKButton.SetPosition(buttonPosition);

    this->m_OKButtonBoundingRect = this->m_OKButton.GetBoundingRectangle();

    float halfButtonWidth = 100.0f / 2.0f;
    float halfButtonHeight = 50.0f / 2.0f;

    ColorVertex OKButtonVertices[] =
    {
        { glm::vec3(-halfButtonWidth, -halfButtonHeight, 1.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f) },
        { glm::vec3(halfButtonWidth, -halfButtonHeight, 1.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f) },
        { glm::vec3(-halfButtonWidth, halfButtonHeight, 1.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f) },
        { glm::vec3(halfButtonWidth, halfButtonHeight, 1.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f) },
    };

    glGenBuffers(1, &this->m_OKButtonVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_OKButtonVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(ColorVertex), OKButtonVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
        reinterpret_cast<void*>(offsetof(ColorVertex, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
        reinterpret_cast<void*>(offsetof(ColorVertex, color)));

    glBindVertexArray(0);

    if(this->m_TextManager.Init() == false)
    {
        return false;
    }
    if(this->m_TextManager.LoadFontFaceFromFile("fonts/OpenSans-SemiboldItalic.ttf", 16) == false)
    {
        return false;
    }

    this->m_TextManager.SetViewProjMatrix(this->m_mtxViewProj);

    this->m_TextManager.SetShaderProgram(this->m_GlyphTexMapSP);

    this->m_TextManager.SetScaleX(this->m_ScaleX);
    this->m_TextManager.SetScaleY(this->m_ScaleY);

    return true;
}

floatRect Notification::GetButtonBoundingRect(void)
{
    return this->m_OKButtonBoundingRect;
}

void Notification::ReleaseMemory(void)
{
    glDeleteBuffers(1, &this->m_OKButtonVB);
    glDeleteBuffers(1, &this->m_ContainerVB);

    glDeleteVertexArrays(1, &this->m_OKButtonVAO);
    glDeleteVertexArrays(1, &this->m_ContainerVAO);

    this->m_TextManager.ReleaseObjects();
}
