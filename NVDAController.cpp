#include <windows.h>         // Заголовок Windows API — нужен для типов и функций Windows,
// может использоваться при работе с DLL.
#include <QString>           // Qt строковый класс.
#include <QDebug>            // Для отладочного вывода (qDebug, qWarning).
#include <QAccessible>       // Поддержка доступности в Qt.
#include <QAccessibleEvent>  // События доступности в Qt.

#include "NVDAController.h"  // Заголовок для функций NVDA (определение типов функций DLL).
#include "LoggerMacros.h"

// ======================
// Класс Speaker — отвечает за озвучку текста через NVDA.
// Работает с DLL nvdaControllerClient, использует таймер для показа текста построчно.
// ======================
Speaker::Speaker(QObject* parent) :
    QObject(parent),                   // Передаём parent в QObject.
    m_nvdaLib("nvdaControllerClient")  // Указываем имя DLL для динамической загрузки.
{
    // Настраиваем таймер, который используется для поочередной озвучки строк.
    m_timer.setSingleShot(true); // Один вызов по таймеру → потом останавливается.

    // Подключаем сигнал таймера к приватному слоту onTimeout().
    connect(&m_timer, &QTimer::timeout, this, &Speaker::onTimeout);

    // Пытаемся загрузить библиотеку NVDA при создании объекта.
    if(!loadNVDA())
        qWarning() << "Не загрузилась NVDA"; // Если не удалось — предупреждение в лог.
}

Speaker::~Speaker()
{
    // Если библиотека была загружена → выгружаем её в деструкторе.
    if(m_nvdaLib.isLoaded())
        m_nvdaLib.unload();
}

// ======================
// Метод загрузки NVDA.
// Подгружает DLL nvdaControllerClient и получает указатель на функцию nvdaController_speakText.
// ======================
bool Speaker::loadNVDA()
{
    LOG_FUNC_START();
    // Загружаем DLL.
    if(!m_nvdaLib.load())
    {
        LOG_ERR(QString("Не загрузилась nvdaControllerClient64.dll:" + m_nvdaLib.errorString())); // Выводим ошибку если не загрузилась.
        return false;
    }

    // Получаем указатель на функцию озвучки nvdaController_speakText.
    m_speakTextFunc = (NVDAController_speakText_t)m_nvdaLib.resolve("nvdaController_speakText");

    if(!m_speakTextFunc) // Если функция не найдена.
    {
        LOG_ERR(QString("Не загрузилось NVDAController_speakText function in DLL"));
        m_nvdaLib.unload(); // Выгружаем библиотеку.
        return false;
    }

    LOG_FUNC_END(QString("Все окей! NVDA!")); // Сообщаем, что библиотека успешно загружена.
    return true;
}

// ======================
// Озвучка текста через NVDA.
// Текст разбивается на строки и озвучивается построчно через таймер.
// ======================
void Speaker::speakTextNVDA(const QString &text)
{
    LOG_FUNC_START();
    if(!m_speakTextFunc) // Если функция не найдена → озвучка невозможна.
    {
        ++i; // Счётчик ошибок.
        LOG_WARN(QString("NVDA speak недоступна!"));
        return;
    }

    // Разбиваем текст на отдельные строки (разные варианты \r, \n).
    m_lines = text.split(QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);

    if(m_lines.isEmpty()) // Если строка пустая — смысла озвучивать нет.
    {
        LOG_INFO(QString("Пустая строка на озвучке"));
        return;
    }

    // Начинаем с первой строки.
    m_index = 0;
    speakNextLine(); // Запускаем процесс озвучки.
    LOG_FUNC_END(QString("Старт озвучки!"));
}

// ======================
// Прерывание озвучки.
// Сбрасывает таймер и очищает список строк.
// ======================
void Speaker::stopSpeak()
{
    m_timer.stop();   // Останавливаем таймер.
    m_lines.clear();  // Чистим список строк.
    m_index = 0;      // Сбрасываем индекс.
}

// ======================
// Озвучка с fallback (резервным поведением).
// Если NVDA не озвучивает, то мы следим таймером и отключаемся.
// ======================
void Speaker::speakWithFallback(QWidget* widget, const QString &text)
{
    LOG_FUNC_START();
    stopSpeak(); // Сначала сбрасываем старую озвучку.

    speakTextNVDA(text); // Пытаемся озвучить текст.

    // Создаём таймер-наблюдатель (с parent = widget,
    // чтобы таймер автоматически уничтожился вместе с виджетом).
    QTimer* checker = new QTimer(widget);
    checker->setInterval(300); // Интервал проверки (300 мс).

    // Подключаем таймер к лямбде, которая проверяет состояние NVDA.
    connect(checker, &QTimer::timeout, widget, [=]()
            {
                // Если виджет уже уничтожен или скрыт — останавливаем таймер.
                if(!widget || !widget->isVisible())
                {
                    LOG_INFO(QString("Виджет уже уничтожен или скрыт"));
                    checker->stop();
                    checker->deleteLater(); // Освобождаем таймер.
                    return;
                }

                // Если NVDA перестала читать → тоже останавливаем таймер.
                if(!isSpeakNVDA)
                {
                    LOG_WARN(QString("NVDA перестала читать!"));
                    checker->stop();
                    checker->deleteLater();
                }
            });

    checker->start(); // Запускаем проверку.
    LOG_FUNC_END(QString("Проверка fallback запущена!"));
}

// ======================
// Озвучка следующей строки.
// Таймер вызывает этот метод, пока строки не закончатся.
// ======================
void Speaker::speakNextLine()
{
    LOG_FUNC_START();
    // Если индекс вышел за пределы строк → всё закончено.
    if (m_index >= m_lines.size())
    {
        m_timer.stop();     // Останавливаем таймер.
        isSpeakNVDA = false; // Ставим флаг "не читаем".
        LOG_INFO(QString("Все озвучено!"));
        return;
    }

    isSpeakNVDA = true; // Устанавливаем флаг "NVDA читает".

    // Если это первый вызов → просто запускаем таймер без чтения (задержка 2 секунды).
    if(m_index == 0)
    {
        ++m_index;        // Увеличиваем индекс.
        m_timer.start(2000); // Ждём 2 секунды перед началом.
        LOG_INFO(QString("Первый вызов. Таймер задержки запущен!"));
        return;
    }

    // Берём текущую строку для озвучки.
    const QString& currentLine = m_lines.at(m_index);
    std::wstring wline = currentLine.toStdWString(); // Преобразуем в wide-string для NVDA.

    // Вызываем функцию NVDA для озвучки строки.
    int res = m_speakTextFunc(wline.c_str());

    if(res != 0)
    {
        LOG_WARN(QString("NVDA не прочел строку " + currentLine));
    }

    ++m_index; // Переходим к следующей строке.

    // Вычисляем задержку перед следующей строкой (динамически от длины строки).
    int delayTime = 200 + (50 * currentLine.length());
    if(delayTime < 300) delayTime = 300; // Минимум 300 мс.

    m_timer.start(delayTime); // Запускаем таймер.
    LOG_FUNC_END(QString("Запущена задержка " + QString::number(delayTime) + " перед следующей строкой!"));
}

// ======================
// Слот, вызываемый при срабатывании таймера.
// Просто вызывает speakNextLine().
// ======================
void Speaker::onTimeout()
{
    speakNextLine();
}
