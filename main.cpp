#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "renderSystem.h"
#include "input.h"
#include "level.h"
#include "unitData.h"

// Constants

const int maxUnitCount = 50;

const float cellBeginValue = 0.001f;
const float cellEndValue = 0.999f;

// Logic variables
bool isGameActive = true;
clock_t clockLastFrame = 0;

int framesCounter = 0;
float framesTimeCounter = 0;
int fps = 0;

int current_level = 0;

unsigned char levelData[levelRows][levelColumns];

UnitData unitsData[maxUnitCount];
int unitsCount = 0;
int heroIndex  = 0;
int twoheroIndex  = 0;

int score = 0;

bool twoPlayer = false;

// Functions

void SetupSystem()
{
    srand(time(0));

    RenderSystemInitialize();
}

void Initialize()
{
    clockLastFrame = clock();

    unitsCount = 0;

    // Load level
    for(int r = 0; r < levelRows; r++)
    {
        for(int c = 0; c < levelColumns; c++)
        {
            unsigned char cellSymbol = allLevels[current_level][r][c];
            levelData[r][c] = cellSymbol;

            switch(cellSymbol)
            {
                case CellSymbol_SecondHero:
                    if (twoPlayer)
                       twoheroIndex = unitsCount;
                    else
                    {
                        levelData[r][c] = CellSymbol_Empty;
                        break;
                    }
                case CellSymbol_Hero:
                {
                    heroIndex = unitsCount;
                    if (!twoPlayer)
                    {
                        twoheroIndex = unitsCount;
                    }

                }

                case CellSymbol_Koopa:

                case CellSymbol_JumpKoopa:

                case CellSymbol_Goomba:
                    UnitType ut = GetUnitTypeFromCell(cellSymbol);
                    unitsData[unitsCount].type = ut;
                    unitsData[unitsCount].y = float(r);
                    unitsData[unitsCount].x = float(c);
                    if (cellSymbol == CellSymbol_Koopa && cellSymbol == CellSymbol_JumpKoopa)
                        unitsData[unitsCount].health = 2;
                    else
                        unitsData[unitsCount].health = 1;
                    unitsData[unitsCount].ySpeed = 0.0;
                    unitsData[unitsCount].xSpeed = 0.0;
                    unitsData[unitsCount].xOrder = UnitOrder_None;
                    unitsData[unitsCount].yOrder = UnitOrder_None;

                    unitsCount++;

                    break;
            }
        }
    }
}

void Render()
{
    // Start frame
    RenderSystemClear();

    // Draw frame (level)

    for(int r = 0; r < levelRows; r++)
    {
        for(int c = 0; c < levelColumns; c++)
        {
            unsigned char cellSymbol = levelData[r][c];

            unsigned char renderSymbol = GetRenderCellSymbol(cellSymbol);
            ConsoleColor symbcol = GetRenderSymbolColor(cellSymbol);
            ConsoleColor symbbgcol = GetRenderSymbolBackGroundColor(cellSymbol);

            if (cellSymbol == CellSymbol_Hero)
                symbcol = GetRenderHeroColor(unitsData[heroIndex].health);


            if (cellSymbol == CellSymbol_SecondHero)
                symbcol = GetRenderSecondHeroColor(unitsData[twoheroIndex].health);

            RenderSystemDrawChar(r, c, renderSymbol, symbcol, symbbgcol);
        }
    }

    // Draw FPS
    char textBuffer[16];
    sprintf(textBuffer, "FPS: %d", fps);
    RenderSystemDrawText(0, 0, textBuffer, ConsoleColor_Red, ConsoleColor_Black);

    // Draw Score
    char scoreBuffer[16];
    sprintf(scoreBuffer, "SCORE: %d", score);
    RenderSystemDrawText(0, 17, scoreBuffer, ConsoleColor_Cyan, ConsoleColor_Black);

    // Draw Score
    if (!twoPlayer)
        RenderSystemDrawText(0, 34, "Press SHIFT 2P", ConsoleColor_Cyan, ConsoleColor_Black);
    else
        RenderSystemDrawText(0, 34, "2P mode", ConsoleColor_White, ConsoleColor_Blue);

    // End frame
    RenderSystemFlush();
}

UnitData* GetUnitAt(int row, int column)
{
    for (int u = 0; u < unitsCount; u++)
    {
        if (unitsData[u].health <= 0)
        {
            continue;
        }
        else if (int(unitsData[u].y) == row
                 && int(unitsData[u].x) == column)
            return &unitsData[u];
    }

    return 0;
}

void KillUnit(UnitData* pToUnitData)
{
    pToUnitData->health = 0;
    int row = int(pToUnitData->y);
    int column = int(pToUnitData->x);
    levelData[row][column] = CellSymbol_Empty;
}

