#include <windows.h>
#include <QString>
#include <QDebug>
#include <QAccessible>
#include <QAccessibleEvent>
#include "NVDAController.h"

Speaker::Speaker(QObject* parent) :
    QObject(parent),
    m_nvdaLib("nvdaControllerClient")
{
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &Speaker::onTimeout);

    if(!loadNVDA()) qWarning() << "Не загрузилась NVDA";
}

Speaker::~Speaker()
{
    if(m_nvdaLib.isLoaded()) m_nvdaLib.unload();
}

bool Speaker::loadNVDA()
{
    if(!m_nvdaLib.load())
    {
        qWarning() << "Не загрузилась nvdaControllerClient64.dll:" << m_nvdaLib.errorString();
        return false;
    }

    m_speakTextFunc = (NVDAController_speakText_t)m_nvdaLib.resolve("nvdaController_speakText");
    if(!m_speakTextFunc)
    {
        qWarning() << "Не загрузилось NVDAController_speakText function in DLL";
        m_nvdaLib.unload();
        return false;
    }

    qDebug() << " Все окей! NVDA!";
    return true;
}
void Speaker::speakTextNVDA(const QString &text)
{
    if(!m_speakTextFunc)
    {

        ++i;
        qWarning() << "NVDA speak недоступна" << i;
        return;
    }

    // Разбиваем текст на строки (поддержка любых комбинаций перевода строки)
    m_lines = text.split(QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);

    if(m_lines.isEmpty())
    {
        qDebug() << "Пустая строка на озвучке";
        return;
    }

    m_index = 0;
    speakNextLine();
}

void Speaker::stopSpeak()
{
    m_timer.stop();
    m_lines.clear();
    m_index = 0;
}

void Speaker::speakWithFallback(QWidget* widget, const QString &text)
{
    stopSpeak();

    speakTextNVDA(text);
    QTimer* checker = new QTimer(widget); // делаем виджет родителем таймера, чтобы при уничтожение виджета уничтожался и таймер
    checker->setInterval(300); // Делаем таймер, чтобы следить читает ли NVDA или уже закончила

    connect(checker, &QTimer::timeout, widget, [=]()
            {
                if(!widget || !widget->isVisible())
                {
                    checker->stop();
                    checker->deleteLater();
                    return;
                }
                if(!isSpeakNVDA)
                {
                    // QAccessibleTextUpdateEvent ev(widget, 0, "", text);
                    // QAccessible::updateAccessibility(&ev);
                    // QAccessibleEvent focusEv(widget, QAccessible::Focus);
                    // QAccessible::updateAccessibility(&focusEv);

                    checker->stop();
                    checker->deleteLater();
                }
            }); // Подключаем коннект, когда интервал таймера проходит, проверяем состояние читает/не читает NVDA
    checker->start();
    // else
    // {
    //     QAccessibleTextUpdateEvent ev(widget, 0, "", text);
    //     QAccessible::updateAccessibility(&ev);
    //     QAccessibleEvent focusEv(widget, QAccessible::Focus);
    //     QAccessible::updateAccessibility(&focusEv);
    // }
}

void Speaker::speakNextLine()
{
    if ((m_index >= m_lines.size()) /*|| m_lines.size() <= 1*/)
    {
        m_timer.stop();
        isSpeakNVDA = false;
        qDebug() << "Все озвучено!";
        return;
    }
    isSpeakNVDA = true;
    if(m_index == 0)
    {
        ++m_index;
        m_timer.start(2000);
        return;
    }
    const QString& currentLine = m_lines.at(m_index);
    std::wstring wline = currentLine.toStdWString();

    int res = m_speakTextFunc(wline.c_str());

    if(res == 0) qDebug() << "NVDA прочел строку " << currentLine;
    else qWarning() << "NVDA не прочел сроку " << currentLine;

    ++m_index;

    // Запускаем таймер на 500 мс, по истечении которого продолжим озвучку
    int delayTime = 200 + (50 * currentLine.length());
    if(delayTime < 300) delayTime = 300;
    m_timer.start(delayTime);
}

void Speaker::onTimeout()
{
    speakNextLine();
}
