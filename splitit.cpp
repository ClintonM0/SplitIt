#include <Windows.h>
#include <lmcons.h>

#include <fmod.hpp>
#include <math.h>
#include <QApplication>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QCursor>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QMessageBox>
#include <QTextStream>
#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <iostream>
#include <splitit.h>
#include <ui_splitit.h>

FMOD::System *soundSystem;
FMOD::Sound *sound;
FMOD::Channel *channel = 0;
FMOD::ChannelGroup *master;
FMOD::DSP *loudnessDSP;
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
bool creationCurrentTimeButton = false;
bool creationLoadButton = false;
bool creationSaveButton = false;

SplitIt::SplitIt(QWidget *parent) :
    QMainWindow(parent, Qt::Window),
    ui(new Ui::SplitIt)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    ui->TintOverlay->setVisible(0);

    ui->CreateButtonText->setDisabled(1);

    QFont font = QFont("Agency FB", 20, QFont::Bold);
    QFont fontUnderline = QFont("Agency FB", 20, QFont::Bold);
    QFont fontSmall = QFont("Agency FB", 16, QFont::Bold);
    QFont fontInput = QFont("Agency FB", 16, QFont::Bold);
    font.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    font.setStretch(125);
    fontInput.setLetterSpacing(QFont::AbsoluteSpacing,0.75);
    fontInput.setStretch(120);
    fontUnderline.setLetterSpacing(QFont::AbsoluteSpacing,2);
    fontUnderline.setStretch(120);
    fontUnderline.setUnderline(1);
    fontSmall.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    fontSmall.setStretch(120);
    ui->CurrentSong->setFont(font);
    ui->SongName->setFont(font);
    ui->CurrentST->setFont(font);    
    ui->STName->setFont(font);
    ui->CreateButtonText->setFont(fontSmall);
    ui->OpenButtonText->setFont(fontSmall);

    ui->PlayPause->setDisabled(1);
    ui->Stop->setDisabled(1);
    ui->Forward->setDisabled(1);
    ui->Back->setDisabled(1);
    ui->Next->setDisabled(1);
    ui->Prev->setDisabled(1);

    ui->CreationBG->setVisible(0);
    ui->CreationCurrentTime->setVisible(0);
    ui->CreationLoad->setVisible(0);
    ui->CreationNewTrack->setVisible(0);
    ui->CreationSave->setVisible(0);
    ui->CreationTrackListTitle->setVisible(0);
    ui->CreationTrackName->setVisible(0);
    ui->CreationTrackTime->setVisible(0);

    ui->CreationNewTrack->setFont(fontUnderline);
    ui->CreationTrackName->setFont(fontInput);
    ui->CreationTrackName->setStyleSheet("QLineEdit { background: rgb(27, 0, 49); }");
    ui->CreationTrackName->setFrame(0);

    ui->CreationTrackTime->setFont(fontInput);
    ui->CreationTrackTime->setStyleSheet("QLineEdit { background: rgb(27, 0, 49); }");
    ui->CreationTrackTime->setFrame(0);

    ui->CreationTrackListTitle->setFont(fontSmall);

    ui->Background->setFocus();

    result = FMOD::System_Create(&soundSystem);

    playState = 0; // 0 = stopped, 1 = paused, 2 = playing
    soundSystem->init(4, FMOD_INIT_NORMAL, 0);
    soundSystem->getMasterChannelGroup(&master);
    master->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &loudnessDSP);
    loudnessDSP->setMeteringEnabled(false, true);

    QGraphicsOpacityEffect *visualiser = new QGraphicsOpacityEffect(this);
    ui->VisualiserB->setGraphicsEffect(visualiser);
    visualiser->setOpacity(0);
}

void SplitIt::closeEvent(QCloseEvent *event)
{
    playState = 0;
    sound->release();
    loudnessDSP->release();
    soundSystem->release();
}