void UpdateAI()
{
    for (int u = 0; u < unitsCount; u++)
    {
        // Ignore hero
        if (u == heroIndex || u == twoheroIndex)
            continue;

        // Ignore TurnedKoopa
        if (unitsData[u].xOrder == UnitOrder_None && unitsData[u].type == UnitType_TurnedKoopa)
        {
            continue;
        }
        // Ignore dead units
        if (unitsData[u].health <= 0)
            continue;

        int row = int(unitsData[u].y);
        int column = int(unitsData[u].x);

        if (unitsData[u].xOrder == UnitOrder_None)
        {
            //Start move to empty cell
            if (levelData[row][column-1] == CellSymbol_Empty)
                unitsData[u].xOrder = UnitOrder_Backward;
            else
                unitsData[u].xOrder = UnitOrder_Forward;
        }
        else
        {
            if (unitsData[u].xOrder == UnitOrder_Backward)
            {
                unsigned char cellLeft = levelData[row][column - 1];
                UnitType unitType = GetUnitTypeFromCell(cellLeft);

                // Can not move cell left
                if ((unitsData[u].x <= (column+cellBeginValue)) && cellLeft != CellSymbol_Empty && unitType == UnitType_None)
                    unitsData[u].xOrder = UnitOrder_Forward;
            }
            else
            {
                unsigned char cellRight = levelData[row][column + 1];
                UnitType unitType = GetUnitTypeFromCell(cellRight);

                // Can not move cell right
                if ((unitsData[u].x >= (column+cellEndValue)) && cellRight != CellSymbol_Empty && unitType == UnitType_None)
                    unitsData[u].xOrder = UnitOrder_Backward;
            }
        }

        if (unitsData[u].type == UnitType_JumpKoopa)
        {
            unitsData[u].yOrder = UnitOrder_Backward;
        }
    }
}

