#include "Game.h"
//#include "Util.h"
#include <cstdlib>
#include <ctime>

/*struct Vertex
{
    glm::vec3 position;
    glm::vec2 texUV;
};*/

/*struct ColorVertex
{
    glm::vec3 position;
    glm::vec4 color;
};*/

enum CheckerColor
{
    BLACK = 0,
    WHITE = 1,
};

Game::Game(void)
{
    m_appInfo.windowTitle = "Backgammon";
    m_appInfo.windowWidth = 1024;
    m_appInfo.windowHeight = 784;
    m_appInfo.isResizable = GL_FALSE;

    this->m_DiceRollings = 0;
    this->m_FirstDieIndex = 0;
    this->m_SecondDieIndex = 0;
    this->m_MaxNumberOfMoves = 0;
    this->m_MovesMade = 0;
    this->m_SourceField = 0;
    this->m_MoveAction = -1;
    this->m_State = 0;

    this->m_CurrentPlayer = CheckerColor::WHITE;

    this->m_GameStarted = false;
    this->m_RollDicePressed = false;
    this->m_TurnEnded = true;
    this->m_IsMovingChecker = false;
    for(int i = 0; i < 6; i++)
    {
        this->m_UsedDice[i] = 0;
    }
    this->m_IsCheckerPicked = false;
    this->m_IsNotificationShown = false;
    this->m_GameEnded = false;
}

Game::~Game(void)
{
    //dtor
}

