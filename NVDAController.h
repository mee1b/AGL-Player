#pragma once

#include <QObject>
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QLibrary>
#include <QRegularExpression>

#include "talkwrap.h"

// Определяем тип функции для взаимодействия с NVDA через NVDAController.dll
// NVDAController_speakText_t — это указатель на функцию, которая принимает wchar_t* (строку Unicode)
// и возвращает long. __stdcall — соглашение вызова, используемое в Windows DLL.
using NVDAController_speakText_t = long (__stdcall *)(const wchar_t*);

class Speaker : public QObject
{
    Q_OBJECT
public:
    explicit Speaker(QObject *parent = nullptr); // Конструктор, инициализация таймера и NVDA
    ~Speaker(); // Деструктор, очистка ресурсов

    // Метод для принудительной озвучки текста через NVDA
    void speakTextNVDA(const QString& text);

    // Метод для остановки текущей озвучки NVDA
    void stopSpeak();

    // Метод для озвучки текста с fallback:
    // если NVDA недоступна, используется стандартная система озвучки (TalkWrap)
    void speakWithFallback(QWidget* widget, const QString& text);

private slots:
    void onTimeout(); // Слот срабатывает по таймеру, отвечает за последовательное озвучивание

private:
    bool loadNVDA(); // Загружает библиотеку NVDAController.dll и получает указатель на функцию speakText
    void speakNextLine(); // Озвучивает следующую строку из списка, используется для разбития текста на части

private:
    QStringList m_lines; // Список строк для последовательной озвучки
    int m_index = 0;     // Текущий индекс строки в m_lines
    int i = 0;           // Дополнительный счетчик/индекс (возможно для внутренних нужд)

    QTimer m_timer;      // Таймер для разбивки текста на строки и озвучки по частям
    QLibrary m_nvdaLib;  // Объект для динамической загрузки NVDAController.dll
    NVDAController_speakText_t m_speakTextFunc = nullptr; // Указатель на функцию speakText из DLL
    TalkWrap m_talker;   // Запасной/стандартный механизм озвучки
    bool isSpeakNVDA = true; // Флаг: используем NVDA (true) или fallback (false)
};
