#include <QApplication>
#include <QCoreApplication>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QWidget>
#include <QPropertyAnimation>
#include <iostream>
#include "splitit.h"
#include "ui_splitit.h"

SplitIt::SplitIt(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::SplitIt)
{
    ui->setupUi(this);
    ui->TintOverlay->setVisible(0);
    QFont font = QFont("Agency FB", 20, QFont::Bold);
    font.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    font.setStretch(125);
    ui->CurrentSong->setFont(font);
    ui->CurrentST->setFont(font);
    ui->CurrentTrackNo->setFont(font);

}

SplitIt::~SplitIt()
{
    delete ui;
}


void SplitIt::on_Quit_clicked()
{
    qApp->exit();
}

void SplitIt::on_ListButton_clicked()
{
    ui->TintOverlay->raise();
    ui->ListButton->raise();
    fadeButton();
}

void SplitIt::fadeButton()
{
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->TintOverlay->setGraphicsEffect(eff);
    QPropertyAnimation* anim = new QPropertyAnimation(this);

    anim->setTargetObject(eff);
    anim->setPropertyName("opacity");

    if(ui->TintOverlay->isVisible() && fadingOut == false)
    {
        fadingOut = true;
        anim->setDuration(400);
        anim->setStartValue(1);\
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::Linear);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        connect(anim, SIGNAL(finished()), this, SLOT(fadeOutFinished()));
    }
    else
    {
        anim->setDuration(250);
        anim->setStartValue(0);\
        anim->setEndValue(1);
        anim->setEasingCurve(QEasingCurve::Linear);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        fadingOut = false;
        ui->TintOverlay->setVisible(1);
    }
}

void SplitIt::on_TintOverlay_clicked()
{
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->TintOverlay->setGraphicsEffect(eff);
    QPropertyAnimation* anim = new QPropertyAnimation(this);

    anim->setTargetObject(eff);
    anim->setPropertyName("opacity");

    if(ui->TintOverlay->isVisible() && fadingOut == false)
    {
        fadingOut = true;
        anim->setDuration(400);
        anim->setStartValue(1);\
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::Linear);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        connect(anim, SIGNAL(finished()), this, SLOT(fadeOutFinished()));
    }
    else
    {
        fadeOutFinished();
    }

}

void SplitIt::fadeOutFinished()
{
    ui->TintOverlay->setVisible(0);

    fadingOut = false;
}

void SplitIt::on_CreateButtonText_clicked()
{
    ui->TintOverlay->raise();
    ui->CreateButton->raise();
    ui->CreateButtonText->raise();
    fadeButton();
}

void SplitIt::on_OpenButtonText_clicked()
{
    ui->TintOverlay->raise();
    ui->OpenButton->raise();
    ui->OpenButtonText->raise();
    fadeButton();
}
