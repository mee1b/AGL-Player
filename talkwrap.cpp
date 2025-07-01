#include "talkwrap.h"

#ifndef USE_TALK_DLL
//Если не используется talk.dll, то просто делаем заглушки
TalkWrap::TalkWrap():reader_(),valid_(false){}
QString TalkWrap::currentReader() const{ return QString();}
void output(const QString& text, bool interrupt) {};
TalkWrap::~TalkWrap(){}
#else
#include <QDebug>
#include "talk/Tolk.h"

static QString initTalkDll()
{
    Tolk_Load();
    qDebug() << "[Tolk] DLL loaded: "
             << Tolk_IsLoaded()
             << '\n'
             << Tolk_DetectScreenReader()
             << '\n'
             << Tolk_HasSpeech();
    Tolk_PreferSAPI(false);

    const wchar_t *win_name = Tolk_DetectScreenReader();
    if (win_name) {
        QString name = QString::fromStdWString(win_name);
        qDebug() << "Talk: the active screen reader driver is: " << name;
        if (!Tolk_HasSpeech())
        {
            qDebug() << "Talk: error! Driver not support speech output! ";
            return QString();
        }
        return name;
    }

    qDebug() << "Talk: none of the supported screen readers is running";
    return QString();
}

//Если идёт штатное использование
TalkWrap::TalkWrap():
    reader_(initTalkDll()),
    valid_(!reader_.isEmpty())
{
}

QString TalkWrap::currentReader() const
{
    return reader_;
}

void TalkWrap::output(const QString &text, bool interrupt)
{
    if (valid_ && !text.isEmpty())
    {
        Tolk_Output(text.toStdWString().data(),interrupt);
    }
}

TalkWrap::~TalkWrap()
{
    if (valid_) {
        Tolk_Unload();
    }
}
#endif

