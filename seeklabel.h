#ifndef SeekLabel_H
#define SeekLabel_H
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QPoint>

class SeekLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SeekLabel(QWidget* parent=0);
    ~SeekLabel();

signals:
    void pressed();
    void released();
    void hovered();
    void unhovered();

protected:
    void mousePressEvent(QMouseEvent* event); //QWidget* widget
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent*);

private:
    int offset;
    int posX;
    int cursorX;
    QPoint initPos;
};

#endif // SeekLabel_H