void Game::BuildBuffers(void)
{
    // BOARD VAO >>
    glGenVertexArrays(1, &this->m_BoardVAO);
    glBindVertexArray(this->m_BoardVAO);

    float boardWidth = (float) this->m_Board.GetWidth();
    float boardHeight = (float) this->m_Board.GetHeight();

    float halfBoardWidth = boardWidth / 2.0f;
    float halfBoardHeight = boardHeight / 2.0f;

    Vertex boardVertices[] =
    {
        { glm::vec3(-halfBoardWidth, -halfBoardHeight, 0.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(halfBoardWidth, -halfBoardHeight, 0.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-halfBoardWidth, halfBoardHeight, 0.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(halfBoardWidth, halfBoardHeight, 0.0f), glm::vec2(1.0f, 1.0f) },
    };
    glGenBuffers(1, &this->m_BoardVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_BoardVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), boardVertices, GL_STATIC_DRAW);

    DefineVertexObjectData();
    glBindVertexArray(0);
    // << BOARD VAO

    // CHECKER VAO >>
    glGenVertexArrays(1, &this->m_CheckerVAO);
    glBindVertexArray(this->m_CheckerVAO);

    float halfCheckerWidth = 64.0f / 2.0f;
    float halfCheckerHeight = 64.0f / 2.0f;

    Vertex checkerVertices[] =
    {
        { glm::vec3(-halfCheckerWidth, -halfCheckerHeight, 1.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(halfCheckerWidth, -halfCheckerHeight, 1.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-halfCheckerWidth, halfCheckerHeight, 1.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(halfCheckerWidth, halfCheckerHeight, 1.0f), glm::vec2(1.0f, 1.0f) },
    };

    glGenBuffers(1, &this->m_CheckerVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_CheckerVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), checkerVertices, GL_STATIC_DRAW);

    DefineVertexObjectData();
    glBindVertexArray(0);
    // << CHECKER VAO

    // DIE VAO >>
    glGenVertexArrays(1, &this->m_DieVAO);
    glBindVertexArray(this->m_DieVAO);

    float halfDieWidth = 64.0f / 2.0f;
    float halfDieHeight = 64.0f / 2.0f;

    Vertex dieVertices[] =
    {
        { glm::vec3(-halfDieWidth, -halfDieHeight, 1.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(halfDieWidth, -halfDieHeight, 1.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-halfDieWidth, halfDieHeight, 1.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(halfDieWidth, halfDieHeight, 1.0f), glm::vec2(1.0f, 1.0f) },
    };

    glGenBuffers(1, &this->m_DieVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_DieVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), dieVertices, GL_STATIC_DRAW);

    DefineVertexObjectData();
    glBindVertexArray(0);
    // << DIE VAO

    // ROLL DICE BUTTON VAO >>
    glGenVertexArrays(1, &this->m_RollDiceBtnVAO);
    glBindVertexArray(this->m_RollDiceBtnVAO);

    float halfRollDiceBtnWidth = this->m_RollDiceBtn.GetWidth() / 2.0f;
    float halfRollDiceBtnHeight = this->m_RollDiceBtn.GetHeight() / 2.0f;

    Vertex rollDiceBtnVertices[] =
    {
        { glm::vec3(-halfRollDiceBtnWidth, -halfRollDiceBtnHeight, 1.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(halfRollDiceBtnWidth, -halfRollDiceBtnHeight, 1.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-halfRollDiceBtnWidth, halfRollDiceBtnHeight, 1.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(halfRollDiceBtnWidth, halfRollDiceBtnHeight, 1.0f), glm::vec2(1.0f, 1.0f) },
    };

    glGenBuffers(1, &this->m_RollDiceBtnVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_RollDiceBtnVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), rollDiceBtnVertices, GL_STATIC_DRAW);

    DefineVertexObjectData();
    glBindVertexArray(0);
    // << ROLL DICE BUTTON VAO

    // FIELD CONTOUR VAO >>
    glGenVertexArrays(1, &this->m_FieldContourVAO);
    glBindVertexArray(this->m_FieldContourVAO);

    ColorVertex fieldContourVertices[] =
    {
        { glm::vec3(-32.0f, -180.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) },
        { glm::vec3(32.0f, -180.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) },
        { glm::vec3(32.0f, 180.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) },
        { glm::vec3(-32.0f, 180.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) },
    };

    glGenBuffers(1, &this->m_FieldContourVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_FieldContourVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(ColorVertex), fieldContourVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
                          reinterpret_cast<void*>(offsetof(ColorVertex, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
                          reinterpret_cast<void*>(offsetof(ColorVertex, color)));

    glBindVertexArray(0);
    // << FIELD CONTOUR VAO

    // CHECKER CONTOUR VAO >>
    glGenVertexArrays(1, &this->m_CheckerHighlightVAO);
    glBindVertexArray(this->m_CheckerHighlightVAO);

    ColorVertex checkerHighlightVertices[182];
    checkerHighlightVertices[0].position = glm::vec3(0.0f, 0.0f, 1.0f);
    checkerHighlightVertices[0].color = glm::vec4(1.0f, 0.0f, 1.0f, 0.3f);

    int i;
    float theta, radius = 32.0f;
    for(i = 1; i <= 360; i++)
    {
        theta = 2.0f * 3.1415926f * (float)i / 180.0f;
        checkerHighlightVertices[i].position.x = radius * glm::cos(theta);
        checkerHighlightVertices[i].position.y = radius * glm::sin(theta);
        checkerHighlightVertices[i].position.z = 1.0f;

        checkerHighlightVertices[i].color = glm::vec4(0.8f, 0.0f, 0.8f, 0.5f);
    }

    glGenBuffers(1, &this->m_CheckerHighlightVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_CheckerHighlightVB);
    glBufferData(GL_ARRAY_BUFFER, 182 * sizeof(ColorVertex), checkerHighlightVertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
                          reinterpret_cast<void*>(offsetof(ColorVertex, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex),
                          reinterpret_cast<void*>(offsetof(ColorVertex, color)));

    glBindVertexArray(0);
    // << CHECKER CONTOUR VAO

    // CHECKER IN STACK VAO >>
    glGenVertexArrays(1, &this->m_CheckerInStackVAO);
    glBindVertexArray(this->m_CheckerInStackVAO);

    Vertex checkerInStackVertices[] =
    {
        { glm::vec3(-32.0f, -10.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(32.0f, -10.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-32.0f, 10.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(32.0f, 10.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
    };

    glGenBuffers(1, &this->m_CheckerInStackVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_CheckerInStackVB);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), checkerInStackVertices, GL_STATIC_DRAW);

    DefineVertexObjectData();

    glBindVertexArray(0);
    // << CHECKER IN STACK VAO
}

bool Game::DefineTextures(void)
{
    ilInit();

    // BOARD TEXTURE >>
    if(this->m_TextureManager.LoadTextureImage("textures/board.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_BoardTexture = this->m_TextureManager.GetTextureId();
    this->m_Board.SetWidth(this->m_TextureManager.GetImageWidth());
    this->m_Board.SetHeight(this->m_TextureManager.GetImageHeight());
    // << BOARD TEXTURE

    // DIE TEXTURE >>
    if(this->m_TextureManager.LoadTextureImage("textures/dice.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_DieTexture = this->m_TextureManager.GetTextureId();
    this->m_Dice[0].SetWidth(this->m_TextureManager.GetImageWidth());
    this->m_Dice[0].SetHeight(this->m_TextureManager.GetImageHeight());
    // << DIE TEXTURE

    // ROLL DICE BUTTON TEXTURES >>
    if(this->m_TextureManager.LoadTextureImage("textures/rollDiceButton.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_RollDiceBtnTexture = this->m_TextureManager.GetTextureId();
    this->m_RollDiceBtn.SetWidth(this->m_TextureManager.GetImageWidth());
    this->m_RollDiceBtn.SetHeight(this->m_TextureManager.GetImageHeight());

    if(this->m_TextureManager.LoadTextureImage("textures/rollDiceButtonPressed.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_RollDiceBtnPressedTexture = this->m_TextureManager.GetTextureId();
    // << ROLL DICE BUTTON TEXTURES

    // CHECKER TEXTURES >>
    if(this->m_TextureManager.LoadTextureImage("textures/whiteChecker.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_WhiteCheckerTexture = this->m_TextureManager.GetTextureId();

    if(this->m_TextureManager.LoadTextureImage("textures/blackChecker.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_BlackCheckerTexture = this->m_TextureManager.GetTextureId();
    // << CHECKER TEXTURES

    // CHECKER IN STACK TEXTURES >>
    if(this->m_TextureManager.LoadTextureImage("textures/whiteCheckerInStack.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_WhiteCheckerInStackTexture = this->m_TextureManager.GetTextureId();

    if(this->m_TextureManager.LoadTextureImage("textures/blackCheckerInStack.dds") == false)
    {
        return false;
    }
    if(this->m_TextureManager.CreateTexture() == false)
    {
        return false;
    }

    this->m_BlackCheckerInStackTexture = this->m_TextureManager.GetTextureId();
    // << CHECKER IN STACK TEXTURES

    return true;
}

bool Game::ManageShaders(void)
{
    if(this->m_VertexShader.LoadShader(GL_VERTEX_SHADER, "shaders/TextureMap.vsh") == false)
    {
        return false;
    }
    if(this->m_VertexShader.CompileShader() == false)
    {
        return false;
    }
    if(this->m_FragmentShader.LoadShader(GL_FRAGMENT_SHADER, "shaders/TextureMap.fsh") == false)
    {
        return false;
    }
    if(this->m_FragmentShader.CompileShader() == false)
    {
        return false;
    }

    this->m_ShaderProgram = glCreateProgram();
    glAttachShader(this->m_ShaderProgram, this->m_VertexShader.GetShader());
    glAttachShader(this->m_ShaderProgram, this->m_FragmentShader.GetShader());

    glDeleteShader(this->m_VertexShader.GetShader());
    glDeleteShader(this->m_FragmentShader.GetShader());

    if(this->m_VertexShader.LoadShader(GL_VERTEX_SHADER, "shaders/ColorContour.vsh") == false)
    {
        return false;
    }
    if(this->m_VertexShader.CompileShader() == false)
    {
        return false;
    }
    if(this->m_FragmentShader.LoadShader(GL_FRAGMENT_SHADER, "shaders/ColorContour.fsh") == false)
    {
        return false;
    }
    if(this->m_FragmentShader.CompileShader() == false)
    {
        return false;
    }

    this->m_ColorSP = glCreateProgram();
    glAttachShader(this->m_ColorSP, this->m_VertexShader.GetShader());
    glAttachShader(this->m_ColorSP, this->m_FragmentShader.GetShader());

    glDeleteShader(this->m_VertexShader.GetShader());
    glDeleteShader(this->m_FragmentShader.GetShader());

    if(this->m_VertexShader.LoadShader(GL_VERTEX_SHADER, "shaders/TextureMap.vsh") == false)
    {
        return false;
    }
    if(this->m_VertexShader.CompileShader() == false)
    {
        return false;
    }
    if(this->m_FragmentShader.LoadShader(GL_FRAGMENT_SHADER, "shaders/GlyphTexMap.fsh") == false)
    {
        return false;
    }
    if(this->m_FragmentShader.CompileShader() == false)
    {
        return false;
    }

    this->m_GlyphTexMapSP = glCreateProgram();
    glAttachShader(this->m_GlyphTexMapSP, this->m_VertexShader.GetShader());
    glAttachShader(this->m_GlyphTexMapSP, this->m_FragmentShader.GetShader());

    return true;
}

bool Game::LinkShaderProgram(GLuint programId)
{
    glLinkProgram(programId);

    GLint programLinkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &programLinkStatus);
    if(programLinkStatus == GL_FALSE)
    {
        GLint logSize;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
        GLchar* errorLog = nullptr;
        glGetProgramInfoLog(programId, logSize, &logSize, errorLog);

        fprintf(stderr, "FAILED TO LINK SHADER PROGRAM!\n Error log: %s\n",
                errorLog);
        return false;
    }

    return true;
}

void Game::InitFields(void)
{
    int i;
    glm::vec2 baseFieldPosition;
    baseFieldPosition.x = m_appInfo.windowWidth - 160.0f;
    baseFieldPosition.y = 32.0f;
    this->m_Fields[0].SetBasePosition(baseFieldPosition);
    this->m_Fields[0].SetOrientation(1); // points downwards

    glm::vec2 currentFieldPosition;
    for(i = 1; i < 12; i++)
    {
        currentFieldPosition = this->m_Fields[i - 1].GetBasePosition();

        if((i - 1) == 5)
        {
            currentFieldPosition.x -= 64.0f;
        }
        baseFieldPosition.x = currentFieldPosition.x - 64.0f;

        this->m_Fields[i].SetBasePosition(baseFieldPosition);
        this->m_Fields[i].SetOrientation(1);
    }

    baseFieldPosition.x = 96.0f;
    baseFieldPosition.y = m_appInfo.windowHeight - 32.0f;
    this->m_Fields[12].SetBasePosition(baseFieldPosition);
    this->m_Fields[12].SetOrientation(-1); // points upwards

    for(i = 13; i < 24; i++)
    {
        currentFieldPosition = this->m_Fields[i - 1].GetBasePosition();

        if((i - 1) == 17)
        {
            currentFieldPosition.x += 64.0f;
        }
        baseFieldPosition.x = currentFieldPosition.x + 64.0f;

        this->m_Fields[i].SetBasePosition(baseFieldPosition);
        this->m_Fields[i].SetOrientation(-1);
    }
}

void Game::InitCheckers(void)
{
    int i;

    for(i = 0; i < 2; i++)
    {
        this->m_Fields[0].AddChecker(CheckerColor::WHITE);
        this->m_Fields[23].AddChecker(CheckerColor::BLACK);
    }
    this->m_SecondPlayer.AddOccupiedField((short)23);
    this->m_FirstPlayer.AddOccupiedField((short)0);

    for(i = 0; i < 5; i++)
    {
        this->m_Fields[5].AddChecker(CheckerColor::BLACK);
        this->m_Fields[11].AddChecker(CheckerColor::WHITE);
        this->m_Fields[12].AddChecker(CheckerColor::BLACK);
        this->m_Fields[18].AddChecker(CheckerColor::WHITE);
    }
    this->m_SecondPlayer.AddOccupiedField((short)5);
    this->m_SecondPlayer.AddOccupiedField((short)12);
    this->m_FirstPlayer.AddOccupiedField((short)11);
    this->m_FirstPlayer.AddOccupiedField((short)18);

    for(i = 0; i < 3; i++)
    {
        this->m_Fields[7].AddChecker(CheckerColor::BLACK);
        this->m_Fields[16].AddChecker(CheckerColor::WHITE);
    }
    this->m_SecondPlayer.AddOccupiedField((short)7);
    this->m_FirstPlayer.AddOccupiedField((short)16);

    glm::vec2 checkerHighlightPosition;
    vector<short> whiteFields = this->m_FirstPlayer.GetOccupiedFields();
    checkerHighlightPosition = this->m_Fields[whiteFields[0]].GetTopCheckerOrigin();
    this->m_CheckerHighlight.SetPosition(checkerHighlightPosition);
}

void Game::DefineVertexObjectData(void)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, texUV)));
}

bool Game::IsCursorOverRectangle(glm::vec2 cursorPosition,
                                 floatRect rectangle)
{
    if((cursorPosition.x >= rectangle.left && cursorPosition.x <= rectangle.right) &&
       (cursorPosition.y >= rectangle.top && cursorPosition.y <= rectangle.bottom))
    {
        return true;
    }

    return false;
}

bool Game::IsCursorOverCircle(glm::vec2 cursorPosition, glm::vec2 circleCenter)
{
    float distance = glm::distance(cursorPosition, circleCenter);

    return (distance <= 32.0f);
}

bool Game::LoadContent(void)
{
    srand(time(NULL));

    glViewport(0, 0, m_appInfo.windowWidth, m_appInfo.windowHeight);

    if(DefineTextures() == false)
    {
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BuildBuffers();

    if(ManageShaders() == false)
    {
        return false;
    }

    glm::vec2 boardOrigin;
    boardOrigin.x = m_appInfo.windowWidth / 2.0f;
    boardOrigin.y = m_appInfo.windowHeight / 2.0f;

    this->m_Board.SetPosition(boardOrigin);

    glm::vec2 rollDiceBtnOrigin;
    rollDiceBtnOrigin.x = m_appInfo.windowWidth - (m_RollDiceBtn.GetWidth() / 2.0f);
    rollDiceBtnOrigin.y = m_appInfo.windowHeight / 2.0f;

    this->m_RollDiceBtn.SetPosition(rollDiceBtnOrigin);

    this->m_FirstPlayer.SetColor(CheckerColor::WHITE);
    this->m_SecondPlayer.SetColor(CheckerColor::BLACK);

    InitFields();

    InitCheckers();

    glm::vec2 dieOrigin;
    dieOrigin.y = m_appInfo.windowHeight / 2.0f;

    dieOrigin.x = 656.0f;
    this->m_Dice[0].SetPosition(dieOrigin);

    dieOrigin.x = 752.0f;
    this->m_Dice[1].SetPosition(dieOrigin);

    LinkShaderProgram(this->m_ShaderProgram);

    LinkShaderProgram(this->m_ColorSP);

    LinkShaderProgram(this->m_GlyphTexMapSP);

    this->m_mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    this->m_mtxProj = glm::ortho(0.0f, (float)m_appInfo.windowWidth, (float)m_appInfo.windowHeight, 0.0f,
                           0.01f, 100.0f);

    this->m_mtxViewProj = m_mtxProj * m_mtxView;

    this->m_Notification.SetWidth(400.0f);
    this->m_Notification.SetHeight(200.0f);

    glm::vec2 notificationPosition;
    notificationPosition.x = m_appInfo.windowWidth / 2.0f;
    notificationPosition.y = m_appInfo.windowHeight / 2.0;
    this->m_Notification.SetPosition(notificationPosition);

    this->m_Notification.SetViewProjMatrix(this->m_mtxViewProj);
    this->m_Notification.SetShaderProgram(this->m_ColorSP);
    this->m_Notification.SetGlyphShaderProgram(this->m_GlyphTexMapSP);

    float scaleX = 1.0f / m_appInfo.windowWidth;
    float scaleY = 1.0f / m_appInfo.windowHeight;
    this->m_Notification.SetScale(scaleX, scaleY);

    if(this->m_Notification.Init() == false)
    {
        return false;
    }

    glActiveTexture(GL_TEXTURE0);

    this->m_FirstDieIndex = rand() % 6;
    this->m_SecondDieIndex = rand() % 6;

    this->m_State = 0;
    this->m_CurrentPlayer = CheckerColor::WHITE;

    this->m_WhiteCheckerStackField.top = 456.0f;
    this->m_WhiteCheckerStackField.left = 960.0f;
    this->m_WhiteCheckerStackField.bottom = 784.0f;
    this->m_WhiteCheckerStackField.right = 1024.0f;

    return true;
}

void Game::RollDice(void)
{
    this->m_FirstDieIndex = rand() % 6;
    this->m_SecondDieIndex = rand() % 6;
}

int Game::CanMoveChecker(const int fieldIndex, const int color)
{
    if((fieldIndex < 0) && (this->m_CurrentPlayer == CheckerColor::BLACK) &&
       (this->m_SecondPlayer.AreAllCheckersHome() == true))
    {
        return 3;
    }
    size_t numberOfCheckersInField = this->m_Fields[fieldIndex].GetNumberOfCheckers();

    if(numberOfCheckersInField == 0)
    {
        return 0;
    }

    if((numberOfCheckersInField == 1) &&
       (this->m_Fields[fieldIndex].GetColor() != color))
    {
        return 1;
    }

    if((numberOfCheckersInField >= 1) &&
       (this->m_Fields[fieldIndex].GetColor() == color))
    {
        return 2;
    }

    return -1;
}

void Game::PlaceChecker(const int color, const int fieldIndex, const int action)
{
    glm::vec2 checkerHighlightPosition;
    switch(action)
    {
    case 0:
        {
            this->m_Fields[fieldIndex].AddChecker(color);
            if(color == CheckerColor::WHITE)
            {
                this->m_FirstPlayer.AddOccupiedField((short)fieldIndex);
                checkerHighlightPosition = this->m_Fields[fieldIndex].GetTopCheckerOrigin();
                this->m_CheckerHighlight.SetPosition(checkerHighlightPosition);
            }
            else
            {
                this->m_SecondPlayer.AddOccupiedField((short)fieldIndex);
            }

        } break;
    case 1:
        {
            this->m_Fields[fieldIndex].RemoveChecker();
            if(color == CheckerColor::WHITE)
            {
                this->m_SecondPlayer.AddLostChecker();
                this->m_SecondPlayer.RemoveOccupiedField((short)fieldIndex);

                this->m_FirstPlayer.AddOccupiedField((short)fieldIndex);
            }
            else
            {
                this->m_FirstPlayer.AddLostChecker();
                this->m_FirstPlayer.RemoveOccupiedField((short)fieldIndex);

                this->m_SecondPlayer.AddOccupiedField((short)fieldIndex);
            }
            this->m_Fields[fieldIndex].AddChecker(color);
            if(color == CheckerColor::WHITE)
            {
                checkerHighlightPosition = this->m_Fields[fieldIndex].GetTopCheckerOrigin();
                this->m_CheckerHighlight.SetPosition(checkerHighlightPosition);
            }
        } break;
    case 2:
        {
            this->m_Fields[fieldIndex].AddChecker(color);
            if(color == CheckerColor::WHITE)
            {
                checkerHighlightPosition = this->m_Fields[fieldIndex].GetTopCheckerOrigin();
                this->m_CheckerHighlight.SetPosition(checkerHighlightPosition);
            }
        } break;
    case 3:
        {
            this->m_SecondPlayer.AddCheckerInStack();
        } break;
    default:
        {
            this->m_Fields[this->m_SourceField].AddChecker(color);
            if(color == CheckerColor::WHITE)
            {
                checkerHighlightPosition = this->m_Fields[this->m_SourceField].GetTopCheckerOrigin();
                this->m_CheckerHighlight.SetPosition(checkerHighlightPosition);
            }
        } break;
    }
}

void Game::UseDie(const int dieIndex, const int state)
{
    this->m_UsedDice[dieIndex] ++;
    this->m_MovesMade ++;
    this->m_State = state;
}

void Game::Reset(void)
{
    this->m_FirstPlayer.ClearCheckerStack();
    this->m_FirstPlayer.ClearLostCheckers();
    this->m_FirstPlayer.ClearOccupiedFields();

    this->m_SecondPlayer.ClearCheckerStack();
    this->m_SecondPlayer.ClearLostCheckers();
    this->m_SecondPlayer.ClearOccupiedFields();

    this->m_TurnEnded = true;
    this->m_DiceRollings = 0;
    this->m_RollDicePressed = false;
    this->m_MovesMade = 0;
    this->m_CurrentPlayer = CheckerColor::BLACK;
    int i;
    for(i = 0; i < 6; i++)
    {
        this->m_UsedDice[i] = 0;
    }
    this->m_MoveAction = -1;

    this->m_CurrentPlayer = CheckerColor::WHITE;
}

void Game::Update(float deltaTime)
{
    glm::vec2 cursorPosition = GetCursorPosition();

    vector<Checker> whiteStack = this->m_FirstPlayer.GetCheckerStack();
    size_t checkersInWhiteStack = whiteStack.size();

    vector<Checker> blackStack = this->m_SecondPlayer.GetCheckerStack();
    size_t checkersInBlackStack = blackStack.size();

    if(checkersInBlackStack == 15)
    {
        this->m_Notification.SetNotificationMessage("Computer won!");
        this->m_IsNotificationShown = true;
        this->m_GameEnded = true;
        this->m_GameStarted = false;
    }
    else if(checkersInWhiteStack == 15)
    {
        this->m_Notification.SetNotificationMessage("You won!");
        this->m_IsNotificationShown = true;
        this->m_GameEnded = true;
        this->m_GameStarted = false;
    }

    vector<short> whiteFields = this->m_FirstPlayer.GetOccupiedFields();
    size_t numberOfWhiteFields;
    size_t numberOfLostWhiteCheckers;

    vector<short> blackFields = this->m_SecondPlayer.GetOccupiedFields();
    size_t numberOfBlackFields;
    vector<Checker> lostBlackCheckers = this->m_SecondPlayer.GetLostCheckers();
    size_t numberOfLostBlackCheckers;

    glm::vec2 checkerOrigin;

    int i;

    numberOfWhiteFields = whiteFields.size();
    for(i = 0; i < numberOfWhiteFields; i++)
    {
        if((IsCursorOverRectangle(cursorPosition, this->m_Fields[whiteFields[i]].GetBoundingRectangle()) == true) &&
           (this->m_IsCheckerPicked == false))
        {
            checkerOrigin = this->m_Fields[whiteFields[i]].GetTopCheckerOrigin();
            this->m_CheckerHighlight.SetPosition(checkerOrigin);
        }
    }

    if((this->m_IsNotificationShown == true) &&
       (IsCursorOverRectangle(cursorPosition, this->m_Notification.GetButtonBoundingRect()) == true))
    {
        if(glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            this->m_IsNotificationShown = false;
            if(this->m_GameEnded == false)
            {
                this->m_State = 0;
            }
            else
            {
                Reset();

                InitFields();
                InitCheckers();
                this->m_GameEnded = false;
                this->m_State = 0;
            }
        }
    }

    if(this->m_GameEnded == false)
    {
        switch(this->m_State)
        {
        case 0:
            {
                if(this->m_CurrentPlayer == CheckerColor::WHITE)
                {
                    if((glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) &&
                       (IsCursorOverRectangle(cursorPosition, this->m_RollDiceBtn.GetBoundingRectangle()) == true))
                    {
                        this->m_RollDicePressed = true;
                        this->m_TurnEnded = false;
                        this->m_CurrentPlayer = CheckerColor::WHITE;
                        this->m_GameStarted = true;

                        this->m_State = 1;
                    }
                }
                else if(this->m_CurrentPlayer == CheckerColor::BLACK)
                {
                    if((this->m_TurnEnded == true) && (this->m_RollDicePressed == false))
                    {
                        this->m_RollDicePressed = true;
                        this->m_GameStarted = true;
                        this->m_State = 1;
                    }
                }
            } break;
        case 1:
            {
                if(this->m_DiceRollings < 6)
                {
                    RollDice();
                    this->m_DiceRollings ++;
                }
                else
                {
                    if(this->m_FirstDieIndex == this->m_SecondDieIndex)
                    {
                        this->m_MaxNumberOfMoves = 4;
                    }
                    else
                    {
                        this->m_MaxNumberOfMoves = 2;
                    }

                    this->m_State = 2;
                }
            } break;
        case 2:
            {
                if(this->m_CurrentPlayer == CheckerColor::WHITE)
                {
                    this->m_State = 3;  // CHECK FIRST PLAYER
                }
                else
                {
                    this->m_State = 4; // CHECK SECOND PLAYER
                }
            } break;
        case 3: // CHECK FIRST PLAYER
            {
                if(((this->m_FirstPlayer.HasPossibleMove(this->m_Fields, this->m_FirstDieIndex) == false) &&
                   (this->m_FirstPlayer.HasPossibleMove(this->m_Fields, this->m_SecondDieIndex) == false)) &&
                   (this->m_MovesMade < this->m_MaxNumberOfMoves))
                {
                    this->m_TurnEnded = true;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_MovesMade = 0;
                    this->m_CurrentPlayer = CheckerColor::BLACK;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;
                    this->m_Notification.SetNotificationMessage("You have no more moves!");
                    this->m_IsNotificationShown = true;
                }
                else if((this->m_FirstPlayer.HasPossibleMove(this->m_Fields, this->m_FirstDieIndex) == false) &&
                        (this->m_UsedDice[this->m_SecondDieIndex] == 1) && (this->m_MovesMade < this->m_MaxNumberOfMoves) &&
                        (this->m_IsNotificationShown == false))
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::BLACK;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;
                    this->m_Notification.SetNotificationMessage("You have no more moves!");
                    this->m_IsNotificationShown = true;
                }
                else if((this->m_FirstPlayer.HasPossibleMove(this->m_Fields, this->m_SecondDieIndex) == false) &&
                        (this->m_UsedDice[this->m_FirstDieIndex] == 1) && (this->m_MovesMade < this->m_MaxNumberOfMoves) &&
                        (this->m_IsNotificationShown == false))
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::BLACK;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;
                    this->m_Notification.SetNotificationMessage("You have no more moves!");
                    this->m_IsNotificationShown = true;
                }
                else if(this->m_MovesMade == this->m_MaxNumberOfMoves)
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::BLACK;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;
                    this->m_State = 0;
                }
                else
                {
                    this->m_State = 5;
                }
            } break;
        case 4: // CHECK SECOND PLAYER
            {
                if(((this->m_SecondPlayer.HasPossibleMove(this->m_Fields, this->m_FirstDieIndex) == false) &&
                   (this->m_SecondPlayer.HasPossibleMove(this->m_Fields, this->m_SecondDieIndex) == false)) &&
                   (this->m_MovesMade < this->m_MaxNumberOfMoves))
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::WHITE;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;

                    this->m_State = 0;
                }
                else if((this->m_SecondPlayer.HasPossibleMove(this->m_Fields, this->m_FirstDieIndex) == false) &&
                        (this->m_UsedDice[this->m_SecondDieIndex] == 1) && (this->m_MovesMade < this->m_MaxNumberOfMoves))
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::WHITE;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;

                    this->m_State = 0;
                }
                else if((this->m_SecondPlayer.HasPossibleMove(this->m_Fields, this->m_SecondDieIndex) == false) &&
                        (this->m_UsedDice[this->m_FirstDieIndex] == 1) && (this->m_MovesMade < this->m_MaxNumberOfMoves))
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::WHITE;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;

                    this->m_State = 0;
                }
                else if(this->m_MovesMade == this->m_MaxNumberOfMoves)
                {
                    this->m_TurnEnded = true;
                    this->m_MovesMade = 0;
                    this->m_DiceRollings = 0;
                    this->m_RollDicePressed = false;
                    this->m_CurrentPlayer = CheckerColor::WHITE;
                    for(i = 0; i < 6; i++)
                    {
                        this->m_UsedDice[i] = 0;
                    }
                    this->m_MoveAction = -1;
                    this->m_State = 0;
                }
                else
                {
                    this->m_State = 8;
                }
            } break;
        case 5: // MANAGE WHITE CHECKERS
            {
                vector<Checker> lostWhiteCheckers = this->m_FirstPlayer.GetLostCheckers();
                numberOfLostWhiteCheckers = lostWhiteCheckers.size();
                if(numberOfLostWhiteCheckers > 0)
                {
                    if((glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
                    {
                        glm::vec2 topLostCheckerPosition = this->m_FirstPlayer.GetTopLostCheckerPosition();
                        if(IsCursorOverCircle(cursorPosition, topLostCheckerPosition) == true)
                        {
                            this->m_PickedChecker.SetColor(CheckerColor::WHITE);
                            this->m_PickedChecker.SetPosition(cursorPosition);
                            this->m_FirstPlayer.RemoveLostChecker();
                            this->m_IsCheckerPicked = true;
                            this->m_CheckerHighlight.SetPosition(cursorPosition);
                            this->m_SourceField = -1;

                            this->m_State = 6; // MANAGE LOST WHITE CHECKERS
                        }
                    }
                }
                else
                {
                    numberOfWhiteFields = whiteFields.size();
                    if((glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
                    {
                        for(i = 0; i < numberOfWhiteFields; i++)
                        {
                            checkerOrigin = this->m_Fields[whiteFields[i]].GetTopCheckerOrigin();
                            if(IsCursorOverCircle(cursorPosition, checkerOrigin) == true)
                            {
                                this->m_PickedChecker = this->m_Fields[whiteFields[i]].GetTopChecker();
                                this->m_PickedChecker.SetPosition(cursorPosition);
                                this->m_Fields[whiteFields[i]].RemoveChecker();

                                if(this->m_Fields[whiteFields[i]].GetNumberOfCheckers() == 0)
                                {
                                    this->m_FirstPlayer.RemoveOccupiedField(whiteFields[i]);
                                }

                                this->m_IsCheckerPicked = true;
                                this->m_SourceField = whiteFields[i];

                                this->m_State = 7; // MANAGE WHITE CHECKERS IN BOARD
                            }
                        }
                    }
                }
            } break;
        case 6: // MANAGE LOST WHITE CHECKERS
            {
                int difference;
                if((glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) &&
                   (this->m_IsCheckerPicked == true))
                {
                    this->m_IsCheckerPicked = false;

                    for(i = 0; i < 6; i++)
                    {
                        if(IsCursorOverRectangle(cursorPosition, this->m_Fields[i].GetBoundingRectangle()) == true)
                        {
                            difference = i - this->m_SourceField;

                            if(this->m_MaxNumberOfMoves == 2)
                            {
                                if(difference == (this->m_FirstDieIndex + 1))
                                {
                                    this->m_MoveAction = CanMoveChecker(i, CheckerColor::WHITE);
                                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 1))
                                    {
                                        PlaceChecker(CheckerColor::WHITE, i, this->m_MoveAction);
                                        UseDie(this->m_FirstDieIndex, 3);

                                        break;
                                    }
                                }
                                else if(difference == (this->m_SecondDieIndex + 1))
                                {
                                    this->m_MoveAction = CanMoveChecker(i, CheckerColor::WHITE);
                                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_SecondDieIndex] < 1))
                                    {
                                        PlaceChecker(CheckerColor::WHITE, i, this->m_MoveAction);
                                        UseDie(this->m_SecondDieIndex, 3);

                                        break;
                                    }
                                }
                                else
                                {
                                    this->m_FirstPlayer.AddLostChecker();
                                    this->m_State = 5;

                                    break;
                                }
                            }
                            else if(this->m_MaxNumberOfMoves == 4)
                            {
                                if(difference == (this->m_FirstDieIndex + 1))
                                {
                                    this->m_MoveAction = CanMoveChecker(i, CheckerColor::WHITE);
                                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 4))
                                    {
                                        PlaceChecker(CheckerColor::WHITE, i, this->m_MoveAction);
                                        UseDie(this->m_FirstDieIndex, 3);

                                        break;
                                    }
                                    else
                                    {
                                        this->m_FirstPlayer.AddLostChecker();
                                        this->m_State = 5;

                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(this->m_State == 6)
                    {
                        this->m_FirstPlayer.AddLostChecker();
                        this->m_State = 5;
                    }
                }
            } break;
        case 7: // MANAGE WHITE CHECKERS IN BOARD
            {
                int difference;
                if((glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) &&
                   (this->m_IsCheckerPicked == true))
                {
                    this->m_IsCheckerPicked = false;
                    for(i = 0; i < 24; i++)
                    {
                        if(IsCursorOverRectangle(cursorPosition, this->m_Fields[i].GetBoundingRectangle()) == true)
                        {
                            difference = i - this->m_SourceField;
                            if(this->m_MaxNumberOfMoves == 2)
                            {
                                if(difference == (this->m_FirstDieIndex + 1))
                                {
                                    this->m_MoveAction = CanMoveChecker(i, CheckerColor::WHITE);
                                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 1))
                                    {
                                        PlaceChecker(CheckerColor::WHITE, i, this->m_MoveAction);
                                        UseDie(this->m_FirstDieIndex, 3);

                                        break;
                                    }
                                }
                                else if(difference == (this->m_SecondDieIndex + 1))
                                {
                                    this->m_MoveAction = CanMoveChecker(i, CheckerColor::WHITE);
                                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_SecondDieIndex] < 1))
                                    {
                                        PlaceChecker(CheckerColor::WHITE, i, this->m_MoveAction);
                                        UseDie(this->m_SecondDieIndex, 3);

                                        break;
                                    }
                                }
                                else
                                {
                                    if(this->m_Fields[this->m_SourceField].GetNumberOfCheckers() == 0)
                                    {
                                        this->m_FirstPlayer.AddOccupiedField(this->m_SourceField);
                                    }
                                    this->m_Fields[this->m_SourceField].AddChecker(CheckerColor::WHITE);
                                    this->m_State = 5;

                                    break;
                                }
                            }
                            else if(this->m_MaxNumberOfMoves == 4)
                            {
                                if(difference == (this->m_FirstDieIndex + 1))
                                {
                                    this->m_MoveAction = CanMoveChecker(i, CheckerColor::WHITE);
                                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 4))
                                    {
                                        PlaceChecker(CheckerColor::WHITE, i, this->m_MoveAction);
                                        UseDie(this->m_FirstDieIndex, 3);

                                        break;
                                    }
                                }
                                else
                                {
                                    if(this->m_Fields[this->m_SourceField].GetNumberOfCheckers() == 0)
                                    {
                                        this->m_FirstPlayer.AddOccupiedField(this->m_SourceField);
                                    }
                                    this->m_Fields[this->m_SourceField].AddChecker(CheckerColor::WHITE);
                                    this->m_State = 5;

                                    break;
                                }
                            }
                        }
                    }
                    if(this->m_State == 7)
                    {
                        numberOfWhiteFields = whiteFields.size();
                        if((this->m_FirstPlayer.AreAllCheckersHome() == true) &&
                           (IsCursorOverRectangle(cursorPosition, this->m_WhiteCheckerStackField) == true))
                        {
                            difference = 24 - this->m_SourceField;
                            if(this->m_MaxNumberOfMoves == 2)
                            {
                                if((this->m_UsedDice[this->m_FirstDieIndex] == 0) &&
                                   (difference == (this->m_FirstDieIndex + 1)))
                                {
                                    this->m_FirstPlayer.AddCheckerInStack();
                                    UseDie(this->m_FirstDieIndex, 3);
                                }
                                else if((this->m_UsedDice[this->m_SecondDieIndex] == 0) &&
                                        (difference == (this->m_SecondDieIndex + 1)))
                                {
                                    this->m_FirstPlayer.AddCheckerInStack();
                                    UseDie(this->m_SecondDieIndex, 3);
                                }
                                else
                                {
                                    if(numberOfWhiteFields == 0)
                                    {
                                        difference = 24 - this->m_SourceField;
                                    }
                                    else
                                    {
                                        difference = 24 - whiteFields[0];
                                    }

                                    if((this->m_UsedDice[this->m_FirstDieIndex] == 0) &&
                                       (difference <= (this->m_FirstDieIndex + 1)))
                                    {
                                        this->m_FirstPlayer.AddCheckerInStack();
                                        UseDie(this->m_FirstDieIndex, 3);
                                    }
                                    else if((this->m_UsedDice[this->m_SecondDieIndex] == 0) &&
                                            (difference < (this->m_SecondDieIndex + 1)))
                                    {
                                        this->m_FirstPlayer.AddCheckerInStack();
                                        UseDie(this->m_SecondDieIndex, 3);
                                    }
                                }
                            }
                            else if(this->m_MaxNumberOfMoves == 4)
                            {
                                if((this->m_UsedDice[this->m_FirstDieIndex] < 4) &&
                                   (difference == (this->m_FirstDieIndex + 1)))
                                {
                                    this->m_FirstPlayer.AddCheckerInStack();
                                    UseDie(this->m_FirstDieIndex, 3);
                                }
                                else
                                {
                                    if(numberOfWhiteFields == 0)
                                    {
                                        difference = 24 - this->m_SourceField;
                                    }
                                    else
                                    {
                                        difference = 24 - whiteFields[0];
                                    }

                                    if((this->m_UsedDice[this->m_FirstDieIndex] < 4) &&
                                       (difference < (this->m_FirstDieIndex + 1)))
                                    {
                                        this->m_FirstPlayer.AddCheckerInStack();
                                        UseDie(this->m_FirstDieIndex, 3);
                                    }
                                }
                            }
                            if(this->m_State == 7)
                            {
                                if(this->m_Fields[this->m_SourceField].GetNumberOfCheckers() == 0)
                                {
                                    this->m_FirstPlayer.AddOccupiedField(this->m_SourceField);
                                }
                                this->m_Fields[this->m_SourceField].AddChecker(CheckerColor::WHITE);
                                this->m_State = 5;
                            }
                        }
                        else
                        {
                            if(this->m_Fields[this->m_SourceField].GetNumberOfCheckers() == 0)
                            {
                                this->m_FirstPlayer.AddOccupiedField(this->m_SourceField);
                            }
                            this->m_Fields[this->m_SourceField].AddChecker(CheckerColor::WHITE);
                            this->m_State = 5;
                        }
                    }
                }
            } break;
        case 8: // MANAGE BLACK CHECKERS
            {
                numberOfLostBlackCheckers = lostBlackCheckers.size();
                if(numberOfLostBlackCheckers > 0)
                {
                    this->m_State = 9; // MANAGE LOST BLACK CHECKERS
                }
                else
                {
                    this->m_State = 10; // MANAGE CHECKERS ON BOARD
                }
            } break;
        case 9: // MANAGE LOST BLACK CHECKERS
            {
                if(this->m_MaxNumberOfMoves == 2)
                {
                    destinationField = 24 - (this->m_FirstDieIndex + 1);
                    this->m_MoveAction = CanMoveChecker(destinationField, CheckerColor::BLACK);
                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 1))
                    {
                        UseDie(this->m_FirstDieIndex, 11);
                        this->m_IsMovingChecker = true;

                        startPosition = this->m_SecondPlayer.GetTopLostCheckerPosition();
                        targetPosition = this->m_Fields[destinationField].GetTopCheckerOrigin();
                        targetPosition.y += (32.0f * this->m_Fields[destinationField].GetOrientation());

                        this->m_PickedChecker.SetColor(CheckerColor::BLACK);
                        this->m_PickedChecker.SetPosition(startPosition);
                        this->m_IsCheckerPicked = true;

                        this->m_SecondPlayer.RemoveLostChecker();

                        distance = glm::distance(startPosition, targetPosition);
                        direction = glm::normalize(targetPosition - startPosition);
                        currentCheckerPosition = startPosition;
                    }
                    else
                    {
                        destinationField = 24 - (this->m_SecondDieIndex + 1);
                        this->m_MoveAction = CanMoveChecker(destinationField, CheckerColor::BLACK);
                        if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_SecondDieIndex] < 1) &&
                           (this->m_IsMovingChecker == false))
                        {
                            UseDie(this->m_SecondDieIndex, 11);
                            this->m_IsMovingChecker = true;

                            startPosition = this->m_SecondPlayer.GetTopLostCheckerPosition();
                            targetPosition = this->m_Fields[destinationField].GetTopCheckerOrigin();
                            targetPosition.y += (32.0f * this->m_Fields[destinationField].GetOrientation());

                            this->m_PickedChecker.SetColor(CheckerColor::BLACK);
                            this->m_PickedChecker.SetPosition(startPosition);
                            this->m_IsCheckerPicked = true;

                            this->m_SecondPlayer.RemoveLostChecker();

                            distance = glm::distance(startPosition, targetPosition);
                            direction = glm::normalize(targetPosition - startPosition);
                            currentCheckerPosition = startPosition;
                        }
                    }
                }
                else if(this->m_MaxNumberOfMoves == 4)
                {
                    destinationField = 24 - (this->m_FirstDieIndex + 1);
                    this->m_MoveAction = CanMoveChecker(destinationField, CheckerColor::BLACK);
                    if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 4))
                    {
                        UseDie(this->m_FirstDieIndex, 11);
                        this->m_IsMovingChecker = true;

                        startPosition = this->m_SecondPlayer.GetTopLostCheckerPosition();
                        targetPosition = this->m_Fields[destinationField].GetTopCheckerOrigin();
                        targetPosition.y += (32.0f * this->m_Fields[destinationField].GetOrientation());

                        this->m_PickedChecker.SetColor(CheckerColor::BLACK);
                        this->m_PickedChecker.SetPosition(startPosition);
                        this->m_IsCheckerPicked = true;

                        this->m_SecondPlayer.RemoveLostChecker();

                        distance = glm::distance(startPosition, targetPosition);
                        direction = glm::normalize(targetPosition - startPosition);
                        currentCheckerPosition = startPosition;
                    }
                }
            } break;
        case 10: // MANAGE BLACK CHECKERS ON BOARD
            {
                numberOfBlackFields = blackFields.size();
                for(i = 0; i < numberOfBlackFields; i++)
                {
                    if(this->m_MaxNumberOfMoves == 2)
                    {
                        destinationField = blackFields[i] - (this->m_FirstDieIndex + 1);
                        this->m_MoveAction = CanMoveChecker(destinationField, CheckerColor::BLACK);
                        if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 1))
                        {
                            UseDie(this->m_FirstDieIndex, 11);
                            this->m_IsMovingChecker = true;

                            startPosition = this->m_Fields[blackFields[i]].GetTopCheckerOrigin();

                            if((destinationField < 0) && (this->m_SecondPlayer.AreAllCheckersHome() == true))
                            {
                                targetPosition = this->m_SecondPlayer.GetPositionOfTopCheckerInStack();
                            }
                            else
                            {
                                targetPosition = this->m_Fields[destinationField].GetTopCheckerOrigin();
                                targetPosition.y += (32.0f * this->m_Fields[destinationField].GetOrientation());
                            }

                            this->m_PickedChecker.SetColor(CheckerColor::BLACK);
                            this->m_PickedChecker.SetPosition(startPosition);
                            this->m_IsCheckerPicked = true;

                            this->m_Fields[blackFields[i]].RemoveChecker();

                            if(this->m_Fields[blackFields[i]].GetNumberOfCheckers() == 0)
                            {
                                this->m_SecondPlayer.RemoveOccupiedField(blackFields[i]);
                            }

                            distance = glm::distance(startPosition, targetPosition);
                            direction = glm::normalize(targetPosition - startPosition);
                            currentCheckerPosition = startPosition;

                            break;
                        }

                        destinationField = blackFields[i] - (this->m_SecondDieIndex + 1);
                        this->m_MoveAction = CanMoveChecker(destinationField, CheckerColor::BLACK);
                        if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_SecondDieIndex] < 1))
                        {
                            UseDie(this->m_SecondDieIndex, 11);
                            this->m_IsMovingChecker = true;

                            startPosition = this->m_Fields[blackFields[i]].GetTopCheckerOrigin();

                            if((destinationField < 0) && (this->m_SecondPlayer.AreAllCheckersHome() == true))
                            {
                                targetPosition = this->m_SecondPlayer.GetPositionOfTopCheckerInStack();
                            }
                            else
                            {
                                targetPosition = this->m_Fields[destinationField].GetTopCheckerOrigin();
                                targetPosition.y += (32.0f * this->m_Fields[destinationField].GetOrientation());
                            }

                            this->m_PickedChecker.SetColor(CheckerColor::BLACK);
                            this->m_PickedChecker.SetPosition(startPosition);
                            this->m_IsCheckerPicked = true;

                            this->m_Fields[blackFields[i]].RemoveChecker();

                            if(this->m_Fields[blackFields[i]].GetNumberOfCheckers() == 0)
                            {
                                this->m_SecondPlayer.RemoveOccupiedField(blackFields[i]);
                            }

                            distance = glm::distance(startPosition, targetPosition);
                            direction = glm::normalize(targetPosition - startPosition);
                            currentCheckerPosition = startPosition;

                            break;
                        }
                    }
                    else if(this->m_MaxNumberOfMoves == 4)
                    {
                        destinationField = blackFields[i] - (this->m_FirstDieIndex + 1);
                        this->m_MoveAction = CanMoveChecker(destinationField, CheckerColor::BLACK);
                        if((this->m_MoveAction >= 0) && (this->m_UsedDice[this->m_FirstDieIndex] < 4))
                        {
                            UseDie(this->m_FirstDieIndex, 11);
                            this->m_IsMovingChecker = true;

                            startPosition = this->m_Fields[blackFields[i]].GetTopCheckerOrigin();

                            if((destinationField < 0) && (this->m_SecondPlayer.AreAllCheckersHome() == true))
                            {
                                targetPosition = this->m_SecondPlayer.GetPositionOfTopCheckerInStack();
                            }
                            else
                            {
                                targetPosition = this->m_Fields[destinationField].GetTopCheckerOrigin();
                                targetPosition.y += (32.0f * this->m_Fields[destinationField].GetOrientation());
                            }

                            this->m_PickedChecker.SetColor(CheckerColor::BLACK);
                            this->m_PickedChecker.SetPosition(startPosition);
                            this->m_IsCheckerPicked = true;

                            this->m_Fields[blackFields[i]].RemoveChecker();

                            if(this->m_Fields[blackFields[i]].GetNumberOfCheckers() == 0)
                            {
                                this->m_SecondPlayer.RemoveOccupiedField(blackFields[i]);
                            }

                            distance = glm::distance(startPosition, targetPosition);
                            direction = glm::normalize(targetPosition - startPosition);
                            currentCheckerPosition = startPosition;

                            break;
                        }
                    }
                }
            } break;
        case 11: // MOVE CHECKER
            {
                currentCheckerPosition += (direction * 8.0f);
                this->m_PickedChecker.SetPosition(currentCheckerPosition);
                currentDistance = glm::distance(currentCheckerPosition, targetPosition);
                if(currentDistance <= 32.0f)
                {
                    this->m_IsMovingChecker = false;

                    PlaceChecker(CheckerColor::BLACK, destinationField, this->m_MoveAction);

                    this->m_IsCheckerPicked = false;

                    this->m_State = 4;
                }
            } break;
        }
    }

    if((this->m_IsCheckerPicked == true) && (this->m_CurrentPlayer == CheckerColor::WHITE))
    {
        this->m_PickedChecker.SetPosition(cursorPosition);
        this->m_CheckerHighlight.SetPosition(this->m_PickedChecker.GetPosition());
    }
}

