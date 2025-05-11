#pragma once

#include "games.h"

namespace test
{
    bool testingEnabled{};
    const std::string TESTING_ENABLED = "11111 111111111111 1111111!\n";
    const std::string CHOICE_TEAM_SPIRIT = "\n11111111 1111:\n11111111 1111111 1111111111 1111 11 -10 11 10: ";
    const std::string CHOICE_SCENE = "11111111 1111 111-11111 1111111 11 1 11 3.\n111-11111 11111: ";
    const std::string NONE_SCENE = "1111111 1 1111 111 111-1111!\n";
    const std::string TEST_THREE_PRESSING = "11111111 111111111:\n1.1111111 1111 - 40%.\n2.111111 \"11111111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_THREE_PERSONAL_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 40%.\n2.111111 \"111111 11111\" - (+15%)\n3.111111111 111 ";
    const std::string TEST_THREE_ZONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 40%.\n2.111111 \"111111 111111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_THREE_NONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 40%.\n2.111111 \"111 111111\" - (+20%)\n3.111111111 111 ";
    const std::string TEST_MEDIUM_PRESSING = "11111111 111111111:\n1.1111111 1111 - 50%.\n2.111111 \"11111111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_MEDIUM_PERSONAL_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 50%.\n2.111111 \"111111 11111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_MEDIUM_ZONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 50%.\n2.111111 \"111111 111111\" - (+15%)\n3.111111111 111 ";
    const std::string TEST_MEDIUM_NONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 50%.\n2.111111 \"111 111111\" - (+20%)\n3.111111111 111 ";
    const std::string TEST_LAY_UP_PRESSING = "11111111 111111111:\n1.1111111 1111 - 60%.\n2.111111 \"11111111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_LAY_UP_PERSONAL_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 60%.\n2.111111 \"111111 11111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_LAY_UP_ZONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 60%.\n2.111111 \"111111 111111\" - (+15%)\n3.111111111 111 ";
    const std::string TEST_LAY_UP_NONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 60%.\n2.111111 \"111 111111\" - (+20%)\n3.111111111 111 ";
    const std::string TEST_COMBINATION_PRESSING = "11111111 111111111:\n1.1111111 1111 - 55%.\n2.111111 \"11111111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_COMBINATION_PERSONAL_DEFENSE = "11111111 111111111 : \n1.1111111 1111 - 55 % .\n2.111111 \"111111 11111\" - (+15%)\n3.111111111 111 ";
    const std::string TEST_COMBINATION_ZONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 55%.\n2.111111 \"111111 111111\" - (-10%)\n3.111111111 111 ";
    const std::string TEST_COMBINATION_NONE_DEFENSE = "11111111 111111111:\n1.1111111 1111 - 55%.\n2.111111 \"1111 111111\" - (+20%)\n3.111111111 111 ";
    const std::string TEST_DIRTY = "11111111 111111111:\n11111111111 111111111 70%\n";
    const std::string TEST_HAND_GOD = "11111111 111111111:\n11111111111 111111111 100%\n";
}

