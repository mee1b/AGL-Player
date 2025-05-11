#pragma once
enum class winOrLose
{
    PLAYER_WIN,
    DRAW,
    PLAYER_LOSE
};

enum class shots
{
    THREE_POINT_SHOT = 1,
    MEDIUM_SHOT,
    LAY_UP_SHOT,
    COMBINATIONT_SHOT,
    RULES_SHOT,
    DIRTY_SHOT,
    HAND_GOD_SHOT
};

enum class defense
{
    CHOICE_DEFENSE,
    PRESSING,
    PERSONAL_DEFENSE,
    ZONE_DEFENSE,
    NONE_DEFENSE,
    RULES_DEFENSE
};

enum class playerHints
{
    EXPERT = 1,
    AMATEUR
};

enum class spirit
{
    MAX_SPIRIT = 10,
    MIN_SPIRIT = -10,
    DIRTY_SPIRIT = -10,
    GOD_SPIRIT = 10,
    MORE_POINT = 5,
};

enum class posessionBall
{
    PLAYER_BALL = 1,
    OPPONENT_BALL,
    PROCENT_BALL_PLAYER = 40

};

enum class situationShow
{
    FIRST_STORY = 1,
    SECOND_STORY,
    THIRD_STORY
};

enum class testing
{
    TEST = 1,
    UNTEST
};