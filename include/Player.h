#pragma once

#include "Checker.h"
#include "CheckerField.h"

class Player
{
    public:
        Player(void);
        virtual ~Player(void);

        void SetColor(const int color);

        // GETTERS >>
        const int GetColor(void);

        vector<Checker> GetLostCheckers(void);

        vector<Checker> GetCheckerStack(void);

        glm::vec2 GetTopLostCheckerPosition(void);

        glm::vec2 GetPositionOfTopCheckerInStack(void);

        vector<short> GetOccupiedFields(void);
        // << GETTERS

        void AddLostChecker(void);

        void RemoveLostChecker(void);

        void AddOccupiedField(const short fieldIndex);

        void RemoveOccupiedField(const short fieldIndex);

        bool HasPossibleMove(CheckerField* fields, const int dieIndex);

        void AddCheckerInStack(void);

        bool AreAllCheckersHome(void);

        void ClearCheckerStack(void);

        void ClearLostCheckers(void);

        void ClearOccupiedFields(void);

    protected:
    private:
        vector<Checker>     m_LostCheckers;
        vector<Checker>     m_CheckerStack;
        int                 m_Color;
        vector<short>       m_OccupiedFields;
        glm::vec2           m_LostCheckersBasePosition;

        bool CanPutCheckerIntoGame(CheckerField* fields, const int dieIndex);
};
