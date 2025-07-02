#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QLibrary>
#include <QRegularExpression>

using NVDAController_speakText_t = long (__stdcall *)(const wchar_t*);

class Speaker : public QObject
{
    Q_OBJECT
public:
    explicit Speaker(QObject *parent = nullptr);
    ~Speaker();

    void speakTextNVDA(const QString& text);
    void stopSpeak();

private slots:
    void onTimeout();

private:
    bool loadNVDA();
    void speakNextLine();

private:
    QStringList m_lines;
    int m_index = 0;
    int i = 0;

    QTimer m_timer;
    QLibrary m_nvdaLib;
    NVDAController_speakText_t m_speakTextFunc = nullptr;
};
