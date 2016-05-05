#include "draglabel.h"
#include <QCursor>
#include <QWidget>
#include <QPoint>
#include <iostream>

using namespace std;

DragLabel::DragLabel(QWidget* parent) : QLabel(parent)
{
}

DragLabel::~DragLabel()
{

}

void DragLabel::mousePressEvent(QMouseEvent *event) //QWidget *widget
{
    initPos = this->pos();
    offset = event->pos().y();
    setCursor(Qt::ClosedHandCursor);
    emit pressed();
}

void DragLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit hovered();


    posX = DragLabel::pos().x();
    posY = DragLabel::pos().y();
    cursorY = mapFromGlobal(QCursor::pos()).y();

    if(event->buttons() & Qt::LeftButton)
    {
        if((posY > 70 && posY < 254) || (posY <= 70 && offset - cursorY < 0) || (posY >= 254 && offset - cursorY > 0))
        {
            this->move(mapToParent(QPoint(0, event->pos().y() - offset)));
        }
        if(posY < 69)
        {
            this->move(initPos.x(), 70);
        }
        if(posY > 255)
        {
            this->move(initPos.x(), 254);
        }
    }
}

void DragLabel::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);

    emit released();
}

void DragLabel::leaveEvent(QEvent*)
{
    emit unhovered();
}