namespace menu
{
    int startGame{};
    int jump{};
    int hint{};
    const int RULES_GAME{ 1 };
    const int ONE_GAME{ 2 };
    const int TOURNAMENT{ 3 };
    const int AUTHOR_GAME{ 4 };
    const int EXIT_GAME{ 5 };
    std::string rules =
        "Это баскетбольный клуб колледжа Алабама."
        "Ты будешь капитаном и плеймейкером нашей команды.\n"
        "Игра длится 2 тайма по 8 минут. Одна атака длится 24 секунды.\n\n";
    std::string rulesShot =
        "Делай броски следующим образом:\n"
        "1. Дальний (трехочковый) бросок в прыжке;\n2. Средний (двухочковый) бросок в прыжке;\n3. Лэй - апп (два очка);\n4. Комбинация и бросок (два очка);\n\n"
        "На попадание влияет:\n1. Защита.\n2. Командный дух.\n\n"
        "Вероятность попадания при лэй - аппе или комбинации выше, чем при дальнем и среднем броске,\nно во время их выполнения обороняющаяся команда может сделать перехват или блок-шот."
        "Эти приёмы выполняются случайным образом автоматически,\nи работают как для игрока так и для противника!\n\n"
        "Командный дух можно как поднять(отличной игрой и успешным решением жизненных вопросов команды),\nтак и потерять(плохой игрой или неудачными решениями).\n"
        "При максимальном подъеме командного духа(+10) открывается спецприем \"Рука бога\",\nкоторый гарантирует 100% попадание с трехочковой линии,\n"
        "но он, так же, тратит все пункты командного духа.\n"
        "При минимальном командном духе (-10) открывается спецприем \"Грязная игра\",\nкоторый с некоторой вероятностью может принести три очка,\n"
        "но будь аккуратен, ведь есть вероятность того, что судья заметит фол и соперник реализует штрафной,\nчто даст ему одно очко.\n"
        "\"Грязная игра\" также доступна и противнику, если он начинает проигрывать матч со значительным разрывом,\nна него действуют те же ограничения.\n\n";
    std::string rulesDefense =
        "Немаловажный момент игры - защита. Выбирай ее с умом, чтобы реализовать все свои задумки и тактики.\nВнимательно следи за тем, какую защиту использует противник!\n\n"
        "Чтобы изменить защиту во время матча, просто введите 0 в качестве следующего броска.\n\n"
        "Выбирай схему следующим образом:\n"
        "1. Прессинг - эффективная защита (шанс всех бросков снижен на 10%), но будь бдителен, ведь после двух защит в прессинге команда устает\n"
        "и следующие две атаки соперника, команда будет в состоянии \"Нет защиты\", чтобы восстановить силы!\n"
        "2. Личная опека - отличная защита от средних и ближних бросков (шанс удачного среднего броска и лэй - аппа -20%),\n"
        "но открывается огромный простор для дальних бросков и комбинаций (шанс удачного дальнего броска и комбинаций + 10 %);\n"
        "3. Зонная защита - отличная защита от дальних бросков и комбинаций (шанс удачного дальнего броска и комбинации -20%),\n"
        "но открывается огромный простор для лэй - аппов и средних бросков (шанс удачного лэй - аппа и средних бросков + 10%);\n"
        "4. Нет защиты - команда отдыхает в защите (повышается шанс удачной реализации всех бросков +10%);\n"
        "Противник будет менять свою защиту в зависимости от бросков, которые ты делаешь!\n"
        "Во время атаки отображается защита противника, а при атаке противника отображается защита игрока.\n\n";
    const std::string REPEAT = "Не понимаю!\nТвой выбор: ";
    const std::string OPPONENT_NAME_CHOICE = "Нажмите клавишу Enter для стандартного названия противника, или\nВведите название команды противника: ";
    const std::string START_DEFENSE = "Твоей стартовой защитой будет...";
    const std::string WIN_BALL_JUMP = "Судья подкидывает мяч вверх в центральном круге и...\nВбрасывание выигрывает команда: ";
    const std::string TIMEOUT = "\nЗвучит свисток, сейчас команды уйдут на перерыв!\nУслышимся после небольшой паузы!\n\n";
    const std::string SECOND_TIME = "Начинается второй тайм! ПОЕХАЛИ!!!\n\n";
    const std::string FINAL = "Последние секунды матча истекли! Судья даёт свисток!\nФинальный счёт на табло:\n";
    const std::string HOORAY = "Поздравляем с победой команду ";
    const std::string DRAW = "Сегодня победитель не выявлен, но в следующий раз победит сильнейший!\n\n";
    const std::string WELCOME = "Добро пожаловать в игру \"Баскетбол\"\n";
    const std::string START_MENU = "1. Правила игры.\n2. Начать игру.\n3. Режим турнира.\n4. Об авторе.\n5. Выйти из игры.\n\nДля продолжения выберите действие: ";
    const std::string CHOICE_HINT = "Выберите режим подсказок:\n1.Опытный(без подсказок).\n2.Любитель(подсказки появляются по нажатию клавиши)\n\nТвой выбор: ";
    const std::string AUTHOR = "Студия разработки игр Dialas представляет.\nАвтор: Медведенко Егор (ник: mee1b).\nВерсия: 2.0.3\n\n";
    const std::string TABLO = "Счёт: ";
}

