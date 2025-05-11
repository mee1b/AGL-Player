#pragma once

#include <string>

struct Player;
struct Opponent;

extern int tournament(Player& player, Opponent& opponent);
extern void startMenu(Player& player, Opponent& opponent);
extern void choiceDefense(int& defense);
extern void choiceDefenseOpponent(int& defense);
extern void jumpBall(int& jump);
extern void probabilityHitPlayer(int& hit, int teamSpirit);
extern void probabilityHitOpponent(int& hit, int teamSpiritOpponent);
extern void probalityStealOpponentOnPlayer(bool& steal);
extern void probalityBlockOpponentOnPlayer(bool& block);
extern void attackShot(int& shot, int teamSpirit);
extern bool playerAttack(Player& player, Opponent& opponent);
extern bool opponentAttack(Player& player, Opponent& opponent);
extern bool rebound();
extern void game(Player& player, Opponent& opponent);
extern void score(int scorePlayer, int scoreOpponent);
extern void switchDefenseOpponent(const Player& player, Opponent& opponent);
extern void showDefense(int defense, std::string name);
