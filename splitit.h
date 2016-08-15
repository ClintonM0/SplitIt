#ifndef SPLITIT_H
#define SPLITIT_H

#include <QMainWindow>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QListWidgetItem>
#include <QPixmap>
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

    void closeEvent(QCloseEvent *event);

private slots:

    void fadeButton();

    void on_ListButton_pressed();
    void fadeOutFinished();
    void on_TintOverlay_pressed();
    void getCurrentTrack();

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

    void on_Next_pressed();
    void on_Next_released();
    void on_Next_hovered();
    void on_Next_unhovered();

    void on_Prev_pressed();
    void on_Prev_released();
    void on_Prev_hovered();
    void on_Prev_unhovered();

    void enableCreationUI();
    void disableCreationUI();

    void on_CreationTrackName_enable();
    void on_CreationTrackName_disable();
    void on_CreationTrackTime_enable();
    void on_CreationTrackTime_disable();

    void on_CreationCurrentTime_pressed();
    void on_CreationCurrentTime_released();
    void on_CreationCurrentTime_hovered();
    void on_CreationCurrentTime_unhovered();

    void on_CreationLoad_hovered();
    void on_CreationLoad_pressed();
    void on_CreationLoad_released();
    void on_CreationLoad_unhovered();

    void on_CreationSave_pressed();
    void on_CreationSave_released();
    void on_CreationSave_hovered();
    void on_CreationSave_unhovered();

    void on_CreationAdd_pressed();
    void on_CreationAdd_released();
    void on_CreationAdd_hovered();
    void on_CreationAdd_unhovered();

    void on_CreationRemove_pressed();
    void on_CreationRemove_released();
    void on_CreationRemove_hovered();
    void on_CreationRemove_unhovered();

    void on_CreationTrackList_itemPressed(QListWidgetItem *selectedTrack);

    void on_SeekKnob_pressed();
    void on_SeekKnob_released();
    void on_SeekKnob_hovered();
    void on_SeekKnob_unhovered();

    void on_List_itemPressed(QListWidgetItem *item);

private:
    Ui::SplitIt *ui;

    int playState;
    int currentTrack;

    QString audioFilePath;
    QFileInfo audioFile;

    unsigned int length;
    unsigned int position;

    QStringList config;
    QList<int> trackArray;
    QStringList nameArray;

    float visualiserAlpha;
};

#endif // SPLITIT_H