namespace engine
{
    const int ATTACK_TIME{ 24 };
    const int THREE_POINT{ 3 };
    const int TWO_POINT{ 2 };
    const int FREE_THROW_POINT{ 1 };
    const int THREE_POINT_AND_PRESSING{ 70 };
    const int THREE_POINT_AND_PERSONAL_DEFENSE{ 55 };
    const int THREE_POINT_AND_ZONE_DEFENSE{ 80 };
    const int THREE_POINT_AND_NONE_DEFENSE{ 50 };
    const int MEDIUM_SHOT_AND_PRESSING{ 60 };
    const int MEDIUM_SHOT_AND_PERSONAL_DEFENSE{ 70 };
    const int MEDIUM_SHOT_AND_ZONE_DEFENSE{ 40 };
    const int MEDIUM_SHOT_AND_NONE_DEFENSE{ 40 };
    const int LAY_UP_AND_PRESSING{ 50 };
    const int LAY_UP_AND_PERSONAL_DEFENSE{ 60 };
    const int LAY_UP_AND_ZONE_DEFENSE{ 30 };
    const int LAY_UP_AND_NONE_DEFENSE{ 30 };
    const int COMBINATION_AND_PRESSING{ 65 };
    const int COMBINATION_AND_PERSONAL_DEFENSE{ 45 };
    const int COMBINATION_AND_ZONE_DEFENSE{ 70 };
    const int COMBINATION_AND_NONE_DEFENSE{ 40 };
    const int PROCENT_REBOUND{ 90 };
    const int PERIOD_FINISH{ 240 };
    const int PERIOD_START{ 0 };
    const int GOOD_STEAL_OPPONENT_ON_PLAYER{ 80 };
    const int GOOD_BLOCK_OPPONENT_ON_PLAYER{ 75 };
    const int SWITCH_DEFENSE{ 1 };
    const int ZERO{ 0 };
    const int ONE_UP{ 1 };
    const int ALL_TOURNAMENT_TEAM{ 8 };
    const int HIGH_RAND{ 100 };
    const int FOUR{ 4 };
    int allTeamTournament{ 8 };
    int probalityJump{};
    int probalityRebound{};
    int period{};
    int userChoice{};
    int howGame{ 1 };
    int resoultTournament{};
    int countPersonalDefense{};
    int countZoneDefense{};
    const int ALL_TOURNAMENT_GAME{ 3 };
    const int MAX_DRAW{ 2 };
    std::string userText{};
    const std::string EMPTY_STRING = "";
    const char COMMENT_CHAR = '*';
    const std::string USER_HISTORY_FILE = "text_game.txt";
}

namespace history
{
const int PROBALITY_WIN{ 50 }; // 50% шанс победы в событии
const int ADD_SPIRIT_VICTORY{ 10 };
const int REM_SPIRIT_FAIL{ -10 };
const int NONE_RISC_SPIRIT{ -5 };
const std::string YOUR_TEAM_SPIRIT = "\nКомандный дух равен ";
const std::string STANDART_OPPONENT_NAME = "Колледж Оклахомы";
const std::string PLAYER_TEAM_NAME = "Колледж Алабамы";
const std::string LOSE_TOURNAMENT = "К сожалению, ты проиграл турнир.\nНо не вешай нос! Этот турнир не последний!\nУдачи в следующий раз!";
const std::string DRAW_TOURNAMENT = "К сожалению, ты проиграл турнир. Из-за большого количества ничьих не хватило очков.\nНо не вешай нос! Этот турнир не последний!\nУдачи в следующий раз!";
const std::string WIN_TOURNAMENT = "Вот он ЧЕМПИОН!!!\nСегодня все поют песни только в твою честь!\nЛюбят только тебя и твоих товарищей!\nМедали на шее, но помни расслабляться рано! Впереди ждут новые победы!!!";
const std::string FIRST_GAME = "Первая игра турнира против команды ";
const std::string SECOND_GAME = "Полуфинал турнира против команды ";
const std::string LAST_GAME = "Финальная игра турнира против команды ";
}

