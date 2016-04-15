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
    void on_Quit_clicked();

    void fadeButton();

    void on_ListButton_clicked();
    void fadeOutFinished();
    void on_TintOverlay_clicked();

    void on_CreateButtonText_clicked();

    void on_OpenButtonText_clicked();

private:
    Ui::SplitIt *ui;
    bool fadingOut = false;
};

#endif // SPLITIT_H