bool MoveUnitTo(UnitData* pToUnitData, float newX, float newY)
{
    if (pToUnitData->health <= 0)
        return false;

    bool canMoveToCell = false;

    int newRow = int(newY);
    int newColumn = int(newX);
    int oldRow = int(pToUnitData->y);
    int oldColumn = int(pToUnitData->x);

    unsigned char unitSymbol = levelData[oldRow][oldColumn];
    unsigned char destinCell = levelData[newRow][newColumn];

    int directionRow = newRow - oldRow;
    int directionColumn = newColumn - oldColumn;

    // All action
    switch(destinCell)
    {
        case CellSymbol_Empty:
            canMoveToCell = true;
            break;

        case CellSymbol_Abyss:
            KillUnit(pToUnitData);
            break;
    }

    // Only hero actions
    if (pToUnitData->type == UnitType_Hero || pToUnitData->type == UnitType_SecondHero)
    {
        switch(destinCell)
        {
            case CellSymbol_Exit:
                current_level++;
                if (current_level > levelCount)
                {
                    isGameActive = false;
                }
                Initialize();
                break;

            case CellSymbol_Crystal:
                canMoveToCell = true;
                score += 50;
                break;

            case CellSymbol_Mushroom:
                canMoveToCell = true;
                if (pToUnitData->health < 2)
                {
                    pToUnitData->health = 2;
                    KillUnit(GetUnitAt(newRow, newColumn));
                }

                break;

            case CellSymbol_Wall:
                if ((directionRow < 0) && (pToUnitData->health > 1))
                    levelData[newRow][newColumn] = CellSymbol_Empty;
                break;

            case CellSymbol_Goomba:
                if (directionRow > 0)
                {
                    UnitData* unitData = GetUnitAt(newRow, newColumn);
                    if (unitData != 0)
                    {
                        KillUnit(unitData);
                        score += 10;
                        pToUnitData->ySpeed = -GetUnitJumpSpeed(pToUnitData->type);
                    }
                }
                break;

            case CellSymbol_Koopa:
                if (directionRow > 0)
                {
                    UnitData* unitData = GetUnitAt(newRow, newColumn);
                    if (unitData != 0)
                    {
                        unitData->type = UnitType_TurnedKoopa;
                        unitData->xOrder = UnitOrder_None;
                        levelData[newRow][newColumn] = CellSymbol_TurnedKoopa;
                        score += 20;
                        pToUnitData->ySpeed = -GetUnitJumpSpeed(pToUnitData->type);
                    }
                }
                break;

            case CellSymbol_TurnedKoopa:
            {

                UnitData* unitData = GetUnitAt(newRow, newColumn);
                if (unitData != 0)
                {
                    if (directionColumn > 0)
                    {
                        unitData->xOrder = UnitOrder_Forward;
                    }
                    else
                    {
                        unitData->xOrder = UnitOrder_Backward;
                    }
                }
                break;

            }

            case CellSymbol_MushroomBox:

                if (directionRow < 0)
                {
                    levelData[newRow-1][newColumn] = CellSymbol_Mushroom;

                    unitsData[unitsCount].type = UnitType_Mushroom;
                    unitsData[unitsCount].y = float(newRow-1);
                    unitsData[unitsCount].x = float(newColumn);
                    unitsData[unitsCount].health = 1;
                    unitsData[unitsCount].ySpeed = 0.0;
                    unitsData[unitsCount].xSpeed = 0.0;
                    unitsData[unitsCount].xOrder = UnitOrder_Forward;
                    unitsData[unitsCount].yOrder = UnitOrder_None;
                    unitsCount++;

                    levelData[newRow][newColumn] = CellSymbol_OpenedBox;
                    break;
                }

            case CellSymbol_Box:

                if (directionRow < 0)
                {
                    levelData[newRow-1][newColumn] = CellSymbol_Crystal;
                    levelData[newRow][newColumn] = CellSymbol_OpenedBox;
                }
                break;
        }
    }
    // Only monster actions
    else
    {
        switch(destinCell)
        {
            case CellSymbol_Hero:
            {
                if (pToUnitData->type != UnitType_Mushroom)
                {
                    unitsData[heroIndex].health--;
                    if (unitsData[heroIndex].health <= 0)
                    {
                        KillUnit(&unitsData[heroIndex]);
                    }

                    if (pToUnitData->xOrder == UnitOrder_Backward)
                    {
                        pToUnitData->xOrder = UnitOrder_Forward;
                    }
                    else
                    {
                       pToUnitData->xOrder = UnitOrder_Backward;
                    }
                    break;
                }
                else
                {
                    if (unitsData[heroIndex].health < 2)
                    {
                        unitsData[heroIndex].health = 2;
                        KillUnit(GetUnitAt(pToUnitData->y, pToUnitData->x));
                    }
                    break;
                }
            }
            case CellSymbol_SecondHero:
            {
                if (pToUnitData->type != UnitType_Mushroom)
                {
                    unitsData[twoheroIndex].health--;
                    if (unitsData[twoheroIndex].health <= 0)
                    {
                        KillUnit(&unitsData[twoheroIndex]);
                    }

                    if (pToUnitData->xOrder == UnitOrder_Backward)
                    {
                        pToUnitData->xOrder = UnitOrder_Forward;
                    }
                    else
                    {
                       pToUnitData->xOrder = UnitOrder_Backward;
                    }
                    break;
                }
                else
                {
                    if (unitsData[twoheroIndex].health < 2)
                    {
                        unitsData[twoheroIndex].health = 2;
                        KillUnit(GetUnitAt(pToUnitData->y, pToUnitData->x));
                    }
                    break;
                }
            }
            default:
                UnitType unitType = GetUnitTypeFromCell (destinCell);
                if (unitType != UnitType_None)
                {
                    if (pToUnitData->type == UnitType_TurnedKoopa && unitType == UnitType_Goomba
                        && pToUnitData->xOrder != UnitOrder_None)
                    {
                        KillUnit(GetUnitAt(newRow, newColumn));
                        score += 10;
                        break;
                    }

                    if (pToUnitData->xOrder == UnitOrder_Backward)
                    {
                        pToUnitData->xOrder = UnitOrder_Forward;
                    }
                    else
                        pToUnitData->xOrder = UnitOrder_Backward;
                }
                break;

        }
    }

    if (canMoveToCell)
    {
        levelData[oldRow][oldColumn] = CellSymbol_Empty;

        pToUnitData->x = newX;
        pToUnitData->y = newY;

        levelData[newRow][newColumn] = unitSymbol;
    }

    return canMoveToCell;
}

