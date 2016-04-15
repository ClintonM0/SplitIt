#ifndef DRAGLABEL_H
#define DRAGLABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QPoint>

class DragLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DragLabel(QWidget* parent=0);
    ~DragLabel();

signals:
    void pressed();
    void released();

protected:
    void mousePressEvent(QMouseEvent* event); //QWidget* widget
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int offset;
    int posX;
    int posY;
    int cursorY;
    QPoint initPos;
};

#endif // DRAGLABEL_H
