#pragma once

#include "Sprite.h"
#include "TextManager.h"

class Notification
{
    public:
        Notification(void);
        virtual ~Notification(void);

        const string GetNotificationMessage(void);

        floatRect GetButtonBoundingRect(void);

        // SETTERS >>
        void SetNotificationMessage(const string message);

        void SetPosition(const glm::vec2 position);

        void SetWidth(const float width);

        void SetHeight(const float height);

        void SetViewProjMatrix(const glm::mat4 mtxViewProj);

        void SetShaderProgram(const GLuint shaderProgramId);

        void SetGlyphShaderProgram(const GLuint shaderProgramId);

        void SetScale(const float scaleX, const float scaleY);
        // << SETTERS

        void Show(void);

        bool Init(void);

        void ReleaseMemory(void);

    protected:
    private:
        string      m_NotificationMessage;
        Sprite      m_Container;
        Sprite      m_OKButton;

        floatRect   m_ContainerBoundingRect;
        floatRect   m_OKButtonBoundingRect;

        TextManager     m_TextManager;

        GLuint  m_ContainerVAO;
        GLuint  m_ContainerVB;
        GLuint  m_OKButtonVAO;
        GLuint  m_OKButtonVB;

        GLuint  m_ColorSP;
        GLuint  m_GlyphTexMapSP;

        glm::mat4   m_mtxViewProj;

        float m_ScaleX;
        float m_ScaleY;
};
