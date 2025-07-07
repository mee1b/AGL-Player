#pragma once
#include <QString>

namespace test
{
bool testingEnabled{};
const QString TESTING_ENABLED = "РЕЖИМ ТЕСТИРОВЩИКА ВКЛЮЧЕН!\n";
const QString CHOICE_TEAM_SPIRIT = "\nТЕСТОВОЕ МЕНЮ:\nВыберите уровень командного духа от -10 до 10: ";
const QString CHOICE_SCENE = "Выберите одну кат-сцену цифрами от 1 до 3.\nКат-сцена номер: ";
const QString NONE_SCENE = "Переход к игре без кат-сцен!\n";
const QString TEST_THREE_PRESSING = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 40%.\n2.Защита \"Прессинг\" - (-10%)\n3.Командный дух ";
const QString TEST_THREE_PERSONAL_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 40%.\n2.Защита \"Личная опека\" - (+15%)\n3.Командный дух ";
const QString TEST_THREE_ZONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 40%.\n2.Защита \"Зонная защита\" - (-10%)\n3.Командный дух ";
const QString TEST_THREE_NONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 40%.\n2.Защита \"Нет защиты\" - (+20%)\n3.Командный дух ";
const QString TEST_MEDIUM_PRESSING = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 50%.\n2.Защита \"Прессинг\" - (-10%)\n3.Командный дух ";
const QString TEST_MEDIUM_PERSONAL_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 50%.\n2.Защита \"Личная опека\" - (-10%)\n3.Командный дух ";
const QString TEST_MEDIUM_ZONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 50%.\n2.Защита \"Зонная защита\" - (+15%)\n3.Командный дух ";
const QString TEST_MEDIUM_NONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 50%.\n2.Защита \"Нет защиты\" - (+20%)\n3.Командный дух ";
const QString TEST_LAY_UP_PRESSING = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 60%.\n2.Защита \"Прессинг\" - (-10%)\n3.Командный дух ";
const QString TEST_LAY_UP_PERSONAL_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 60%.\n2.Защита \"Личная опека\" - (-10%)\n3.Командный дух ";
const QString TEST_LAY_UP_ZONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 60%.\n2.Защита \"Зонная защита\" - (+15%)\n3.Командный дух ";
const QString TEST_LAY_UP_NONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 60%.\n2.Защита \"Нет защиты\" - (+20%)\n3.Командный дух ";
const QString TEST_COMBINATION_PRESSING = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 55%.\n2.Защита \"Прессинг\" - (-10%)\n3.Командный дух ";
const QString TEST_COMBINATION_PERSONAL_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ : \n1.Базовый шанс - 55 % .\n2.Защита \"Личная опека\" - (+15%)\n3.Командный дух ";
const QString TEST_COMBINATION_ZONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 55%.\n2.Защита \"Зонная защита\" - (-10%)\n3.Командный дух ";
const QString TEST_COMBINATION_NONE_DEFENSE = "ТЕСТОВОЕ СООБЩЕНИЕ:\n1.Базовый шанс - 55%.\n2.Защита \"ЛНет защиты\" - (+20%)\n3.Командный дух ";
const QString TEST_DIRTY = "ТЕСТОВОЕ СООБЩЕНИЕ:\nВероятность попадания 70%\n";
const QString TEST_HAND_GOD = "ТЕСТОВОЕ СООБЩЕНИЕ:\nВероятность попадания 100%\n";
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
const QString repeatStart = "Введите \"начать\" для запуска игры!";
const QString rules =
    "\nЭто баскетбольный клуб колледжа Алабама.\n"
    "Ты будешь капитаном и плеймейкером нашей команды.\n";
const QString rulesShot =
    "Для смены защиты в игре нажми 0\n"
    "Делай броски следующим образом:\n"
    "1. Дальний (трехочковый) бросок в прыжке;\n2. Средний (двухочковый) бросок в прыжке;\n3. Лэй - апп (два очка);\n4. Комбинация и бросок (два очка);\n\n";
const QString rulesDefense =
    "Выбирай схему следующим образом:\n"
    "1. Прессинг - эффективна против трех очковых бросков и комбинаций,\nмалоэффективна против двухочковых бросков и лэй - аппов\n"
    "2. Личная опека - эффективна против лэй - аппов и комбинаций,\nмалоэффективна против двухочковых и трехочковых бросков\n"
    "3. Зонная защита - эффективна против двухочковых и техочковых\nбросков, малоэффективна против лэй - аппов и комбинаций\n"
    "4. Нет защиты - неэффективна против всех видов бросков\n";
const QString REPEAT = "Не понимаю!\nТвой выбор: ";
const QString OPPONENT_NAME_CHOICE = "Нажмите клавишу Enter для стандартного названия противника, или\nВведите название команды противника: ";
const QString START_DEFENSE = "Твоей стартовой защитой будет...";
const QString WIN_BALL_JUMP = "Судья подкидывает мяч вверх в центральном круге и...\nВбрасывание выигрывает команда: ";
const QString TIMEOUT = "\nЗвучит свисток, сейчас команды уйдут на перерыв!\nУслышимся после небольшой паузы!\n\n";
const QString SECOND_TIME = "Начинается второй тайм! ПОЕХАЛИ!!!\n\n";
const QString FINAL = "Последние секунды матча истекли! Судья даёт свисток!\nФинальный счёт на табло:\n";
const QString HOORAY = "Поздравляем с победой команду ";
const QString DRAW = "Сегодня победитель не выявлен, но в следующий раз победит сильнейший!\n\n";
const QString WELCOME = "Добро пожаловать в игру \"Баскетбол\"\nДля начала игры введите команду \"начать\"\nДля выхода введите команду \"выход\"";
const QString RULES_IN_GAME = "Каждая защита имеет свои бонусы и недостатки, выбирай с умом.\nУ тебя и у соперника есть командный дух, который повышается при попдании и уменьшается,\nесли промазал!\nЧем выше командный дух, тем больше шансов попасть и наоборот!\n";
const QString START_MENU = "1. Правила игры.\n2. Начать игру.\n3. Режим турнира.\n4. Об авторе.\n5. Выйти из игры.\n\nДля продолжения выберите действие: ";
const QString CHOICE_HINT = "Выберите режим подсказок:\n1.Опытный(без подсказок).\n2.Любитель(подсказки появляются по нажатию клавиши)\n\nТвой выбор: ";
const QString AUTHOR = "Студия разработки игр Dialas представляет.\nАвтор: Медведенко Егор (ник: mee1b).\nВерсия: 2.0.3\n\n";
const QString TABLO = "Счёт: ";
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
const int PROCENT_REBOUND{ 80 };
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
QString userText{};
const QString EMPTY_STRING = "";
const char COMMENT_CHAR = '*';
const QString USER_HISTORY_FILE = "text_game.txt";
}

