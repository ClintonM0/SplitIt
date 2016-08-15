#include <Windows.h>
#include <lmcons.h>
#include <math.h>
#include <iostream>

#include <fmod.hpp>

// Qt include files
#include <QApplication>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QCursor>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QMessageBox>
#include <QTextStream>
#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>

#include <splitit.h>
#include <ui_splitit.h>

using namespace std;

// FMOD Sound System declaration
FMOD::System *soundSystem;
FMOD::Sound *sound;
FMOD::Channel *channel = 0;
FMOD::ChannelGroup *master;
FMOD::DSP *loudnessDSP;
FMOD_RESULT result;

// Music volume declaration
float volume = 0;

// Button press statuses for each button
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
bool seekButton = false;
bool creationCurrentTimeButton = false;
bool creationLoadButton = false;
bool creationSaveButton = false;

bool creationUIOn = false;

// Qt Initialisation
SplitIt::SplitIt(QWidget *parent) :
    QMainWindow(parent, Qt::Window),
    ui(new Ui::SplitIt)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    ui->TintOverlay->setVisible(0);

    // Font settings for UI text
    QFont font = QFont("Agency FB", 20, QFont::Bold);
    QFont fontSmall = QFont("Agency FB", 16, QFont::Bold);
    QFont fontInput = QFont("Agency FB", 16, QFont::Bold);

    font.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    font.setStretch(125);
    fontInput.setLetterSpacing(QFont::AbsoluteSpacing,0.75);
    fontInput.setStretch(120);
    fontSmall.setLetterSpacing(QFont::AbsoluteSpacing,2.5);
    fontSmall.setStretch(120);

    ui->CurrentSong->setFont(font);
    ui->SongName->setFont(font);
    ui->CurrentST->setFont(font);
    ui->STName->setFont(font);
    ui->CreateButtonText->setFont(fontSmall);
    ui->OpenButtonText->setFont(fontSmall);

    // Disable music controls upon launch. Enabled later when music file is loaded.
    ui->PlayPause->setDisabled(1);
    ui->Stop->setDisabled(1);
    ui->Forward->setDisabled(1);
    ui->Back->setDisabled(1);
    ui->Next->setDisabled(1);
    ui->Prev->setDisabled(1);
    ui->VolumeKnob->setDisabled(1);
    ui->SeekKnob->setDisabled(1);

    // Hide Creation UI upon launch.
    ui->CreationBG->setVisible(0);
    ui->CreationAdd->setVisible(0);
    ui->CreationCurrentTime->setVisible(0);
    ui->CreationLoad->setVisible(0);
    ui->CreationNewTrack->setVisible(0);
    ui->CreationRemove->setVisible(0);
    ui->CreationSave->setVisible(0);
    ui->CreationTrackList->setVisible(0);
    ui->CreationTrackListTitle->setVisible(0);
    ui->CreationTrackName->setVisible(0);
    ui->CreationTrackTime->setVisible(0);

    // Hide List UI upon launch.
    ui->ListBG->setVisible(0);
    ui->ListTitle->setVisible(0);
    ui->List->setVisible(0);

    // Creation UI font setup
    QFont fontCreationTrackList = QFont("Agency FB", 12, QFont::Bold);
    QFont fontUnderline = QFont("Agency FB", 20, QFont::Bold);

    fontCreationTrackList.setLetterSpacing(QFont::AbsoluteSpacing,2);
    fontUnderline.setLetterSpacing(QFont::AbsoluteSpacing,2);
    fontUnderline.setStretch(120);
    fontUnderline.setUnderline(1);

    ui->CreationTrackList->setFont(fontCreationTrackList);
    ui->CreationNewTrack->setFont(fontUnderline);
    ui->CreationTrackName->setFont(fontInput);
    ui->CreationTrackName->setStyleSheet("QLineEdit { background: rgb(27, 0, 49); }");
    ui->CreationTrackTime->setFont(fontInput);
    ui->CreationTrackTime->setStyleSheet("QLineEdit { background: rgb(27, 0, 49); }");
    ui->CreationTrackListTitle->setFont(fontSmall);

    // List UI font setup
    QFont fontNarrow = QFont("Agency FB", 20);
    fontNarrow.setLetterSpacing(QFont::AbsoluteSpacing,2);
    fontNarrow.setStretch(120);

    ui->ListTitle->setFont(font);
    ui->List->setFont(fontNarrow);

    ui->Background->setFocus();

    // Initialise FMOD Sound System
    result = FMOD::System_Create(&soundSystem);

    playState = 0; // 0 = stopped, 1 = paused, 2 = playing
    soundSystem->init(4, FMOD_INIT_NORMAL, 0);
    soundSystem->getMasterChannelGroup(&master);
    // DSP = Digital Sound Processor
    master->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &loudnessDSP);
    loudnessDSP->setMeteringEnabled(false, true);

    // Initialise Music Visualiser with 100% transparency
    QGraphicsOpacityEffect *visualiser = new QGraphicsOpacityEffect(this);
    ui->VisualiserB->setGraphicsEffect(visualiser);
    visualiser->setOpacity(0);
}

