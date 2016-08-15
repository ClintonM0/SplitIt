#include <SeekLabel.h>
#include <QCursor>
#include <QWidget>
#include <QPoint>
#include <iostream>
#include <splitit.h>

using namespace std;

SeekLabel::SeekLabel(QWidget* parent) : QLabel(parent)
{
}

SeekLabel::~SeekLabel()
{

}

void SeekLabel::mousePressEvent(QMouseEvent *event) //QWidget *widget
{
    this->setFocus();
    initPos = this->pos();
    offset = event->pos().x();
    setCursor(Qt::ClosedHandCursor);
    emit pressed();
}

void SeekLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit hovered();

    posX = SeekLabel::pos().x();
    cursorX = mapFromGlobal(QCursor::pos()).x();

    if(event->buttons() & Qt::LeftButton)
    {
        if((posX > 173 && posX < 659) || (posX <= 173 && offset - cursorX < 0) || (posX >= 659 && offset - cursorX > 0))
        {
            this->move(mapToParent(QPoint(event->pos().x() - offset, 0)));
        }
        if(posX < 172)
        {
            this->move(173, initPos.y());
        }
        if(posX > 660)
        {
            this->move(659, initPos.y());
        }
    }
}

void SeekLabel::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);

    emit released();
}

void SeekLabel::leaveEvent(QEvent*)
{
    emit unhovered();
}
