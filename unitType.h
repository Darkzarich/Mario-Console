#pragma once

#include "level.h"

enum UnitType
{
    UnitType_None,
    UnitType_Hero,
    UnitType_SecondHero,
    UnitType_Goomba,
    UnitType_Mushroom,
    UnitType_Koopa,
    UnitType_TurnedKoopa,
    UnitType_JumpKoopa,
};

UnitType GetUnitTypeFromCell(unsigned char cellSymbol)
{

    switch(cellSymbol)
    {
        case CellSymbol_Hero:          return UnitType_Hero;
        case CellSymbol_Goomba:        return UnitType_Goomba;
        case CellSymbol_Mushroom:      return UnitType_Mushroom;
        case CellSymbol_Koopa:         return UnitType_Koopa;
        case CellSymbol_TurnedKoopa:   return UnitType_TurnedKoopa;
        case CellSymbol_JumpKoopa:     return UnitType_JumpKoopa;
        case CellSymbol_SecondHero:    return UnitType_SecondHero;
    }

    return UnitType_None;
}

int GetUnitSpeedX(UnitType ut)
{
    switch(ut)
    {
        case UnitType_Hero: return 10;
        case UnitType_Goomba: return 3;
        case UnitType_Mushroom: return 5;
        case UnitType_Koopa:  return 4;
        case UnitType_TurnedKoopa:  return 15;
        case UnitType_JumpKoopa:  return 4;
        case UnitType_SecondHero:  return 10;
    }

    return 0;
}

int GetUnitJumpSpeed(UnitType ut)
{
    switch(ut)
    {
        case UnitType_Hero: return 17;
        case UnitType_SecondHero:  return 17;
        case UnitType_JumpKoopa:  return 8;
    }

    return 0;
}
