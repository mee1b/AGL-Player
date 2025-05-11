#pragma once

#include "namespace.h"

struct Situation
{
    int probalityVictory{};
    int playerChoice{};
    const int ACTION{ 1 };
    const int UNACTION{ 2 };
    std::string scene{};
    std::string choice{};
    std::string riscNone{};
    std::string riscVictory{};
    std::string riscFail{};
};

struct Opponent
{
    int score{};
    int hit{};
    int shot{};
    int defense{};
    int teamSpiritOpponent{ 0 };
    int probalityDirtyGame{};
    const int PROCENT_DIRTY_GAME = 50;
    const int DIRTY_DEEP = 5;
    const int ENTER_ON_DIRTY = 30;
    std::string name{};
};

struct Player
{
    int score{};
    int hit{};
    int shot{};
    int defense{};
    int teamSpirit{};
    int probalityDirtyGame{};
    const int PROCENT_DIRTY_GAME{ 30 };
    std::string name{};
};
