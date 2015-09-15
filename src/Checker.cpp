#include "Checker.h"

Checker::Checker(void)
{
    //ctor
}

Checker::~Checker(void)
{
    //dtor
}

void Checker::SetColor(const int color)
{
    this->m_Color = color;
}

const int Checker::GetColor(void)
{
    return this->m_Color;
}
