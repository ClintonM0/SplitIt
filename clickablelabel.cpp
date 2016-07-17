#include <clickablelabel.h>
#include <QCursor>
#include <iostream>

ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent)
{
    setMouseTracking(true);
}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::mousePressEvent(QMouseEvent*)
{
    this->setFocus();
    emit pressed();

}
void ClickableLabel::mouseReleaseEvent(QMouseEvent*)
{
    emit released();
}

void ClickableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(this->rect().contains(event->pos()))
    {
        emit hovered();
    }
}
void ClickableLabel::leaveEvent(QEvent*)
{
    emit unhovered();
}
