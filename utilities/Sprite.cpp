#include "Sprite.h"

Sprite::Sprite()
{
    m_Position = glm::vec2(0.0f, 0.0f);
    m_RotationAngle = 0.0f;
    m_Scale = glm::vec2(1.0f, 1.0f);
}

Sprite::~Sprite()
{
    //dtor
}

void Sprite::SetPosition(const glm::vec2 position)
{
    this->m_Position = position;
}

void Sprite::SetRotationAngle(const float rotationAngle)
{
    this->m_RotationAngle = rotationAngle;
}

void Sprite::SetScale(const glm::vec2 scale)
{
    this->m_Scale = scale;
}

void Sprite::SetWidth(const float width)
{
    this->m_Width = width;
}

void Sprite::SetHeight(const float height)
{
    this->m_Height = height;
}

const glm::vec2 Sprite::GetPosition(void)
{
    return this->m_Position;
}

const glm::vec2 Sprite::GetScale(void)
{
    return this->m_Scale;
}

const float Sprite::GetRotationAngle(void)
{
    return this->m_RotationAngle;
}

const float Sprite::GetWidth(void)
{
    return this->m_Width;
}

const float Sprite::GetHeight(void)
{
    return this->m_Height;
}

const floatRect Sprite::GetBoundingRectangle(void)
{
    float halfWidth = this->m_Width / 2.0f;
    float halfHeight = this->m_Height / 2.0f;

    this->m_BoundingRect.top = this->m_Position.y - halfHeight;
    this->m_BoundingRect.left = this->m_Position.x - halfWidth;

    this->m_BoundingRect.bottom = this->m_Position.y + halfHeight;
    this->m_BoundingRect.right = this->m_Position.x + halfWidth;

    return this->m_BoundingRect;
}

glm::mat4 Sprite::GetModelMatrix(void)
{
    glm::mat4 mtxRotation = glm::rotate(glm::radians(this->m_RotationAngle),
                                    glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 mtxTranslation = glm::translate(glm::vec3(this->m_Position.x,
                                        this->m_Position.y, 0.0f));
    glm::mat4 mtxScale = glm::scale(glm::vec3(this->m_Scale.x,
                                    this->m_Scale.y, 1.0f));

    return (mtxTranslation * mtxScale * mtxRotation);
}
