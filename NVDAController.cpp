#include <windows.h>
#include <QString>
#include <QDebug>
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

void Speaker::speakNextLine()
{
    if (m_index >= m_lines.size())
    {
        m_timer.stop();
        qDebug() << "Все озвучено!";
        return;
    }

    const QString& currentLine = m_lines.at(m_index);
    std::wstring wline = currentLine.toStdWString();

    int res = m_speakTextFunc(wline.c_str());

    if(res == 0) qDebug() << "NVDA прочел строку " << currentLine;
    else qWarning() << "NVDA не прочел сроку " << currentLine;

    ++m_index;

    // Запускаем таймер на 500 мс, по истечении которого продолжим озвучку
    int delayTime = 200 + (75 * currentLine.length());
    if(delayTime < 500) delayTime = 500;
    m_timer.start(delayTime);
}

void Speaker::onTimeout()
{
    speakNextLine();
}
