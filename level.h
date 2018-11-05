#pragma once

#include "renderSystem.h"

typedef const unsigned char obj;

const int levelRows = screenRows;
const int levelColumns = screenColumns;
const int gravitySpeed = 38;

const int levelCount = 1; // Counting with zero

obj CellSymbol_Empty       = ' ';
obj CellSymbol_Wall        = '#';
obj CellSymbol_SteelWall   = '@';
obj CellSymbol_Abyss       = 'a';
obj CellSymbol_Pipe        = 'p';
obj CellSymbol_Crystal     = '0';
obj CellSymbol_Box         = '?';
obj CellSymbol_OpenedBox   = '&';
obj CellSymbol_Mushroom    = '^';
obj CellSymbol_MushroomBox = '!';
obj CellSymbol_Hero        = 'h';
obj CellSymbol_Goomba      = 'g';
obj CellSymbol_Koopa       = 'k';
obj CellSymbol_TurnedKoopa = 'K';
obj CellSymbol_Exit        = '|';
obj CellSymbol_Flag        = '>';
obj CellSymbol_Jumper      = 'J';
obj CellSymbol_JumpKoopa   = 'j';
obj CellSymbol_SecondHero  = '2';

obj allLevels [ ][levelRows][levelColumns+1] = {

                                            {
                                                "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
                                                "#         0                                                          g         #",
                                                "#      @     @                                                       @     |>  #",
                                                "#     @@     @                  ##?#        0 0 0                   @@     |   #",
                                                "#    @@@     @@      pppp                            pppp          @@@     |   #",
                                                "#   @@@@ k   @@@      pp   g     k      g g           pp          @@@@     |   #",
                                                "#  @@@@@@@J@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#",
                                                "#                                                                              #",
                                                "#                   0                                      0 0                 #",
                                                "# @       #??#     ###            !               ?####   ######               #",
                                                "# @@                                                                           #",
                                                "# @@@                                                                          #",
                                                "# @@@@     ##      ######     ?   ?   ?     ?#    #               ##           #",
                                                "# @@@@@                                                                        #",
                                                "# @@@@@@               # g g g g            g             g g     k      #     #",
                                                "# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##@    #",
                                                "#                                                                              #",
                                                "#               ?                                                            ###",
                                                "#                                                                              #",
                                                "#                                              pppp                     #!#    #",
                                                "#   2      ?  #!#?#                 pppp        pp        pppp          # #    #",
                                                "#                        pppp        pp         pp         pp                  #",
                                                "#  h           g          pp    k    pp g       pp     k g pp        #   g     #",
                                                "# ####J############################################################  ###########",
                                                "#a#################################################################aa###########",
                                            },
                                            {
                                                "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
                                                "#         @@@@@         @@@@@             @@@@                       g         #",
                                                "#      @   @@@    ###?#  @@@               @@        pppp            @     |>  #",
                                                "#     @@0                 @     ##?#      0 0 0 0    pppp           @@     |   #",
                                                "#    @@@     @@      pppp                             pp    g     @@@@     |   #",
                                                "#   @@@@ g   @@J  g   pp   g     k      g g           pp@@@@@@@@@@@@@@     |   #",
                                                "#  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@J@@@@@@@@@@@@@@@@@@@@@@@@@@@@#",
                                                "#    @@@                           @@@@@                         @@@@@@@@@@@@@@#",
                                                "#     @             0               @@@                     0 0   @@@@@@@ @@@@@#",
                                                "#J        #??#     ###    ####    !  @0            ?####   ######  @@@@@  @@@@@#",
                                                "#@ @                                  ####                          @@@   @@@@@#",
                                                "#@ @@@                                                               @    @@@@@#",
                                                "#@ @@@     ##############     ?      ?      ?#    #               ##      @@@@@#",
                                                "#@ @@@@@@!?@@0                                                            @@@@@#",
                                                "#@ @@@@@@         g    #       g    g       g      g      g g        #   #@@@@@#",
                                                "#@ @@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ##@@@@@#",
                                                "#@                              @@@@@      @@@@@@@@@@ @@@@@@@@@@          @@@@@#",
                                                "#@@!     k                g     @@@@@       @@@@@@@@   @@@@@@@@ ###          @@#",
                                                "#   #######         ####?###    @@@@@   ?    @@@@@@     @@@@@@    #  g  #      #",
                                                "#?        #####          ###                                      ##@@@##   ?  #",
                                                "#   2     ########J      ###           pppp      ???     pppp                 @#",
                                                "#@                ##    ####    ppp     pp                pp                 @@#",
                                                "#@ h           k       #####     p   g  pp   g   k       gpp        #   g   @@@#",
                                                "#@@@#J######################    #p################################   ###########",
                                                "#@@@########################aaaa#p################################aaa###########",
                                            },

                                            };