void Game::Render(void)
{
    float grey[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, grey);

    unsigned int i, j;

    glUseProgram(this->m_ShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    DrawBoard();

    DrawRollDiceButton();

    glBindVertexArray(this->m_CheckerVAO);
    glActiveTexture(GL_TEXTURE0);
    size_t numberOfCheckersInField;
    // DRAW WHITE CHECKERS >>
    glBindTexture(GL_TEXTURE_2D, this->m_WhiteCheckerTexture);

    vector<Checker> whiteCheckers;
    vector<short> whiteFields = this->m_FirstPlayer.GetOccupiedFields();
    size_t numberOfWhiteFields = whiteFields.size();

    for(i = 0; i < numberOfWhiteFields; i++)
    {
        whiteCheckers = this->m_Fields[whiteFields[i]].GetCheckers();
        numberOfCheckersInField = whiteCheckers.size();
        for(j = 0; j < numberOfCheckersInField; j++)
        {
            this->m_mtxWorld = whiteCheckers[j].GetModelMatrix();
            this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    // << DRAW WHITE CHECKERS

    // DRAW BLACK CHECKERS >>
    glBindTexture(GL_TEXTURE_2D, this->m_BlackCheckerTexture);

    vector<Checker> blackCheckers;
    vector<short> blackFields = this->m_SecondPlayer.GetOccupiedFields();
    size_t numberOfBlackFields = blackFields.size();

    for(i = 0; i < numberOfBlackFields; i++)
    {
        blackCheckers = this->m_Fields[blackFields[i]].GetCheckers();
        numberOfCheckersInField = blackCheckers.size();
        for(j = 0; j < numberOfCheckersInField; j++)
        {
            this->m_mtxWorld = blackCheckers[j].GetModelMatrix();
            this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
    // << DRAW BLACK CHECKERS

    // DRAW LOST WHITE CHECKERS >>
    glUseProgram(this->m_ShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->m_WhiteCheckerTexture);

    vector<Checker> lostWhiteCheckers = this->m_FirstPlayer.GetLostCheckers();
    size_t numberOfLostWhiteCheckers = lostWhiteCheckers.size();

    for(i = 0; i < numberOfLostWhiteCheckers; i++)
    {
        this->m_mtxWorld = lostWhiteCheckers[i].GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    // << DRAW LOST WHITE CHECKERS

    // DRAW LOST BLACK CHECKERS >>
    glBindTexture(GL_TEXTURE_2D, this->m_BlackCheckerTexture);

    vector<Checker> lostBlackCheckers = this->m_SecondPlayer.GetLostCheckers();
    size_t numberOfLostBlackCheckers = lostBlackCheckers.size();

    for(i = 0; i < numberOfLostBlackCheckers; i++)
    {
        this->m_mtxWorld = lostBlackCheckers[i].GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    // << DRAW LOST BLACK CHECKERS

    glBindVertexArray(0);

    glBindVertexArray(this->m_CheckerInStackVAO);
    glActiveTexture(GL_TEXTURE0);
    // DRAW WHITE CHECKER STACK >>
    glBindTexture(GL_TEXTURE_2D, this->m_WhiteCheckerInStackTexture);

    vector<Checker> whiteCheckerStack = this->m_FirstPlayer.GetCheckerStack();
    size_t numberOfWhiteCheckersInStack = whiteCheckerStack.size();

    for(i = 0; i < numberOfWhiteCheckersInStack; i++)
    {
        this->m_mtxWorld = whiteCheckerStack[i].GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // << DRAW WHITE CHECKER STACK

    // DRAW BLACK CHECKER STACK >>
    glBindTexture(GL_TEXTURE_2D, this->m_BlackCheckerInStackTexture);

    vector<Checker> blackCheckerStack = this->m_SecondPlayer.GetCheckerStack();
    size_t numberOfBlackCheckersInStack = blackCheckerStack.size();

    for(i = 0; i < numberOfBlackCheckersInStack; i++)
    {
        this->m_mtxWorld = blackCheckerStack[i].GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    // << DRAW BLACK CHECKER STACK

    glBindVertexArray(0);

    DrawDice();

    // DRAW FIELD CONTOUR >>
    glUseProgram(this->m_ColorSP);

    glBindVertexArray(this->m_FieldContourVAO);

    this->m_mtxWorld = this->m_FieldContour.GetModelMatrix();
    this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glBindVertexArray(0);
    // << DRAW FIELD CONTOUR

    // DRAW CHECKER HIGHLIGHT >>
    glBindVertexArray(this->m_CheckerHighlightVAO);

    this->m_mtxWorld = this->m_CheckerHighlight.GetModelMatrix();
    this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 361);

    glBindVertexArray(0);
    // << DRAW CHECKER HIGHLIGHT

    glUseProgram(this->m_ShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    DrawPickedChecker();

    if(this->m_IsNotificationShown == true)
    {
        this->m_Notification.Show();
    }
}

void Game::DrawBoard(void)
{
    glBindVertexArray(this->m_BoardVAO);

    glBindTexture(GL_TEXTURE_2D, this->m_BoardTexture);

    this->m_mtxWorld = this->m_Board.GetModelMatrix();
    this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}

void Game::DrawRollDiceButton(void)
{
    glBindVertexArray(this->m_RollDiceBtnVAO);

    if(m_RollDicePressed == false)
    {
        glBindTexture(GL_TEXTURE_2D, this->m_RollDiceBtnTexture);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, this->m_RollDiceBtnPressedTexture);
    }

    this->m_mtxWorld = this->m_RollDiceBtn.GetModelMatrix();
    this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}

void Game::DrawDice(void)
{
    glBindVertexArray(this->m_DieVAO);

    glBindTexture(GL_TEXTURE_2D, this->m_DieTexture);

    GLfloat* vertexData;
    float left, right;
    if(this->m_GameStarted == true)
    {
        left = (this->m_FirstDieIndex % 6) * 64.0f;
        right = left + 64.0f;
        vertexData = (GLfloat*)glMapNamedBuffer(this->m_DieVB, GL_WRITE_ONLY);
        vertexData[3] = left / 384.0f;
        vertexData[8] = right / 384.0f;
        vertexData[13] = left / 384.0f;
        vertexData[18] = right / 384.0f;
        glUnmapNamedBuffer(this->m_DieVB);

        this->m_mtxWorld = this->m_Dice[0].GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        left = (this->m_SecondDieIndex % 6) * 64.0f;
        right = left + 64.0f;
        vertexData = (GLfloat*)glMapNamedBuffer(this->m_DieVB, GL_WRITE_ONLY);
        vertexData[3] = left / 384.0f;
        vertexData[8] = right / 384.0f;
        vertexData[13] = left / 384.0f;
        vertexData[18] = right / 384.0f;
        glUnmapNamedBuffer(this->m_DieVB);

        this->m_mtxWorld = this->m_Dice[1].GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glBindVertexArray(0);
}

void Game::DrawPickedChecker(void)
{
    if(this->m_IsCheckerPicked == true)
    {
        glBindVertexArray(this->m_CheckerVAO);

        if(this->m_PickedChecker.GetColor() == CheckerColor::WHITE)
        {
            glBindTexture(GL_TEXTURE_2D, this->m_WhiteCheckerTexture);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, this->m_BlackCheckerTexture);
        }

        this->m_mtxWorld = this->m_PickedChecker.GetModelMatrix();
        this->m_mtxWorldViewProj = this->m_mtxViewProj * this->m_mtxWorld;

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(this->m_mtxWorldViewProj));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(0);
    }
}

void Game::OnResize(int width, int height)
{
    GLBase::OnResize(width, height);

    this->m_mtxProj = glm::ortho(0.0f, (float)m_appInfo.windowWidth, (float)m_appInfo.windowHeight, 0.0f,
                           0.01f, 100.0f);

    this->m_mtxViewProj = this->m_mtxProj * this->m_mtxView;

    this->m_Notification.SetViewProjMatrix(this->m_mtxViewProj);
}

void Game::ReleaseObjects(void)
{
    this->m_VertexShader.DeleteShader();
    this->m_FragmentShader.DeleteShader();

    this->m_Notification.ReleaseMemory();

    glDeleteTextures(1, &this->m_BoardTexture);
    glDeleteTextures(1, &this->m_DieTexture);
    glDeleteTextures(1, &this->m_RollDiceBtnTexture);
    glDeleteTextures(1, &this->m_RollDiceBtnPressedTexture);
    glDeleteTextures(1, &this->m_WhiteCheckerTexture);
    glDeleteTextures(1, &this->m_BlackCheckerTexture);
    glDeleteTextures(1, &this->m_WhiteCheckerInStackTexture);
    this->m_TextureManager.ReleaseMemory();

    glDeleteBuffers(1, &this->m_BoardVB);
    glDeleteBuffers(1, &this->m_CheckerVB);
    glDeleteBuffers(1, &this->m_DieVB);
    glDeleteBuffers(1, &this->m_RollDiceBtnVB);
    glDeleteBuffers(1, &this->m_FieldContourVB);
    glDeleteBuffers(1, &this->m_CheckerHighlightVB);
    glDeleteBuffers(1, &this->m_CheckerInStackVB);

    glDeleteProgram(this->m_ShaderProgram);
    glDeleteProgram(this->m_ColorSP);

    glDeleteVertexArrays(1, &this->m_BoardVAO);
    glDeleteVertexArrays(1, &this->m_DieVAO);
    glDeleteVertexArrays(1, &this->m_CheckerVAO);
    glDeleteVertexArrays(1, &this->m_RollDiceBtnVAO);
    glDeleteVertexArrays(1, &this->m_FieldContourVAO);
    glDeleteVertexArrays(1, &this->m_CheckerHighlightVAO);
    glDeleteVertexArrays(1, &this->m_CheckerInStackVAO);

    GLBase::ReleaseObjects();
}
