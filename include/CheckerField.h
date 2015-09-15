#pragma once
#include "Checker.h"
//#include <glm/glm.hpp>

class CheckerField
{
    public:
        CheckerField(void);
        virtual ~CheckerField(void);

        void AddChecker(const int color);

        void RemoveChecker(void);

        // SETTERS >>
        void SetBasePosition(const glm::vec2 basePosition);

        void SetNumberOfCheckers(const size_t numberOfCheckers);

        void SetOrientation(const int orientation);

        void SetColor(const int color);

        void SetCheckers(vector<Checker> checkers);

        void SetOrigin(const glm::vec2 origin);
        // << SETTERS

        // GETTERS >>
        const glm::vec2 GetBasePosition(void);

        const size_t GetNumberOfCheckers(void);

        const int GetOrientation(void);

        const int GetColor(void);

        Checker GetTopChecker(void);

        const glm::vec2 GetTopCheckerOrigin(void);

        vector<Checker> GetCheckers(void);

        floatRect GetBoundingRectangle(void);

        const glm::vec2 GetOrigin(void);
        // << GETTERS

    protected:
    private:
        glm::vec2           m_BasePosition;
        size_t              m_NumberOfCheckers;
        int                 m_Orientation;
        int                 m_Color; // 0 = black; 1 = white
        vector<Checker>     m_Checkers;
        floatRect           m_BoundingRectangle;
        glm::vec2           m_Origin;
};