unsigned char GetRenderCellSymbol(unsigned char cellSymbol)
{
    switch(cellSymbol)
    {
        case CellSymbol_Empty:       return ' ';
        case CellSymbol_Wall:        return 177;
        case CellSymbol_SteelWall:   return 178;
        case CellSymbol_Abyss:       return ' ';
        case CellSymbol_Pipe:        return 178;
        case CellSymbol_Crystal:     return 4;
        case CellSymbol_Box:         return '?';
        case CellSymbol_OpenedBox:   return 254;
        case CellSymbol_Mushroom:    return 6;
        case CellSymbol_MushroomBox: return '?';
        case CellSymbol_Hero:        return 2;
        case CellSymbol_Goomba:      return 2;
        case CellSymbol_Koopa:       return 2;
        case CellSymbol_TurnedKoopa: return 2;
        case CellSymbol_Exit:        return 222;
        case CellSymbol_Flag:        return 16;
        case CellSymbol_Jumper:      return 24;
        case CellSymbol_JumpKoopa:   return 2;
        case CellSymbol_SecondHero:  return 2;
    }

    return '?';
}

ConsoleColor GetRenderSymbolColor(unsigned char cellSymbol)
{
    switch(cellSymbol)
    {
        case CellSymbol_Wall:        return ConsoleColor_DarkYellow;
        case CellSymbol_SteelWall:   return ConsoleColor_White;
        case CellSymbol_Pipe:        return ConsoleColor_Green;
        case CellSymbol_Crystal:     return ConsoleColor_Cyan;
        case CellSymbol_Box:         return ConsoleColor_DarkRed;
        case CellSymbol_OpenedBox:   return ConsoleColor_Gray;
        case CellSymbol_Mushroom:    return ConsoleColor_Red;
        case CellSymbol_MushroomBox: return ConsoleColor_DarkRed;
        case CellSymbol_Hero:        return ConsoleColor_Yellow;
        case CellSymbol_Goomba:      return ConsoleColor_DarkRed;
        case CellSymbol_Koopa:       return ConsoleColor_Green;
        case CellSymbol_TurnedKoopa: return ConsoleColor_DarkGreen;
        case CellSymbol_Exit:        return ConsoleColor_Green;
        case CellSymbol_Flag:        return ConsoleColor_White;
        case CellSymbol_Jumper:      return ConsoleColor_Green;
        case CellSymbol_JumpKoopa:   return ConsoleColor_Green;
        case CellSymbol_SecondHero:  return ConsoleColor_White;
    }

    return ConsoleColor_Gray;
}

ConsoleColor GetRenderSymbolBackGroundColor(unsigned char cellSymbol)
{
    switch(cellSymbol)
    {
        case CellSymbol_Wall:        return ConsoleColor_Red;
        case CellSymbol_SteelWall:   return ConsoleColor_DarkGray;
        case CellSymbol_Pipe:        return ConsoleColor_DarkGreen;
        case CellSymbol_Box:         return ConsoleColor_Yellow;
        case CellSymbol_OpenedBox:   return ConsoleColor_DarkGray;
        case CellSymbol_MushroomBox: return ConsoleColor_Yellow;
        case CellSymbol_Jumper:      return ConsoleColor_DarkGray;
    }

    return ConsoleColor_Black;
}

ConsoleColor GetRenderHeroColor(int heroHealth)
{
    switch(heroHealth)
    {
        case 2: return ConsoleColor_Red;
    }

    return ConsoleColor_Yellow;
}

ConsoleColor GetRenderSecondHeroColor(int heroHealth)
{
    switch(heroHealth)
    {
        case 2: return ConsoleColor_Magenta;
    }

    return ConsoleColor_White;
}
