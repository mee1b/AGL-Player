#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "enums.h"
#include "structs.h"
#include "history.h"

std::ofstream record;
std::stringstream gameMessage;

extern void recording(std::string comment);
extern void recording(int comment);
extern void userComment(std::string userText, std::string gameText, int& userChoice);
extern void userComment(std::string gameText, std::string& userChoice);
extern void hints();
extern void author();
extern void gameRules();
extern void deleteName(std::vector<std::string>& namesTeamOpponent, int choiceTeamName);
