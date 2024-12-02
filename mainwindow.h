#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>
#include <QPlainTextEdit>
#include "managerwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &manager);
#endif

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QString curFile;
};
#endif // MAINWINDOW_H
/*Общедоступный API ограничен конструктором. В protected разделе мы переопределяем QWidget::closeEvent(), чтобы определять, когда пользователь пытается закрыть окно,
 * и предупреждать пользователя о несохраненных изменениях.
 * В private slots разделе мы объявляем слоты, соответствующие пунктам меню, а также загадочный documentWasModified() слот.
 * Наконец, в private разделе класса у нас есть различные элементы, которые будут объяснены в свое время.*/
