#pragma once
#include <QString>

struct Opponent
{
    int score{};
    int hit{};
    int shot{};
    int defense{};
    int teamSpiritOpponent{ 0 };
    int probalityDirtyGame{};
    const int PROCENT_DIRTY_GAME = { 50 };
    const int DIRTY_DEEP { 5 };
    const int ENTER_ON_DIRTY { 30 };
    QString name{};
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
    QString name{};
};