// Release FMOD sound system upon application exit
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
    ui->ListBG->raise();
    ui->ListTitle->raise();
    ui->List->raise();

    fadeButton();

    if(ui->List->isVisible())
    {
        ui->ListBG->setVisible(0);
        ui->ListTitle->setVisible(0);
        ui->List->setVisible(0);
    }
    else
    {
        ui->ListBG->setVisible(1);
        ui->ListTitle->setVisible(1);
        ui->List->setVisible(1);
    }
    ui->ListButton->raise();
}

// Tint overlay fade function
void SplitIt::fadeButton()
{
    // Qt fade effect
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->TintOverlay->setGraphicsEffect(eff);
    QPropertyAnimation* anim = new QPropertyAnimation(this);

    anim->setTargetObject(eff);
    anim->setPropertyName("opacity");

    // Fade in or out
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
        if(creationUIOn == true)
        {
            creationUIOn = false;
            disableCreationUI();
        }
        if(ui->List->isVisible())
        {
            ui->ListBG->setVisible(0);
            ui->ListTitle->setVisible(0);
            ui->List->setVisible(0);
        }

        anim->setDuration(400);
        anim->setStartValue(1);
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::Linear);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        connect(anim, SIGNAL(finished()), this, SLOT(fadeOutFinished()));
    }
    // Handle for when tint overlay is clicked while fading out
    else
    {
        fadeOutFinished();
    }
}

// End tint overlay function
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

    // Linear search for track according to current position
    while(trackArray.count() != (i) && trackArray.at(i) * 1000 <= int(position))
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
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButtonClicked"));
    fadeButton();
}
void SplitIt::on_OpenButtonText_released()
{
    // Open Windows Explorer with Qt to select file.
    openButton = false;
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButton"));
    audioFilePath = QFileDialog::getOpenFileName(this, tr("Select Audio File"),
                                                     "C://",
                                                     tr("Audio Files (*.mp3 *.wav)"));
    if(audioFilePath != NULL)
    {
        const char * filePath = audioFilePath.toStdString().c_str();
        if(playState != 0)
        {
            playState = 0;
            channel->setPaused(1);
        }
        sound->release();
        soundSystem->createSound(filePath, FMOD_CREATESTREAM, 0, &sound);
        soundSystem->playSound(sound, 0, true, &channel);
        sound->getLength(&length, FMOD_TIMEUNIT_MS);
        audioFile.setFile(audioFilePath);
        ui->SongName->setText(audioFile.fileName().left(audioFile.fileName().length() - 4));
        // Enable music controls
        ui->PlayPause->setDisabled(0);
        ui->Stop->setDisabled(0);
        ui->Forward->setDisabled(0);
        ui->Back->setDisabled(0);
        ui->Next->setDisabled(0);
        ui->Prev->setDisabled(0);
        ui->VolumeKnob->setDisabled(0);
        ui->SeekKnob->setDisabled(0);
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
    }
    fadeButton();
}
void SplitIt::on_OpenButtonText_hovered()
{
    if(openButton == false)
    {
        ui->OpenButton->setPixmap(QPixmap(":/images/GenericButtonHover"));
    }
}
void SplitIt::on_OpenButtonText_unhovered()
{
    ui->OpenButton->setPixmap(QPixmap(":/images/GenericButton"));
}


