#include "talkwrap.h"  // Подключаем свой заголовок с объявлением класса TalkWrap

// ======================
// Блок заглушек, если не используется talk.dll
// ======================
#ifndef USE_TALK_DLL
// Если макрос USE_TALK_DLL не определён, мы делаем "пустую реализацию" класса TalkWrap,
// чтобы код, который его использует, компилировался, но ничего не делал.

// Конструктор — создаёт объект с пустым reader_ и valid_ = false
TalkWrap::TalkWrap() : reader_(), valid_(false) {}

// Возвращает имя текущего screen reader'а. Так как DLL не используется — всегда пусто.
QString TalkWrap::currentReader() const {
    return QString();
}

// Метод озвучки — ничего не делает, так как DLL нет.
void output(const QString& text, bool interrupt) {}

// Деструктор — просто заглушка.
TalkWrap::~TalkWrap() {}
#else
// ======================
// Блок, когда talk.dll используется
// ======================
#include <QDebug>      // Для отладочного вывода
#include "talk/Tolk.h" // Заголовок DLL talk/Tolk — содержит функции работы с Talk и screen reader

// ======================
// Функция инициализации DLL Talk и определение активного screen reader
// ======================
static QString initTalkDll()
{
    Tolk_Load(); // Загружаем DLL (инициализация)

    // Выводим информацию о статусе DLL и поддержке речи
    qDebug() << "[Tolk] DLL loaded: "
             << Tolk_IsLoaded()            // Загружена ли DLL?
             << '\n'
             << Tolk_DetectScreenReader()  // Определяет, какой скринридер активен
             << '\n'
             << Tolk_HasSpeech();          // Поддерживает ли озвучку

    Tolk_PreferSAPI(false); // Настройка использования SAPI (False = не использовать SAPI, только нативные драйверы)

    // Определяем активный screen reader
    const wchar_t *win_name = Tolk_DetectScreenReader();
    if (win_name) // Если найден активный драйвер
    {
        QString name = QString::fromStdWString(win_name); // Конвертируем wide-string в QString
        qDebug() << "Talk: the active screen reader driver is: " << name;

        if (!Tolk_HasSpeech()) // Проверяем, поддерживает ли драйвер озвучку
        {
            qDebug() << "Talk: error! Driver not support speech output! ";
            return QString(); // Возвращаем пустую строку — речи нет
        }
        return name; // Возвращаем имя найденного screen reader
    }

    // Если ни один поддерживаемый screen reader не запущен
    qDebug() << "Talk: none of the supported screen readers is running";
    return QString(); // Возвращаем пустую строку
}

// ======================
// Конструктор TalkWrap с использованием DLL
// ======================
TalkWrap::TalkWrap():
    reader_(initTalkDll()),         // Вызываем initTalkDll для инициализации и получения активного драйвера
    valid_(!reader_.isEmpty())      // Если драйвер найден — valid_ = true, иначе false
{
}

// Возвращает имя текущего screen reader
QString TalkWrap::currentReader() const
{
    return reader_;
}

// ======================
// Вывод текста через Talk
// ======================
void TalkWrap::output(const QString &text, bool interrupt)
{
    // Если DLL успешно загружена и текст не пустой
    if (valid_ && !text.isEmpty())
    {
        // Вызываем функцию Talk для вывода речи
        // text.toStdWString().data() — конвертация QString в wchar_t*
        // interrupt — флаг, прерывать ли текущую речь
        Tolk_Output(text.toStdWString().data(), interrupt);
    }
}

// ======================
// Деструктор TalkWrap
// ======================
TalkWrap::~TalkWrap()
{
    if (valid_) {      // Если DLL была загружена и активен драйвер
        Tolk_Unload(); // Выгружаем DLL и освобождаем ресурсы
    }
}
#endif

