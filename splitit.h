#ifndef SPLITIT_H
#define SPLITIT_H

#include <QMainWindow>
#include <QPoint>

namespace Ui {
class SplitIt;
}

class SplitIt : public QMainWindow
{
    Q_OBJECT

public:
    explicit SplitIt(QWidget *parent = 0);
    ~SplitIt();

private slots:
    void on_Quit_pressed();

    void fadeButton();

    void on_ListButton_pressed();
    void fadeOutFinished();
    void on_TintOverlay_pressed();

    void on_OpenButtonText_pressed();
    void on_OpenButtonText_hovered();
    void on_OpenButtonText_unhovered();
    void on_OpenButtonText_released();

    void on_CreateButtonText_pressed();
    void on_CreateButtonText_hovered();
    void on_CreateButtonText_unhovered();
    void on_CreateButtonText_released();

    void on_VolumeKnob_pressed();
    void on_VolumeKnob_hovered();
    void on_VolumeKnob_unhovered();
    void on_VolumeKnob_released();

private:
    Ui::SplitIt *ui;
    bool fadingOut = false;
    bool volumePressed = false;
};

#endif // SPLITIT_H