void SplitIt::on_CreateButtonText_pressed()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButtonClicked"));
    createButton = true;
}
void SplitIt::on_CreateButtonText_released()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton"));

    ui->TintOverlay->raise();
    ui->CreateButton->raise();
    ui->CreateButtonText->raise();
    ui->CreationBG->raise();
    ui->CreationAdd->raise();
    ui->CreationCurrentTime->raise();
    ui->CreationLoad->raise();
    ui->CreationNewTrack->raise();
    ui->CreationRemove->raise();
    ui->CreationSave->raise();
    ui->CreationTrackList->raise();
    ui->CreationTrackListTitle->raise();
    ui->CreationTrackName->raise();
    ui->CreationTrackTime->raise();

    fadeButton();

    if(creationUIOn == false)
    {
        creationUIOn = true;
        enableCreationUI();
    }
    else
    {
        creationUIOn = false;
        disableCreationUI();
    }

    createButton = false;
}
void SplitIt::on_CreateButtonText_hovered()
{
    if(createButton == false)
    {
        ui->CreateButton->setPixmap(QPixmap(":/images/GenericButtonHover"));
    }
}
void SplitIt::on_CreateButtonText_unhovered()
{
    ui->CreateButton->setPixmap(QPixmap(":/images/GenericButton"));
}


void SplitIt::on_VolumeKnob_pressed()
{
    volumeButton = true;
    ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnobClicked"));
}
void SplitIt::on_VolumeKnob_released()
{
    volumeButton = false;
    ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnob"));

    if(ui->VolumeKnob->pos().y() <= 254 && ui->VolumeKnob->pos().y() >= 70)
    {
        volume = float(254 - ui->VolumeKnob->pos().y())/float(184);
        channel->setVolume(volume);
    }
}
void SplitIt::on_VolumeKnob_hovered()
{
    if(volumeButton == false)
    {
        ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnobHover"));
    }
}
void SplitIt::on_VolumeKnob_unhovered()
{
    if(volumeButton == false)
    {
        ui->VolumeKnob->setPixmap(QPixmap(":/images/VolumeKnob"));
    }
}


