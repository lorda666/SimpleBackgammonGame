#include "Player.h"
#include <algorithm>

Player::Player(void)
{
    //ctor
}

Player::~Player(void)
{
    //dtor
}

void Player::SetColor(const int color)
{
    this->m_Color = color;
}

const int Player::GetColor(void)
{
    return this->m_Color;
}

vector<Checker> Player::GetLostCheckers(void)
{
    return this->m_LostCheckers;
}

vector<Checker> Player::GetCheckerStack(void)
{
    return this->m_CheckerStack;
}

glm::vec2 Player::GetTopLostCheckerPosition(void)
{
    size_t numberOfLostCheckers = this->m_LostCheckers.size();
    if(numberOfLostCheckers > 0)
    {
        return this->m_LostCheckers[numberOfLostCheckers - 1].GetPosition();
    }
}

glm::vec2 Player::GetPositionOfTopCheckerInStack(void)
{
    glm::vec2 checkerPosition;

    size_t numberOfCheckersInStack = this->m_CheckerStack.size();
    if(numberOfCheckersInStack == 0)
    {
        if(this->m_Color == 0)
        {
            return glm::vec2(992.0f, 10.0f);
        }
        else
        {
            return glm::vec2(992.0f, 774.0f);
        }
    }
    else
    {
        checkerPosition = this->m_CheckerStack[numberOfCheckersInStack - 1].GetPosition();
    }

    return checkerPosition;
}

vector<short> Player::GetOccupiedFields(void)
{
    return this->m_OccupiedFields;
}

void Player::AddLostChecker(void)
{
    this->m_LostCheckersBasePosition.x = 480.0f;
    if(this->m_Color == 0)
    {
        this->m_LostCheckersBasePosition.y = 684.0f;
    }
    else
    {
        this->m_LostCheckersBasePosition.y = 100.0f;
    }

    Checker lostChecker;
    int numberOfLostCheckers = this->m_LostCheckers.size();
    if(numberOfLostCheckers == 0)
    {
        lostChecker.SetPosition(this->m_LostCheckersBasePosition);
    }
    else
    {
        glm::vec2 topLostCheckerPosition;
        topLostCheckerPosition = this->m_LostCheckers[numberOfLostCheckers - 1].GetPosition();

        if(this->m_Color == 0)
        {
            topLostCheckerPosition.y -= 20.0f;
        }
        else
        {
            topLostCheckerPosition.y += 20.0f;
        }
        lostChecker.SetPosition(topLostCheckerPosition);
    }

    this->m_LostCheckers.push_back(lostChecker);
}

void Player::RemoveLostChecker(void)
{
    this->m_LostCheckers.pop_back();
}

void Player::AddOccupiedField(const short fieldIndex)
{
    this->m_OccupiedFields.push_back(fieldIndex);
    if(this->m_Color == 0)
    {
        sort(this->m_OccupiedFields.begin(), this->m_OccupiedFields.end(), greater<short>());
    }
    else
    {
        sort(this->m_OccupiedFields.begin(), this->m_OccupiedFields.end());
    }
}

void Player::RemoveOccupiedField(const short fieldIndex)
{
    vector<short>::iterator it;
    it = find(this->m_OccupiedFields.begin(), this->m_OccupiedFields.end(), fieldIndex);

    if(it != this->m_OccupiedFields.end())
    {
        this->m_OccupiedFields.erase(it);
    }
}

