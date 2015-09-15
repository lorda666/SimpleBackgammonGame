#pragma once

#include "Util.h"
#include "TextureManager.h"

class Sprite
{
    public:
        Sprite(void);
        Sprite(glm::vec2 position);
        virtual ~Sprite(void);

        // SETTERS >>
        void SetPosition(const glm::vec2 position);

        void SetRotationAngle(const float rotationAngle);

        void SetScale(const glm::vec2 scale);

        void SetWidth(const float width);

        void SetHeight(const float height);
        // << SETTERS

        // GETTERS >>
        const glm::vec2 GetPosition(void);

        const glm::vec2 GetScale(void);

        const float GetRotationAngle(void);

        const float GetWidth(void);

        const float GetHeight(void);

        const floatRect GetBoundingRectangle(void);
        // << GETTERS

        glm::mat4 GetModelMatrix(void);

    private:
        glm::vec2   m_Position;
        glm::vec2   m_Scale;
        float       m_RotationAngle;
        float       m_Width;
        float       m_Height;
        floatRect   m_BoundingRect;
};
