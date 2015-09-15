#pragma once

#include "Sprite.h"

class Checker : public Sprite
{
    public:
        Checker(void);
        virtual ~Checker(void);

        void SetColor(const int color);

        const int GetColor(void);
    protected:
    private:
        int     m_Color; // 0 = black; 1 = white
};