namespace history
{
const int PROBALITY_WIN{ 50 }; // 50% шанс победы в событии
const int ADD_SPIRIT_VICTORY{ 10 };
const int REM_SPIRIT_FAIL{ -10 };
const int NONE_RISC_SPIRIT{ -5 };
const QString YOUR_TEAM_SPIRIT = "\nКомандный дух равен ";
const QString STANDART_OPPONENT_NAME = "Колледж Оклахомы";
const QString PLAYER_TEAM_NAME = "Колледж Алабамы";
const QString LOSE_TOURNAMENT = "К сожалению, ты проиграл турнир.\nНо не вешай нос! Этот турнир не последний!\nУдачи в следующий раз!";
const QString DRAW_TOURNAMENT = "К сожалению, ты проиграл турнир. Из-за большого количества ничьих не хватило очков.\nНо не вешай нос! Этот турнир не последний!\nУдачи в следующий раз!";
const QString WIN_TOURNAMENT = "Вот он ЧЕМПИОН!!!\nСегодня все поют песни только в твою честь!\nЛюбят только тебя и твоих товарищей!\nМедали на шее, но помни расслабляться рано! Впереди ждут новые победы!!!";
const QString FIRST_GAME = "Первая игра турнира против команды ";
const QString SECOND_GAME = "Полуфинал турнира против команды ";
const QString LAST_GAME = "Финальная игра турнира против команды ";
}

