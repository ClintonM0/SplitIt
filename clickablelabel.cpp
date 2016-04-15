#include "clickablelabel.h"
#include <QCursor>

ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::mousePressEvent(QMouseEvent*)
{
    emit clicked();
}