namespace attack
{
const std::string SHOT_CHOICE = "Капитан! Какой бросок делаем в этой атаке? ";
const std::string SHOT_CHOICE_AND_HINT = "Капитан! Какой бросок делаем в этой атаке? (Чтобы посмотреть виды бросков, нажми 5): ";
const std::string SUPERPOWER_HINT = "Если командный дух равен -10 или меньше:\n6.Грязная игра (три очка, плюс 10 к командному духу).\nЕсли командный дух равен 10 или больше:\n7.Рука бога (три очка, плюс 10 к командному духу).\n\n";
const std::string UNKNOW_TACTICS = "На тренировках мы не разбирали такие броски, капитан!\nДавай сыграем то, что мы уже знаем!\n";
const std::string OPEN_DIRTY = "6.Командный дух падает, открыт приём \"Грязная игра\".\n";
const std::string OPEN_HAND = "7.Командный дух на подъёме, открыт приём \"Рука бога\".\n";
const std::string THREE_POINT = "Трехочковый бросок!!!\n";
const std::string ADD_THREE = "Три очка в корзине!!!\n";
const std::string LOSE_SHOT = "Промах! Мяч в воздухе, кто же им завладеет?\n";
const std::string MEDIUM_SHOT = "Средний бросок!!!\n";
const std::string ADD_TWO = "Два очка в корзине!!!\n";
const std::string LAY_UP = "Это же лэй-ап!!!\n";
const std::string COMBINATION = "Смотрите, игроки разыгрывают комбинацию!!!\n";
const std::string ADD_HAND = "КАКАЯ ТРАЕКТОРИЯ ПОЛЕТА!!!\nНе бросок, а заглядение.\nТри очка в корзине!\n\n";
const std::string RESET_HAND = "Это был великолепный бросок!\nНо видимо он отнял много сил у игроков, перед следующим броском команде нужно собраться!\n\n";
const std::string ADD_DIRTY = "ЭТО ЖЕ ФОЛ!!!\nНо судья ничего не видит.\nТри очка в корзине!\n\n";
const std::string FAIL_DIRTY = "ЭТО ЖЕ ФОЛ!!!\nСудья назначает штрафной бросок в кольцо команды ";
const std::string REALESE_DIRTY_PENALTY = " реализует штрафной бросок!\n\n";
const std::string IN_ATTACK = " в атаке:\n";
const std::string REBOUND_IN_ATTACK = "\nПодбор в атаке за командой: ";
const std::string STEAL_GOOD = "Мяч перехвачен!\n\nМяч достаётся команде ";
const std::string BLOCK_GOOD = "Вот это БЛОК-ШОТ!\n\nМяч достаётся команде ";
bool steal{ false };
bool block{ false };

}

namespace defend
{
const std::string SHOW_HINT = "(Чтобы посмотреть схемы защиты нажми 5.)";
const std::string QUESTION_DEFENSE = "Какой будет наша защита? ";
const std::string UNKNOW_TACTICS = "На тренировках мы не разбирали эти схемы, капитан!\nДавай сыграем то, что мы уже знаем!\n";
const std::string REBOUND_IN_DEFENSE = "\nПодбор в защите за командой: ";
const std::string DEFENSE_TACTICS = "Защита команды ";
const std::string PRESSING_OPPONENT = " - прессинг\n";
const std::string PERSONAL_OPPONENT = " - личная опека\n";
const std::string TEAM_OPPONENT = "Команда ";
const std::string ZONE_OPPONENT = " играет зонную защиту\n";
const std::string NONE_DEFENSE_OPPONENT = " расслабилась - у них нет защиты\n";
const std::string PLAYER_FATIGUE = " устала от прессинга, сейчас у них нет защиты!\n";
const std::string PLAYER_RELAX = " на пике сил, может стоит задуматься о игре в защите?!\n";
const std::string RELAX = "\nКапитан, команда сильно устала в прессинге!\nНужен отдых!\n\n";
int fatigue = 0;
const int SEVERE_FATIGUE = 2;
}
