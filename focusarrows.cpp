

/*#include "widget.h"
#include <QPushButton>
#include <QApplication>
#include <QShortcut>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_grid = new QGridLayout;
    for(int r = 0; r < 10; r++)
    {
        for(int c = 0; c < 10; c++)
        {
            m_grid->addWidget(new QPushButton("Row " + QString::number(r)
                                              + ", Col " + QString::number(c)),
                              r, c);
        }
    }
    this->setLayout(m_grid);

    m_grid->itemAtPosition(1, 1)->widget()->setFocus();

    this->setStyleSheet("QPushButton::focus{ background: black; color: white;}");

    // only works for in Qt for Embedded Linux, Symbian and Windows CE only.
    //    QApplication::setNavigationMode(Qt::NavigationModeKeypadDirectional);

    QShortcut * shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::Key_Up),this,
                             SLOT(on_up()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Down),this,
                             SLOT(on_down()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Left),this,
                             SLOT(on_left()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Right),this,
                             SLOT(on_right()));
}

void Widget::on_up()
{
    moveFocus(0, -1);
}

void Widget::on_down()
{
    moveFocus(0, 1);
}

void Widget::on_left()
{
    moveFocus(-1, 0);
}

void Widget::on_right()
{
    moveFocus(1, 0);
}

void Widget::moveFocus(int dx, int dy)
{
    if(qApp->focusWidget() == 0)
        return;
    int idx = m_grid->indexOf(qApp->focusWidget());
    if(idx == -1)
        return;
    int r, c, rowSpan, colSpan;
    m_grid->getItemPosition(idx, &r, &c, &rowSpan, &colSpan);
    QLayoutItem* layoutItem = m_grid->itemAtPosition(r + dy, c + dx);
    if(layoutItem == 0)
        return;
    layoutItem->widget()->setFocus();
}

Widget::~Widget()
{

}*/
