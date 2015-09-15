#pragma once

#include "GLBase.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Die.h"
#include "Checker.h"
#include "Player.h"
#include "CheckerField.h"
#include "Notification.h"

using namespace std;

class Game : public GLBase
{
    public:
        Game(void);
        virtual ~Game(void);

        virtual bool LoadContent(void) override;

        virtual void Update(float deltaTime) override;

        virtual void Render(void) override;

        virtual void OnResize(int width, int height) override;

        virtual void ReleaseObjects(void) override;
    protected:
    private:
        GLuint      m_BoardVAO;
        GLuint      m_BoardVB;
        GLuint      m_DieVAO;
        GLuint      m_DieVB;
        GLuint      m_RollDiceBtnVAO;
        GLuint      m_RollDiceBtnVB;
        GLuint      m_CheckerVAO;
        GLuint      m_CheckerVB;
        GLuint      m_CheckerHighlightVAO;
        GLuint      m_CheckerHighlightVB;
        GLuint      m_CheckerInStackVAO;
        GLuint      m_CheckerInStackVB;

        TextureManager      m_TextureManager;
        GLuint              m_BoardTexture;
        GLuint              m_DieTexture;
        GLuint              m_RollDiceBtnTexture;
        GLuint              m_RollDiceBtnPressedTexture;
        GLuint              m_WhiteCheckerTexture;
        GLuint              m_BlackCheckerTexture;
        GLuint              m_WhiteCheckerInStackTexture;
        GLuint              m_BlackCheckerInStackTexture;

        ShaderManager   m_VertexShader;
        ShaderManager   m_FragmentShader;
        GLuint          m_ShaderProgram;
        GLuint          m_ColorSP;
        GLuint          m_GlyphTexMapSP;

        Player      m_FirstPlayer; // white checkers
        Player      m_SecondPlayer; // black checkers

        Sprite          m_Board;
        Die             m_Dice[2];
        Sprite          m_RollDiceBtn;
        CheckerField    m_Fields[24];
        Sprite          m_FieldContour;
        Sprite          m_CheckerHighlight;
        Checker         m_PickedChecker;

        floatRect   m_WhiteCheckerStackField;

        Notification    m_Notification;

        vector<short> m_BlackFields;
        vector<short> m_WhiteFields;

        glm::mat4   m_mtxView;
        glm::mat4   m_mtxProj;
        glm::mat4   m_mtxViewProj;
        glm::mat4   m_mtxWorld;
        glm::mat4   m_mtxWorldViewProj;

        bool    m_GameStarted;
        bool    m_GameEnded;
        bool    m_RollDicePressed;
        bool    m_TurnEnded;
        bool    m_IsMovingChecker;
        bool    m_IsCheckerPicked;
        bool    m_IsNotificationShown;

        int     m_DiceRollings;
        int     m_CurrentPlayer; // 0 - black; 1 - white
        int     m_FirstDieIndex;
        int     m_SecondDieIndex;
        int     m_MaxNumberOfMoves;
        int     m_MovesMade;
        int     m_UsedDice[6];
        int     m_SourceField;
        int     m_State;

        int     m_MoveAction;

        // USED BY AI >>
        int destinationField;
        glm::vec2 startPosition;
        glm::vec2 currentCheckerPosition;
        glm::vec2 targetPosition;
        glm::vec2 direction;
        float distance;
        float currentDistance;
        // << USED BY AI

        void BuildBuffers(void);

        bool DefineTextures(void);

        bool ManageShaders(void);

        bool LinkShaderProgram(GLuint programId);

        void InitFields(void);

        void InitCheckers(void);

        void DefineVertexObjectData(void);

        inline bool IsCursorOverRectangle(glm::vec2 cursorPosition, floatRect rectangle);

        inline bool IsCursorOverCircle(glm::vec2 cursorPosition, glm::vec2 circleCenter);

        inline void RollDice(void);

        inline int CanMoveChecker(const int fieldIndex, const int color);

        void PlaceChecker(const int color, const int fieldIndex, const int action);

        inline void UseDie(const int dieIndex, const int state);

        inline void Reset(void);

        // DRAW METHODS >>
        inline void DrawBoard(void);

        inline void DrawRollDiceButton(void);

        inline void DrawDice(void);

        inline void DrawPickedChecker(void);
        // << DRAW METHODS
};