SplitIt::~SplitIt()
{
    delete ui;
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
        anim->setStartValue(0);
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
        anim->setStartValue(1);
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

void SplitIt::getCurrentTrack()
{
    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    currentTrack = 0;
    int i = 0;

    while(trackArray.count() != (i + 1) && trackArray.at(i) * 1000 <= int(position))
    {
        currentTrack = i;
        i = i + 1;
    }
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
    QString qFilePath = NULL;
    openButton = false;
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButton.png"));
    qFilePath = QFileDialog::getOpenFileName(this, tr("Select Audio File"),
                                                     "C://",
                                                     tr("Audio Files (*.mp3 *.wav)"));
    if(qFilePath != NULL)
    {
        const char * filePath = qFilePath.toStdString().c_str();
        if(playState != 0)
        {
            playState = 0;
            channel->setPaused(1);
        }
        sound->release();
        soundSystem->createSound(filePath, FMOD_CREATESTREAM, 0, &sound);
        soundSystem->playSound(sound, 0, true, &channel);
        sound->getLength(&length, FMOD_TIMEUNIT_MS);
        QFileInfo audioFile(qFilePath);
        ui->SongName->setText(audioFile.fileName().left(audioFile.fileName().length() - 4));
        ui->PlayPause->setDisabled(0);
        ui->Stop->setDisabled(0);
        ui->Forward->setDisabled(0);
        ui->Back->setDisabled(0);
        ui->Next->setDisabled(0);
        ui->Prev->setDisabled(0);
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
    }
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
    QString qFilePath = NULL;
    createButton = false;
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton.png"));
    qFilePath = QFileDialog::getOpenFileName(this, tr("Select Audio File"),
                                                     "C://",
                                                     tr("SplitIt Config (*.txt)"));
    if(qFilePath != NULL)
    {
        QFile configFile(qFilePath);
        if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream configStream(&configFile);
        int lineNumber = 1;
        while(!configStream.atEnd())
        {
            QString line = configStream.readLine();
            config.append(line);
            if(line.left(1) == QString("A") && lineNumber > 2)
            {
                trackArray.append(line.right(line.length() - 2).toInt());
            }
            if(line.left(1) == QString("B") && lineNumber > 2)
            {
                nameArray.append(line.right(line.length() - 2));
            }
            lineNumber = lineNumber + 1;
        }
        ui->STName->setText(config.at(1));
        getCurrentTrack();
        ui->SongName->setText(nameArray.at(currentTrack));
    }

    fadeButton();
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
    playPauseButton = true;

}
void SplitIt::on_PlayPause_released()
{
    QGraphicsOpacityEffect *visualiser = new QGraphicsOpacityEffect(this);
    ui->VisualiserB->setVisible(1);
    ui->VisualiserB->setGraphicsEffect(visualiser);
    playPauseButton = false;

    if(playState == 2)
    {
        visualiser->setOpacity(0);
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOff"));
        playState = 1;
        channel->setPaused(true);
    }
    else
    {
        playState = 2;
        channel->setPaused(false);

        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonOn"));

        while(playState == 2)
        {
            FMOD_DSP_METERING_INFO meter = {};
            soundSystem->update();
            loudnessDSP->getMeteringInfo(0, &meter);
            visualiserAlpha = meter.rmslevel[0];

            visualiser->setOpacity(visualiserAlpha);
            QApplication::processEvents();
        }
    }
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
    stopButton = true;
}
void SplitIt::on_Stop_released()
{
    ui->Stop->setPixmap(QPixmap(":/images/StopButton"));
    ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
    channel->setPaused(true);
    channel->setPosition(0, FMOD_TIMEUNIT_MS);
    ui->VisualiserB->setVisible(0);
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
    forwardButton = true;
}
void SplitIt::on_Forward_released()
{
    ui->Forward->setPixmap(QPixmap(":/images/TimeSkipFwdButton"));\
    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    if(int(position) + 10000 < int(length))
    {
        channel->setPosition(position + 10000, FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);
    }
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
    backButton = true;
}
void SplitIt::on_Back_released()
{
    ui->Back->setPixmap(QPixmap(":/images/TimeSkipBakButton"));

    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    if(int(position) - 10000 > 0)
    {
        channel->setPosition(position - 10000, FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);
    }
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


void SplitIt::on_Next_pressed()
{
    ui->Next->setPixmap(QPixmap(":/images/TrackFwdButtonPressed"));
    nextButton = true;
}
void SplitIt::on_Next_released()
{
    ui->Next->setPixmap(QPixmap(":/images/TrackFwdButton"));

    getCurrentTrack();
    if(currentTrack + 1 != trackArray.length())
    {
        channel->setPosition((unsigned int)(trackArray.at(currentTrack + 1) * 1000), FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);

        ui->VisualiserB->setVisible(0);
    }
    nextButton = false;
}
void SplitIt::on_Next_hovered()
{
    if(nextButton == false)
    {
        ui->Next->setPixmap(QPixmap(":/images/TrackFwdButtonHovered"));
    }
}
void SplitIt::on_Next_unhovered()
{
    ui->Next->setPixmap(QPixmap(":/images/TrackFwdButton"));
}


void SplitIt::on_Prev_pressed()
{
    ui->Prev->setPixmap(QPixmap(":/images/TrackBakButtonPressed"));
    getCurrentTrack();
    if(currentTrack != 0)
    {
        channel->setPosition((unsigned int)(trackArray.at(currentTrack - 1) * 1000), FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);

        ui->VisualiserB->setVisible(0);
    }
    prevButton = true;
}
void SplitIt::on_Prev_released()
{
    ui->Prev->setPixmap(QPixmap(":/images/TrackBakButton"));
    prevButton = false;
}
void SplitIt::on_Prev_hovered()
{
    if(prevButton == false)
    {
        ui->Prev->setPixmap(QPixmap(":/images/TrackBakButtonHovered"));
    }
}
void SplitIt::on_Prev_unhovered()
{
    ui->Prev->setPixmap(QPixmap(":/images/TrackBakButton"));
}


void SplitIt::on_CreationTrackName_enable()
{
    ui->CreationTrackName->setFocus();
}
void SplitIt::on_CreationTrackName_disable()
{
    ui->CreationTrackName->clearFocus();
}


void SplitIt::on_CreationTrackTime_enable()
{
    ui->CreationTrackTime->setFocus();
}
void SplitIt::on_CreationTrackTime_disable()
{
    ui->CreationTrackTime->clearFocus();
}


void SplitIt::on_CreationCurrentTime_pressed()
{
    ui->CreationCurrentTime->setPixmap(QPixmap(":/images/CreationUI/SetCurrentTimePressed"));
    creationCurrentTimeButton = true;
}
void SplitIt::on_CreationCurrentTime_released()
{
    ui->CreationCurrentTime->setPixmap(QPixmap(":/images/CreationUI/SetCurrentTime"));
    creationCurrentTimeButton = false;
}
void SplitIt::on_CreationCurrentTime_hovered()
{
    if(creationCurrentTimeButton == false)
    {
        ui->CreationCurrentTime->setPixmap(QPixmap(":/images/CreationUI/SetCurrentTimeHovered"));
    }
}
void SplitIt::on_CreationCurrentTime_unhovered()
{
    ui->CreationCurrentTime->setPixmap(QPixmap(":/images/CreationUI/SetCurrentTime"));
}


void SplitIt::on_CreationLoad_pressed()
{
    ui->CreationLoad->setPixmap(QPixmap(":/images/CreationUI/LoadPressed"));
    creationLoadButton = true;
}
void SplitIt::on_CreationLoad_released()
{
    ui->CreationLoad->setPixmap(QPixmap(":/images/CreationUI/Load"));
    creationLoadButton = false;
}
void SplitIt::on_CreationLoad_hovered()
{
    if(creationLoadButton == false)
    {
        ui->CreationLoad->setPixmap(QPixmap(":/images/CreationUI/LoadHovered"));
    }
}
void SplitIt::on_CreationLoad_unhovered()
{
    ui->CreationLoad->setPixmap(QPixmap(":/images/CreationUI/Load"));
}


void SplitIt::on_CreationSave_pressed()
{
    ui->CreationSave->setPixmap(QPixmap(":/images/CreationUI/SavePressed"));
    creationSaveButton = true;
}
void SplitIt::on_CreationSave_released()
{
    ui->CreationSave->setPixmap(QPixmap(":/images/CreationUI/Save"));
    creationSaveButton = false;
}
void SplitIt::on_CreationSave_hovered()
{
    if(creationSaveButton == false)
    {
        ui->CreationSave->setPixmap(QPixmap(":/images/CreationUI/SaveHovered"));
    }
}
void SplitIt::on_CreationSave_unhovered()
{
    ui->CreationSave->setPixmap(QPixmap(":/images/CreationUI/Save"));
}