void UpdateUnit(UnitData* pToUnitData, float deltaTime)
{
    // Unit row and column
    int row = int(pToUnitData->y);
    int column = int(pToUnitData->x);

    if (levelData[row + 1][column] == CellSymbol_Jumper)
    {
            pToUnitData->ySpeed = -17;
    }

    // Y order
    if (pToUnitData->yOrder == UnitOrder_Backward)
    {
        // Jump
        if (pToUnitData->y >= (row + cellEndValue)
            && (levelData[row + 1][column] != CellSymbol_Empty)
            && (levelData[row + 1][column] != CellSymbol_Abyss))
        {
            pToUnitData->ySpeed = -GetUnitJumpSpeed(pToUnitData->type);
        }
    }

    // X order
    if (pToUnitData->xOrder == UnitOrder_Backward)
    {
         pToUnitData->xSpeed = -GetUnitSpeedX(pToUnitData->type);
    }
    else
    {
        if (pToUnitData->xOrder == UnitOrder_Forward)
        {
            pToUnitData->xSpeed = GetUnitSpeedX(pToUnitData->type);
        }
        else
        {
            pToUnitData->xSpeed = 0;
        }
    }

    // New position

    float deltaY = pToUnitData->ySpeed * deltaTime;
    float deltaX = pToUnitData->xSpeed * deltaTime;
    float newY = pToUnitData->y + deltaY;
    float newX = pToUnitData->x + deltaX;
    int newRow = int(newY);
    int newColumn = int(newX);

    // Y (row step)
    if (newRow != row)
    {
        // If unit can go to cell
        if(MoveUnitTo(pToUnitData, pToUnitData->x, newY))
        {
            row = int(pToUnitData->y);
        }
        else
        {
            // Can not move cell down
            if (newRow > row)
            {
                pToUnitData->y = row + cellEndValue;

                if (pToUnitData->ySpeed > 0.0)
                    pToUnitData->ySpeed = 0.0;
            }
            // Can not move cell up
            else
            {
                pToUnitData->y = row + cellBeginValue;

                if (pToUnitData->ySpeed < 0.0)
                    pToUnitData->ySpeed = 0.0;
            }
        }
    }
    else
    {
        pToUnitData->y = newY;
    }

    // X (column step)
    if (newColumn != column)
    {
        // If unit can go to cell
        if(MoveUnitTo(pToUnitData, newX, pToUnitData->y))
        {
            column = int(pToUnitData->x);
        }
        else
        {
            // Can not move cell right
            if (newColumn > column)
            {
                pToUnitData->x = column + cellEndValue;

                if (pToUnitData->xSpeed > 0.0)
                    pToUnitData->xSpeed = 0.0;
            }
            // Can not move cell left
            else
            {
                pToUnitData->x = column + cellBeginValue;

                if (pToUnitData->xSpeed < 0.0)
                    pToUnitData->xSpeed = 0.0;
            }
        }
    }
    else
    {
        pToUnitData->x = newX;
    }

    // Gravity
    pToUnitData->ySpeed += gravitySpeed * deltaTime;
    if (pToUnitData->ySpeed > gravitySpeed)
       pToUnitData->ySpeed = gravitySpeed;
}

void Update()
{
    clock_t clockNow = clock();
    clock_t deltaClock = clockNow - clockLastFrame;
    float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
    clockLastFrame = clockNow;

    framesCounter++;
    framesTimeCounter += deltaTime;
    if(framesTimeCounter >= 1.0)
    {
        framesTimeCounter -= 1.0;
        fps = framesCounter;
        framesCounter = 0;
    }

    if (IsKeyDown((VK_UP)))
        unitsData[heroIndex].yOrder = UnitOrder_Backward;
    else
        unitsData[heroIndex].yOrder = UnitOrder_None;

    if (IsKeyDown((VK_LEFT)))
        unitsData[heroIndex].xOrder = UnitOrder_Backward;
    else
    {
       if (IsKeyDown(VK_RIGHT))
       {
           unitsData[heroIndex].xOrder = UnitOrder_Forward;
       }
       else
           unitsData[heroIndex].xOrder = UnitOrder_None;
    }

    if (IsKeyDown(VK_SHIFT))
    {
        twoPlayer = true;
        Initialize();
    }

    if (twoPlayer)
    {
        if (IsKeyDown((0x57)))
            unitsData[twoheroIndex].yOrder = UnitOrder_Backward;
        else
            unitsData[twoheroIndex].yOrder = UnitOrder_None;

        if (IsKeyDown((0x41)))
            unitsData[twoheroIndex].xOrder = UnitOrder_Backward;
        else
        {
           if (IsKeyDown(0x44))
           {
               unitsData[twoheroIndex].xOrder = UnitOrder_Forward;
           }
           else
              unitsData[twoheroIndex].xOrder = UnitOrder_None;
        }
    }

    if (IsKeyDown(0x52))
    {
        score = 0;
        Initialize();
    }

    UpdateAI();

    for(int u = 0; u < unitsCount; u++)
    {
        UpdateUnit(&unitsData[u], deltaTime);
    }
    if (!twoPlayer)
    {
        if (unitsData[heroIndex].health <= 0)
        {
            score = 0;
            Initialize();
        }
    }
    else
    {
        if (unitsData[heroIndex].health <= 0 && unitsData[twoheroIndex].health <= 0)
        {
            score = 0;
            Initialize();
        }
    }
}

void Shutdown()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Green);
    printf("\tScore: %i \n\n \tYou completed all levels! Press Enter...", score);
    char a = getchar();
}

int main()
{
    SetupSystem();
    Initialize();

    do
    {
        Render();
        Update();
    }
    while(isGameActive);

    Shutdown();

    return 0;
}