bool Player::HasPossibleMove(CheckerField* fields, const int dieIndex)
{
    int destination;
    int i, numberOfPossibleMoves;
    if(CanPutCheckerIntoGame(fields, dieIndex) == false)
    {
        return false;
    }
    GLsizei numberOfOccupiedFields = this->m_OccupiedFields.size();
    if(this->m_Color == 1)
    {
        numberOfPossibleMoves = 0;
        for(i = 0; i < numberOfOccupiedFields; i++)
        {
            destination = this->m_OccupiedFields[i] + (dieIndex + 1);
            if(destination < 24)
            {
                if(fields[destination].GetNumberOfCheckers() == 0)
                {
                    numberOfPossibleMoves ++;
                }
                else if((fields[destination].GetNumberOfCheckers() == 1) &&
                        (fields[destination].GetColor() != this->m_Color))
                {
                    numberOfPossibleMoves ++;
                }
                else if((fields[destination].GetNumberOfCheckers() >= 1) &&
                        (fields[destination].GetColor() == this->m_Color))
                {
                    numberOfPossibleMoves ++;
                }
            }
            else if((destination >= 24) && (AreAllCheckersHome() == true))
            {
                numberOfPossibleMoves ++;
            }
        }
    }
    else if(this->m_Color == 0)
    {
        numberOfPossibleMoves = 0;
        for(i = 0; i < numberOfOccupiedFields; i++)
        {
            destination = this->m_OccupiedFields[i] - (dieIndex + 1);
            if(destination >= 0)
            {
                if(fields[destination].GetNumberOfCheckers() == 0)
                {
                    numberOfPossibleMoves ++;
                }
                else if((fields[destination].GetNumberOfCheckers() == 1) &&
                        (fields[destination].GetColor() != this->m_Color))
                {
                    numberOfPossibleMoves ++;
                }
                else if((fields[destination].GetNumberOfCheckers() >= 1) &&
                        (fields[destination].GetColor() == this->m_Color))
                {
                    numberOfPossibleMoves ++;
                }
            }
            else if((destination < 0) && (AreAllCheckersHome() == true))
            {
                numberOfPossibleMoves ++;
            }
        }
    }

    return (numberOfPossibleMoves > 0);
}

void Player::AddCheckerInStack(void)
{
    Checker checker;
    glm::vec2 checkerPosition;
    checkerPosition.x = 992.0f;

    int direction = 1;
    if(this->m_Color == 1)
    {
        direction = -1;
    }
    size_t numberOfCheckersInStack = this->m_CheckerStack.size();

    if(numberOfCheckersInStack == 0)
    {
        if(this->m_Color == 0)
        {
            checkerPosition.y = 10.0f;
        }
        else
        {
            checkerPosition.y = 774.0f;
        }
    }
    else
    {
        Checker topChecker = this->m_CheckerStack[numberOfCheckersInStack - 1];
        checkerPosition.y = topChecker.GetPosition().y + (20.0f * direction);
    }

    checker.SetColor(this->m_Color);
    checker.SetPosition(checkerPosition);
    this->m_CheckerStack.push_back(checker);
}

bool Player::AreAllCheckersHome(void)
{
    GLsizei numberOfOccupiedFields = this->m_OccupiedFields.size();
    int i;
    if(this->m_Color == 1)
    {
        for(i = 0; i < numberOfOccupiedFields; i++)
        {
            if(this->m_OccupiedFields[i] < 18)
            {
                return false;
            }
        }
    }
    else if(this->m_Color == 0)
    {
        for(i = 0; i < numberOfOccupiedFields; i++)
        {
            if(this->m_OccupiedFields[i] > 5)
            {
                return false;
            }
        }
    }

    return true;
}

void Player::ClearCheckerStack(void)
{
    this->m_CheckerStack.clear();
}

void Player::ClearLostCheckers(void)
{
    this->m_LostCheckers.clear();
}

void Player::ClearOccupiedFields(void)
{
    this->m_OccupiedFields.clear();
}

bool Player::CanPutCheckerIntoGame(CheckerField* fields, const int dieIndex)
{
    GLsizei numberOfLostCheckers = this->m_LostCheckers.size();
    int destination;
    //int i;
    if(numberOfLostCheckers > 0)
    {
        if(this->m_Color == 1)
        {
            destination = dieIndex;
        }
        else if(this->m_Color == 0)
        {
            destination = 23 - dieIndex;
        }

        if((fields[destination].GetNumberOfCheckers() > 1) &&
           (fields[destination].GetColor() != this->m_Color))
        {
            return false;
        }
    }

    return true;
}