void SplitIt::on_PlayPause_pressed()
{
    ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButtonPressed"));
    playPauseButton = true;

}
void SplitIt::on_PlayPause_released()
{
    // Activate Visualiser
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
        // Update visualiser for each tick while music is played
        while(playState == 2)
        {
            FMOD_DSP_METERING_INFO meter = {};
            soundSystem->update();
            loudnessDSP->getMeteringInfo(0, &meter);
            visualiserAlpha = meter.rmslevel[0];

            visualiser->setOpacity(visualiserAlpha);

            if(trackArray.count() != 0)
            {
                getCurrentTrack();
                ui->SongName->setText(nameArray.at(currentTrack));
            }
            if(seekButton == false)
            {
                channel->getPosition(&position, FMOD_TIMEUNIT_MS);
                float knobPosition = float(position)/float(length);
                ui->SeekKnob->move(knobPosition * 484 + 174, ui->SeekKnob->pos().y());
            }
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
    stopButton = false;
    ui->Stop->setPixmap(QPixmap(":/images/StopButton"));
    ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
    channel->setPaused(true);
    channel->setPosition(0, FMOD_TIMEUNIT_MS);
    ui->VisualiserB->setVisible(0);

    if(trackArray.count() != 0)
    {
        getCurrentTrack();
        ui->SongName->setText(nameArray.at(currentTrack));
    }

    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    float knobPosition = float(position)/float(length);
    ui->SeekKnob->move(knobPosition * 484 + 174, ui->SeekKnob->pos().y());

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
    forwardButton = false;
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
    if(trackArray.length() != 0)
    {
        getCurrentTrack();
        ui->SongName->setText(nameArray.at(currentTrack));
    }

    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    float knobPosition = float(position)/float(length);
    ui->SeekKnob->move(knobPosition * 484 + 174, ui->SeekKnob->pos().y());
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
    backButton = false;
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
    if(trackArray.length() != 0)
    {
        getCurrentTrack();
        ui->SongName->setText(nameArray.at(currentTrack));
    }

    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    float knobPosition = float(position)/float(length);
    ui->SeekKnob->move(knobPosition * 484 + 174, ui->SeekKnob->pos().y());
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
    nextButton = false;
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
    if(trackArray.length() != 0)
    {
        getCurrentTrack();
        ui->SongName->setText(nameArray.at(currentTrack));
    }
    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    float knobPosition = float(position)/float(length);
    ui->SeekKnob->move(knobPosition * 484 + 174, ui->SeekKnob->pos().y());
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
    prevButton = true;
    ui->Prev->setPixmap(QPixmap(":/images/TrackBakButtonPressed"));
}
void SplitIt::on_Prev_released()
{
    ui->Prev->setPixmap(QPixmap(":/images/TrackBakButton"));
    prevButton = false;

    getCurrentTrack();
    if(currentTrack != 0)
    {
        channel->setPosition((unsigned int)(trackArray.at(currentTrack) * 1000), FMOD_TIMEUNIT_MS);
    }
    else
    {
        playState = 0;
        ui->PlayPause->setPixmap(QPixmap(":/images/PlayPauseButton"));
        channel->setPaused(true);
        channel->setPosition(0, FMOD_TIMEUNIT_MS);

        ui->VisualiserB->setVisible(0);
    }
    if(trackArray.length() != 0)
    {
        getCurrentTrack();
        ui->SongName->setText(nameArray.at(currentTrack));
    }
    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    float knobPosition = float(position)/float(length);
    ui->SeekKnob->move(knobPosition * 484 + 174, ui->SeekKnob->pos().y());
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


void SplitIt::enableCreationUI()
{
    ui->CreationBG->setVisible(1);
    ui->CreationAdd->setVisible(1);
    ui->CreationCurrentTime->setVisible(1);
    ui->CreationLoad->setVisible(1);
    ui->CreationNewTrack->setVisible(1);
    ui->CreationRemove->setVisible(1);
    ui->CreationSave->setVisible(1);
    ui->CreationTrackList->setVisible(1);
    ui->CreationTrackListTitle->setVisible(1);
    ui->CreationTrackName->setVisible(1);
    ui->CreationTrackTime->setVisible(1);
}
void SplitIt::disableCreationUI()
{
    ui->Background->setFocus();
    ui->CreationBG->setVisible(0);
    ui->CreationAdd->setVisible(0);
    ui->CreationCurrentTime->setVisible(0);
    ui->CreationLoad->setVisible(0);
    ui->CreationNewTrack->setVisible(0);
    ui->CreationRemove->setVisible(0);
    ui->CreationSave->setVisible(0);
    ui->CreationTrackList->setVisible(0);
    ui->CreationTrackListTitle->setVisible(0);
    ui->CreationTrackName->setVisible(0);
    ui->CreationTrackTime->setVisible(0);
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

    channel->getPosition(&position, FMOD_TIMEUNIT_MS);
    int currentTimeSeconds = (position/1000)%60;
    int currentTimeMinutes = ((position/1000 - currentTimeSeconds)%3600)/60;
    int currentTimeHours = (position/1000 - (currentTimeMinutes * 60) - currentTimeSeconds)/3600;
    QString minutes;
    QString seconds;
    if(currentTimeMinutes < 10)
    {
        minutes = QString("0") + QString::number(currentTimeMinutes);
    }
    else
    {
        minutes = QString::number(currentTimeMinutes);
    }
    if(currentTimeSeconds < 10)
    {
        seconds = QString("0") + QString::number(currentTimeSeconds);
    }
    else
    {
        seconds = QString::number(currentTimeSeconds);
    }
    QString currentTime = QString::number(currentTimeHours) + QString(":") + minutes + QString(":") + seconds;

    ui->CreationTrackTime->setText(currentTime);
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

    QString qFilePath = NULL;
    qFilePath = QFileDialog::getOpenFileName(this, tr("Select Audio File"),
                                                     "C://",
                                                     tr("SplitIt Config (*.txt)"));
    if(qFilePath != NULL)
    {
        QFile configFile(qFilePath);
        if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        trackArray.clear();
        nameArray.clear();
        QTextStream configStream(&configFile);
        int lineNumber = 1;
        // Read Config
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

    ui->CreationTrackList->clear();
    ui->CreationTrackList->addItems(nameArray);
    ui->List->clear();
    ui->List->addItems(nameArray);

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

    QStringList saveFile;

    saveFile.append(audioFilePath);
    saveFile.append(ui->STName->text());
    int line = 3;
    while(line != (trackArray.count() * 2 + 3))
    {
        saveFile.append(QString("A ").append(QString::number(trackArray.at((line - 3)/2))));
        saveFile.append(QString("B ").append(nameArray.at((line - 3)/2)));
        line = line + 2;
    }
    saveFile.append(QString("end"));

    QString fileName = QCoreApplication::applicationDirPath() + QString("/soundtracks/")
            + audioFile.fileName() + ".txt";
    if(!QDir(QCoreApplication::applicationDirPath() + QString("/soundtracks/")).exists())
    {
        QDir().mkdir(QCoreApplication::applicationDirPath() + QString("/soundtracks/"));
    }
    QFile newConfig(fileName);
    newConfig.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&newConfig);
    output << saveFile.at(0) << "\n";
    int i = 0;
    while(saveFile.at(i) != "end")
    {
        output << saveFile.at(i + 1) << "\n";
        i = i + 1;
    }
    newConfig.close();
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


void SplitIt::on_CreationAdd_pressed()
{
    ui->CreationAdd->setPixmap(QPixmap(":/images/CreationUI/AddPressed"));

}
void SplitIt::on_CreationAdd_released()
{
    ui->CreationAdd->setPixmap(QPixmap(":/images/CreationUI/Add"));
    bool valid = true;

    QString newTrackName = ui->CreationTrackName->text();
    QString newTrackTime = ui->CreationTrackTime->text();
    int timeStringLength = newTrackTime.size();

    newTrackTime.right(2).toInt(&valid, 10);
    if(valid == true)
    {
        newTrackTime.right(5).left(2).toInt(&valid, 10);
    }
    if(valid == true)
    {
        if(newTrackTime[timeStringLength - 3] != QChar(':'))
        {
            valid = false;
        }
    }
    if(timeStringLength > 5)
    {
        if(valid == true)
        {
            newTrackTime.left(timeStringLength - 6).toInt(&valid, 10);
        }
        if(newTrackTime[timeStringLength - 6] != QChar(':'))
        {
            valid = false;
        }
    }
    int secondsCheck = newTrackTime.right(2).left(1).toInt();
    int minutesCheck = newTrackTime.right(5).left(1).toInt();
    if((secondsCheck >= 6) || (minutesCheck >= 6))
    {
        valid = false;
    }
    if(valid == true)
    {
        int newTrackTimeSeconds;
        if(timeStringLength > 5)
        {
            newTrackTimeSeconds = newTrackTime.right(2).toInt()
                    + (newTrackTime.right(5).left(2).toInt() * 60)
                    + (newTrackTime.left(timeStringLength - 6).toInt() * 3600);
        }
        else
        {
            newTrackTimeSeconds = newTrackTime.right(2).toInt()
                        + (newTrackTime.right(5).left(2).toInt() * 60);
        }
        int newTrackIndex = 0;
        if(trackArray.count() == 0)
        {
            trackArray.insert(0, newTrackTimeSeconds);
            nameArray.insert(0, newTrackName);
        }
        else
        {
            for(int i = 1; i != trackArray.count(); i = i + 1)
            {
                if(trackArray.at(i - 1) < newTrackTimeSeconds)
                {
                    newTrackIndex = i;
                }
            }
            trackArray.insert(newTrackIndex, newTrackTimeSeconds);
            nameArray.insert(newTrackIndex, newTrackName);
        }
        ui->CreationTrackList->clear();
        ui->CreationTrackList->addItems(nameArray);
        ui->List->clear();
        ui->List->addItems(nameArray);
    }
    else
    {
        QMessageBox invalidTime;
        invalidTime.setText("Invalid Time");
        invalidTime.exec();
    }
}
void SplitIt::on_CreationAdd_hovered()
{
    ui->CreationAdd->setPixmap(QPixmap(":/images/CreationUI/AddHovered"));
}
void SplitIt::on_CreationAdd_unhovered()
{
    ui->CreationAdd->setPixmap(QPixmap(":/images/CreationUI/Add"));
}


void SplitIt::on_CreationRemove_pressed()
{
    ui->CreationRemove->setPixmap(QPixmap(":/images/CreationUI/RemovePressed"));
}
void SplitIt::on_CreationRemove_released()
{
    ui->CreationRemove->setPixmap(QPixmap(":/images/CreationUI/Remove"));

    if(ui->CreationTrackList->row(ui->CreationTrackList->currentItem()) > -1)
    {
        int trackIndex = ui->CreationTrackList->row(ui->CreationTrackList->currentItem());
        trackArray.removeAt(trackIndex);
        nameArray.removeAt(trackIndex);
        ui->CreationTrackList->clear();
        ui->CreationTrackList->addItems(nameArray);
        ui->List->clear();
        ui->List->addItems(nameArray);
    }
}
void SplitIt::on_CreationRemove_hovered()
{
    ui->CreationRemove->setPixmap(QPixmap(":/images/CreationUI/RemoveHovered"));
}
void SplitIt::on_CreationRemove_unhovered()
{
    ui->CreationRemove->setPixmap(QPixmap(":/images/CreationUI/Remove"));
}

void SplitIt::on_CreationTrackList_itemPressed(QListWidgetItem *item)
{
    ui->CreationTrackList->setCurrentItem(item);
    int trackIndex = ui->CreationTrackList->row(item);
    ui->CreationTrackName->setText(nameArray.at(trackIndex));

    for(int i = 0; i < ui->List->count(); i = i + 1)
    {
        ui->CreationTrackList->item(i)->setTextColor(QColor(255, 255, 255, 255));
    }
    item->setTextColor(QColor(0, 191, 243, 255));

    int trackTime = trackArray.at(trackIndex);
    int trackTimeSeconds = trackTime%60;
    int trackTimeMinutes = ((trackTime - trackTimeSeconds)%3600)/60;
    int trackTimeHours = (trackTime - (trackTimeMinutes * 60) - trackTimeSeconds)/3600;
    QString minutes;
    QString seconds;
    if(trackTimeMinutes < 10)
    {
        minutes = QString("0") + QString::number(trackTimeMinutes);
    }
    else
    {
        minutes = QString::number(trackTimeMinutes);
    }
    if(trackTimeSeconds < 10)
    {
        seconds = QString("0") + QString::number(trackTimeSeconds);
    }
    else
    {
        seconds = QString::number(trackTimeSeconds);
    }
    QString trackTimeString = QString::number(trackTimeHours) + QString(":") + minutes + QString(":") + seconds;
    ui->CreationTrackTime->setText(trackTimeString);
}


void SplitIt::on_SeekKnob_pressed()
{
    seekButton = true;
    ui->SeekKnob->setPixmap(QPixmap(":/images/VolumeKnobClicked"));

}
void SplitIt::on_SeekKnob_released()
{
    seekButton = false;
    ui->SeekKnob->setPixmap(QPixmap(":/images/VolumeKnob"));

    if(ui->SeekKnob->pos().x() <= 659 && ui->SeekKnob->pos().x() >= 173)
    {
        float timePortion = float(ui->SeekKnob->pos().x() - 173)/float(486);
        int newPosition = timePortion * length;
        channel->setPosition(newPosition, FMOD_TIMEUNIT_MS);
    }

}
void SplitIt::on_SeekKnob_hovered()
{
    if(seekButton == false)
    {
        ui->SeekKnob->setPixmap(QPixmap(":/images/VolumeKnobHover"));
    }
}
void SplitIt::on_SeekKnob_unhovered()
{
    ui->SeekKnob->setPixmap(QPixmap(":/images/VolumeKnob"));
}

void SplitIt::on_List_itemPressed(QListWidgetItem *item)
{
    int trackIndex = ui->List->row(item);
    ui->SongName->setText(nameArray.at(trackIndex));
    channel->setPosition(trackArray.at(trackIndex), FMOD_TIMEUNIT_MS);

    for(int i = 0; i < ui->List->count(); i = i + 1)
    {
        ui->List->item(i)->setTextColor(QColor(255, 255, 255, 255));
    }
    item->setTextColor(QColor(0, 191, 243, 255));

    float timePortion = float(ui->SeekKnob->pos().x() - 173)/float(486);
    int newPosition = timePortion * length;
    channel->setPosition(newPosition, FMOD_TIMEUNIT_MS);
}
