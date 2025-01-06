#ifndef FOCUSARROWS_H
#define FOCUSARROWS_H

#include <QWidget>
#include <QGridLayout>

class focusArrows : public QWidget
{
    Q_OBJECT
public:
    focusArrows(QWidget* parent = 0);
    ~focusArrows();
    QGridLayout* focusArr;
public slots:
    void on_up();
    void on_down();
    void on_right();
    void on_left();
    void moveFocus(int dx, int dy);
};

#endif // FOCUSARROWS_H

//Выбрать и использовать функции из focusarrows.cpp в проекте, для перемещения стрелками по окнам
