#include <Windows.h>
#include <lmcons.h>

#include <fmod.hpp>
#include <math.h>
#include <QApplication>
#include <QCoreApplication>
#include <QCursor>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QWidget>
#include <QPropertyAnimation>
#include <iostream>
#include <splitit.h>
#include <ui_splitit.h>

FMOD::System *soundSystem;
FMOD::Sound *sound;
FMOD::Channel *channel = 0;
FMOD_RESULT result;

float volume = 0;

bool fadingOut = false;
bool openButton = false;
bool createButton = false;
bool volumeButton = false;
bool playPauseButton = false;
bool stopButton = false;
bool forwardButton = false;
bool backButton = false;
bool nextButton = false;
bool prevButton = false;

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
    ui->SongName->setFont(font);
    ui->CurrentST->setFont(font);
    ui->CreateButtonText->setFont(fontSmall);
    ui->OpenButtonText->setFont(fontSmall);

    result = FMOD::System_Create(&soundSystem);

    playState = 0; // 0 = stopped, 1 = paused, 2 = playing
    soundSystem->init(4, FMOD_INIT_NORMAL, 0);
}

SplitIt::~SplitIt()
{
    delete ui;
}

void SplitIt::on_Quit_pressed()
{
    sound->release();
    soundSystem->release();
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
    openButton = true;
    ui->TintOverlay->raise();
    ui->OpenButton->raise();
    ui->OpenButtonText->raise();
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButtonClicked.png"));
    fadeButton();
}
void SplitIt::on_OpenButtonText_released()
{
    openButton = false;
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButton.png"));
    QString audioFile = QFileDialog::getOpenFileName(this, tr("Select Audio File"),
                                                     "C://",
                                                     tr("Audio Files (*.mp3 *.wav)"));
    const char * filePath = audioFile.toStdString().c_str();
    soundSystem->createSound(filePath, FMOD_ACCURATETIME, 0, &sound);
    soundSystem->playSound(sound, 0, true, &channel);
    sound->getLength(length, FMOD_TIMEUNIT_MS);
    fadeButton();
}
void SplitIt::on_OpenButtonText_hovered()
{
    if(openButton == false)
    {
        ui->OpenButton->setPixmap(QPixmap(":/images/GenericButtonHover.png"));
    }
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
    createButton = true;
}
void SplitIt::on_CreateButtonText_released()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton.png"));
    createButton = false;
}
void SplitIt::on_CreateButtonText_hovered()
{
    if(createButton == false)
    {
        ui->CreateButton->setPixmap(QPixmap(":/images/GenericButtonHover.png"));
    }
}

void SplitIt::on_CreateButtonText_unhovered()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton.png"));
}


void SplitIt::on_VolumeKnob_pressed()
{
    volumeButton = true;
    ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnobClicked.png"));
}
void SplitIt::on_VolumeKnob_released()
{
    volumeButton = false;
    ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnob.png"));

    if(ui->VolumeKnob->pos().y() <= 254 && ui->VolumeKnob->pos().y() >= 70)
    {
        volume = float(254 - ui->VolumeKnob->pos().y())/float(184);
        std::cout << volume;
        std::cout << "\n";
        std::cout << ui->VolumeKnob->pos().y();
        std::cout << "\n";
        channel->setVolume(volume);
    }
}
void SplitIt::on_VolumeKnob_hovered()
{
    if(volumeButton == false)
    {
        ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnobHover.png"));
    }
}
void SplitIt::on_VolumeKnob_unhovered()
{
    if(volumeButton == false)
    {
        ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnob.png"));
    }
}


void SplitIt::on_PlayPause_pressed()
{
    ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonPressed"));
    if(playState <= 1)
    {
        playState = 2;
        channel->setPaused(false);
    }
    else
    {
        playState = 1;
        channel->setPaused(true);
    }
    playPauseButton = true;
}
void SplitIt::on_PlayPause_released()
{
    if(playState == 1)
    {
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOff"));
    }
    else
    {
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOn"));
    }
    playPauseButton = false;
}
void SplitIt::on_PlayPause_hovered()
{
    if(playPauseButton == false)
    {
        if(playState == 0)
        {
            ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonHovered"));
        }
        else
        {
            if(playState == 1)
            {
                ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOffHovered"));
            }
            else
            {
                ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOnHovered"));
            }
        }
    }
}
void SplitIt::on_PlayPause_unhovered()
{
    if(playState == 0)
    {
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
    }
    else
    {
        if(playState == 1)
        {
            ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOff"));
        }
        else
        {
            ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOn"));
        }
    }
}


void SplitIt::on_Stop_pressed()
{
    playState = 0;
    ui->Stop->setPixmap(QPixmap(":/images/StopButtonPressed"));
    ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
    channel->setPaused(true);
    channel->setPosition(0, FMOD_TIMEUNIT_MS);
    stopButton = true;
}
void SplitIt::on_Stop_released()
{
    ui->Stop->setPixmap(QPixmap(":/images/StopButton"));
    stopButton = false;
}
void SplitIt::on_Stop_hovered()
{
    if(stopButton == false)
    {
        ui->Stop->setPixmap(QPixmap(":/images/StopButtonHovered"));
    }
}
void SplitIt::on_Stop_unhovered()
{
    ui->Stop->setPixmap(QPixmap(":/images/StopButton"));
}


void SplitIt::on_Forward_pressed()
{
    ui->Forward->setPixmap(QPixmap(":/images/TimeSkipFwdButtonPressed"));
    channel->getPosition(position, FMOD_TIMEUNIT_MS);
    if(int(*position) + 10000 < int(*length))
    {
        channel->setPosition(*position + 10000, FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);
    }
    forwardButton = true;
}
void SplitIt::on_Forward_released()
{
    ui->Forward->setPixmap(QPixmap(":/images/TimeSkipFwdButton"));\
    forwardButton = false;
}
void SplitIt::on_Forward_hovered()
{
    if(forwardButton == false)
    {
        ui->Forward->setPixmap(QPixmap(":/images/TimeSkipFwdButtonHovered"));
    }
}
void SplitIt::on_Forward_unhovered()
{
    ui->Forward->setPixmap(QPixmap(":/images/TimeSkipFwdButton"));
}


void SplitIt::on_Back_pressed()
{
    ui->Back->setPixmap(QPixmap(":/images/TimeSkipBakButtonPressed"));
    channel->getPosition(position, FMOD_TIMEUNIT_MS);
    if(int(*position) - 10000 > 0)
    {
        channel->setPosition(*position - 10000, FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);
    }
    backButton = true;
}
void SplitIt::on_Back_released()
{
    ui->Back->setPixmap(QPixmap(":/images/TimeSkipBakButton"));
    backButton = false;
}
void SplitIt::on_Back_hovered()
{
    if(backButton == false)
    {
        ui->Back->setPixmap(QPixmap(":/images/TimeSkipBakButtonHovered"));
    }
}
void SplitIt::on_Back_unhovered()
{
    ui->Back->setPixmap(QPixmap(":/images/TimeSkipBakButton"));
}
