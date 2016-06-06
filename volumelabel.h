#ifndef VolumeLabel_H
#define VolumeLabel_H
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QPoint>

class VolumeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VolumeLabel(QWidget* parent=0);
    ~VolumeLabel();

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
    int posY;
    int cursorY;
    QPoint initPos;
};

#endif // VolumeLabel_H
