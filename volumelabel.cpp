#include <VolumeLabel.h>
#include <QCursor>
#include <QWidget>
#include <QPoint>
#include <iostream>
#include <splitit.h>

using namespace std;

VolumeLabel::VolumeLabel(QWidget* parent) : QLabel(parent)
{
}

VolumeLabel::~VolumeLabel()
{

}

void VolumeLabel::mousePressEvent(QMouseEvent *event) //QWidget *widget
{
    this->setFocus();
    initPos = this->pos();
    offset = event->pos().y();
    setCursor(Qt::ClosedHandCursor);
    emit pressed();
}

void VolumeLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit hovered();

    posX = VolumeLabel::pos().x();
    posY = VolumeLabel::pos().y();
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

void VolumeLabel::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);

    emit released();
}

void VolumeLabel::leaveEvent(QEvent*)
{
    emit unhovered();
}
