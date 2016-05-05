#include <QApplication>
#include <QCoreApplication>
#include <QCursor>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QWidget>
#include <QPropertyAnimation>
#include <iostream>
#include <splitit.h>
#include <ui_splitit.h>

SplitIt::SplitIt(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::SplitIt)
{
    ui->setupUi(this);
    ui->TintOverlay->setVisible(0);

    QFont font = QFont("Agency FB", 20, QFont::Bold);
    QFont fontSmall = QFont("Agency FB", 16, QFont::Bold);
    font.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    font.setStretch(125);
    fontSmall.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    fontSmall.setStretch(120);
    ui->CurrentSong->setFont(font);
    ui->CurrentST->setFont(font);
    ui->CurrentTrackNo->setFont(font);
    ui->CreateButtonText->setFont(fontSmall);
    ui->OpenButtonText->setFont(fontSmall);

}

SplitIt::~SplitIt()
{
    delete ui;
}


void SplitIt::on_Quit_pressed()
{
    qApp->exit();
}

void SplitIt::on_ListButton_pressed()
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

void SplitIt::on_TintOverlay_pressed()
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

void SplitIt::on_OpenButtonText_pressed()
{
    ui->TintOverlay->raise();
    ui->OpenButton->raise();
    ui->OpenButtonText->raise();
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButtonClicked.png"));
    fadeButton();
}

void SplitIt::on_OpenButtonText_released()
{
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButton.png"));
}


void SplitIt::on_OpenButtonText_hovered()
{
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButtonHover.png"));
}

void SplitIt::on_OpenButtonText_unhovered()
{
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButton.png"));
}


void SplitIt::on_CreateButtonText_pressed()
{
    ui->TintOverlay->raise();
    ui->CreateButton->raise();
    ui->CreateButtonText->raise();
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButtonClicked.png"));
    fadeButton();
}

void SplitIt::on_CreateButtonText_released()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton.png"));
}


void SplitIt::on_CreateButtonText_hovered()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButtonHover.png"));
}

void SplitIt::on_CreateButtonText_unhovered()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton.png"));
}


void SplitIt::on_VolumeKnob_pressed()
{
    volumePressed = true;
    ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnobClicked.png"));
}

void SplitIt::on_VolumeKnob_released()
{
    volumePressed = false;
    ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnob.png"));
}

void SplitIt::on_VolumeKnob_hovered()
{
    if(volumePressed == false)
    {
        ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnobHover.png"));
    }
}

void SplitIt::on_VolumeKnob_unhovered()
{
    if(volumePressed == false)
    {
        ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnob.png"));
    }
}
