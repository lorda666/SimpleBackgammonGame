#include "CheckerField.h"

CheckerField::CheckerField(void)
{
    this->m_NumberOfCheckers = 0;
    this->m_Color = 0;
    this->m_Orientation = 1;
}

CheckerField::~CheckerField(void)
{
    //dtor
}

void CheckerField::AddChecker(const int color)
{
    Checker checker;
    checker.SetColor(color);

    glm::vec2 checkerPosition;
    checkerPosition.x = this->m_BasePosition.x;

    if(this->m_NumberOfCheckers == 0)
    {
        this->m_Color = color;
    }

    if((this->m_NumberOfCheckers % 5) == 0)
    {
        if((this->m_NumberOfCheckers / 5) % 2 == 0)
        {
            checkerPosition.y = this->m_BasePosition.y + (32.0f * this->m_Orientation);
        }
        else
        {
            checkerPosition.y = this->m_BasePosition.y + (64.0f * this->m_Orientation);
        }
    }
    else
    {
        glm::vec2 lastCheckerPosition = this->m_Checkers[this->m_NumberOfCheckers - 1].GetPosition();
        checkerPosition.y = lastCheckerPosition.y + (64.0f * this->m_Orientation);
    }

    checker.SetPosition(checkerPosition);
    this->m_Checkers.push_back(checker);
    this->m_NumberOfCheckers ++;
}

void CheckerField::RemoveChecker(void)
{
    if(this->m_NumberOfCheckers > 0)
    {
        this->m_Checkers.pop_back();
        this->m_NumberOfCheckers --;
    }
}

void CheckerField::SetBasePosition(const glm::vec2 basePosition)
{
    this->m_BasePosition = basePosition;
}

void CheckerField::SetNumberOfCheckers(const size_t numberOfCheckers)
{
    this->m_NumberOfCheckers = numberOfCheckers;
}

void CheckerField::SetOrientation(const int orientation)
{
    this->m_Orientation = orientation;
}

void CheckerField::SetColor(const int color)
{
    this->m_Color = color;
}

void CheckerField::SetCheckers(vector<Checker> checkers)
{
    this->m_Checkers = checkers;
}

void CheckerField::SetOrigin(const glm::vec2 origin)
{
    this->m_Origin = origin;
}

const glm::vec2 CheckerField::GetBasePosition(void)
{
    return this->m_BasePosition;
}

const size_t CheckerField::GetNumberOfCheckers(void)
{
    size_t numberOfCheckers = this->m_Checkers.size();

    return numberOfCheckers;
}

const int CheckerField::GetOrientation(void)
{
    return this->m_Orientation;
}

const int CheckerField::GetColor(void)
{
    return this->m_Color;
}

Checker CheckerField::GetTopChecker(void)
{
    return this->m_Checkers[this->m_NumberOfCheckers - 1];
}

const glm::vec2 CheckerField::GetTopCheckerOrigin(void)
{
    glm::vec2 checkerPosition;

    if(this->m_NumberOfCheckers > 0)
    {
        checkerPosition = this->m_Checkers[this->m_NumberOfCheckers - 1].GetPosition();
    }
    else
    {
        checkerPosition = this->m_BasePosition;
    }
    return checkerPosition;
}

vector<Checker> CheckerField::GetCheckers(void)
{
    return this->m_Checkers;
}

floatRect CheckerField::GetBoundingRectangle(void)
{
    this->m_BoundingRectangle.left = this->m_BasePosition.x - 32.0f;
    this->m_BoundingRectangle.right = this->m_BasePosition.x + 32.0f;

    if(this->m_Orientation == 1)
    {
        this->m_BoundingRectangle.top = this->m_BasePosition.y;
        this->m_BoundingRectangle.bottom = this->m_BasePosition.y + 360.0f;
    }
    else if(this->m_Orientation == -1)
    {
        this->m_BoundingRectangle.top = this->m_BasePosition.y - 360.0f;
        this->m_BoundingRectangle.bottom = this->m_BasePosition.y;
    }

    return this->m_BoundingRectangle;
}

const glm::vec2 CheckerField::GetOrigin(void)
{
    m_Origin.x = m_BasePosition.x;
    m_Origin.y = m_BasePosition.y + (180.0f * m_Orientation);

    return this->m_Origin;
}
