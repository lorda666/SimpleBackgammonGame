#pragma once

#include "Sprite.h"

class Die : public Sprite
{
    public:
        Die(void);
        virtual ~Die(void);

        void Rotate(void);

    private:
};