namespace attack
{
const QString SHOT_CHOICE = "Капитан! Какой бросок делаем в этой атаке? ";
const QString SHOT_CHOICE_AND_HINT = "Капитан! Какой бросок делаем в этой атаке? (Чтобы посмотреть виды бросков, нажми 5): ";
const QString SUPERPOWER_HINT = "Если командный дух равен -10 или меньше:\n6.Грязная игра (три очка, плюс 10 к командному духу).\nЕсли командный дух равен 10 или больше:\n7.Рука бога (три очка, плюс 10 к командному духу).\n\n";
const QString UNKNOW_TACTICS = "На тренировках мы не разбирали такие броски, капитан!\nДавай сыграем то, что мы уже знаем!\n";
const QString OPEN_DIRTY = "6.Командный дух падает, открыт приём \"Грязная игра\".\n";
const QString OPEN_HAND = "7.Командный дух на подъёме, открыт приём \"Рука бога\".\n";
const QString THREE_POINT = "Трехочковый бросок!!!\n";
const QString ADD_THREE = "Три очка в корзине!!!\n";
const QString LOSE_SHOT = "Промах! Мяч в воздухе, кто же им завладеет?\n";
const QString MEDIUM_SHOT = "Средний бросок!!!\n";
const QString ADD_TWO = "Два очка в корзине!!!\n";
const QString LAY_UP = "Это же лэй-ап!!!\n";
const QString COMBINATION = "Смотрите, игроки разыгрывают комбинацию!!!\n";
const QString ADD_HAND = "КАКАЯ ТРАЕКТОРИЯ ПОЛЕТА!!!\nНе бросок, а заглядение.\nТри очка в корзине!\n\n";
const QString RESET_HAND = "Это был великолепный бросок!\nНо видимо он отнял много сил у игроков, перед следующим броском команде нужно собраться!\n\n";
const QString ADD_DIRTY = "ЭТО ЖЕ ФОЛ!!!\nНо судья ничего не видит.\nТри очка в корзине!\n\n";
const QString FAIL_DIRTY = "ЭТО ЖЕ ФОЛ!!!\nСудья назначает штрафной бросок в кольцо команды ";
const QString REALESE_DIRTY_PENALTY = " реализует штрафной бросок!\n\n";
const QString IN_ATTACK = " в атаке:\n";
const QString REBOUND_IN_ATTACK = "\nПодбор в атаке за командой: ";
const QString STEAL_GOOD = "Мяч перехвачен!\n\nМяч достаётся команде ";
const QString BLOCK_GOOD = "Вот это БЛОК-ШОТ!\n\nМяч достаётся команде ";
bool steal{ false };
bool block{ false };

}

namespace defend
{
const QString SHOW_HINT = "(Чтобы посмотреть схемы защиты нажми 5.)";
const QString QUESTION_DEFENSE = "Какой будет наша защита? ";
const QString UNKNOW_TACTICS = "На тренировках мы не разбирали эти схемы, капитан!\nДавай сыграем то, что мы уже знаем!\n";
const QString REBOUND_IN_DEFENSE = "\nПодбор в защите за командой: ";
const QString DEFENSE_TACTICS = "Защита команды ";
const QString PRESSING_OPPONENT = " - прессинг\n";
const QString PERSONAL_OPPONENT = " - личная опека\n";
const QString TEAM_OPPONENT = "Команда ";
const QString ZONE_OPPONENT = " играет зонную защиту\n";
const QString NONE_DEFENSE_OPPONENT = " расслабилась - у них нет защиты\n";
const QString PLAYER_FATIGUE = " устала от прессинга, сейчас у них нет защиты!\n";
const QString PLAYER_RELAX = " на пике сил, может стоит задуматься о игре в защите?!\n";
const QString RELAX = "\nКапитан, команда сильно устала в прессинге!\nНужен отдых!\n\n";
int fatigue = 0;
const int SEVERE_FATIGUE = 2;
}
