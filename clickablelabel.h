#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent=0);
    ~ClickableLabel();

signals:
    void pressed();
    void released();
    void hovered();
    void unhovered();

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent*);
};

#endif // CLICKABLELABEL_H
