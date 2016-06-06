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

    void on_PlayPause_pressed();
    void on_PlayPause_released();
    void on_PlayPause_hovered();
    void on_PlayPause_unhovered();

    void on_Stop_pressed();
    void on_Stop_released();
    void on_Stop_hovered();
    void on_Stop_unhovered();

    void on_Forward_pressed();
    void on_Forward_released();
    void on_Forward_hovered();
    void on_Forward_unhovered();

    void on_Back_pressed();
    void on_Back_released();
    void on_Back_hovered();
    void on_Back_unhovered();

private:
    Ui::SplitIt *ui;
    int playState;
    unsigned int *position;
    unsigned int *length;
};

#endif // SPLITIT_H
