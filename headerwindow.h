#ifndef HEADERWINDOW_H
#define HEADERWINDOW_H

#include <QWidget>

namespace Ui {
class headerwindow;
}

class headerwindow : public QWidget
{
    Q_OBJECT

public:
    explicit headerwindow(QWidget *parent = nullptr);
    ~headerwindow();

private slots:
    void on_toolButton_customContextMenuRequested(const QPoint &pos);

private:
    Ui::headerwindow *ui;
};

#endif // HEADERWINDOW_H
