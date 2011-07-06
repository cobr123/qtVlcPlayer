#include <QtGui>
#include <QDebug>

#include "qtvlc.h"
#include "vlcplayer.h"
#include "button.h"
#include "playlist.h"

vlcPlayer::vlcPlayer(QUrl url, QWidget *parent) :
    QMainWindow(parent)
{
    statusBar()->showMessage("Initializing");
    mUrl = url;
    bPlaying = false;
    poller = new QTimer(this);
    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *Layout1 = new QHBoxLayout;

    playBtn = createButton("Play", SLOT(play()));
    stopBtn = createButton("Stop", SLOT(stop()));
    pauseBtn = createButton("Pause", SLOT(pause()));

    volumeSlider = new QSlider(Qt::Orientation(Qt::Horizontal));
    volumeSlider->setValue(50);
    connect(volumeSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(volumeChanged()));

    Layout1->addWidget(playBtn);
    Layout1->addWidget(pauseBtn);
    Layout1->addWidget(stopBtn);
    Layout1->addWidget(volumeSlider);

    timeLeft = new QLabel;
    timeLeft->setText("");
    timeLeft->setFont(QFont("Verdana", 20));

    mainLayout->addWidget(timeLeft);
    mainLayout->addLayout(Layout1);
    //mainLayout->addLayout(Layout2);

    positionSlider = new QSlider(Qt::Orientation(Qt::Horizontal));
    positionSlider->setValue(0);
    connect(positionSlider, SIGNAL(sliderReleased())
            ,this, SLOT(positionChanged()));
    connect(positionSlider, SIGNAL(sliderMoved(int))
            ,this, SLOT(updatePosTime()));
    mainLayout->addWidget(positionSlider);

    widget->setLayout(mainLayout);
    setWindowTitle(tr("Player"));

    statusTime = new QLabel(
                QTime(0,0,0,0).toString("hh:mm:ss")
                + " / "
                + QTime(0,0,0,0).toString("hh:mm:ss")
                            );
    statusBar()->addPermanentWidget(statusTime);

    qtVlcSource = new qtVlc(0);
    qtVlcOut = new qtVlc(0);

    connect(qtVlcSource, SIGNAL(timeChanged())
            ,this, SLOT(timeChanged()));

    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(false);
    statusBar()->showMessage("Ready to play");
    poller->start(1000);

    //6 = "Saturday" onAir day, 23 - hh onAir time, 00 - mm onAir time, 3 - onAir hours length
    //cal = new calendar(6, 23, 00, 3);
    cal = new calendar(6, 23, 00, 3);
    if(cal->isOnAir())
    {
        play();
    }
}

vlcPlayer::~vlcPlayer()
{
    bPlaying = false;
    if(qtVlcOut->getUrl() != "")
    {
        qDebug() << "~vlcPlayer qtVlcOut";
        delete qtVlcOut;
    }

    if(qtVlcSource->getUrl() != "")
    {
        qDebug() << "~vlcPlayer qtVlcSource";
        delete qtVlcSource;
    }
}

void vlcPlayer::updateInterface()
{
    timeLeft->setText(cal->timeLeft());
    if(bPlaying)
    {
        emit timeChanged();
    }
}

void vlcPlayer::positionChanged()
{
    if(qtVlcOut->getUrl() != "")
    {
        qtVlcOut->setPosition(positionSlider->value(), qtVlcSource->currentTime());
    }
}

void vlcPlayer::volumeChanged()
{
    if(qtVlcOut->getUrl() != "")
    {
        qtVlcOut->setVolume(volumeSlider->value());
    }
}

void vlcPlayer::timeChanged()
{
    if(!positionSlider->isSliderDown() && qtVlcOut->getUrl() != "")
    {
        statusTime->setText(
                    QTime(0,0,0,0).addMSecs(qtVlcOut->currentTime()).toString("hh:mm:ss")
                    + " / "
                    + QTime(0,0,0,0).addMSecs(qtVlcSource->currentTime()).toString("hh:mm:ss")
                    );
        positionSlider->setValue(qtVlcOut->currentTime());
        positionSlider->setMaximum(qtVlcSource->currentTime());
    }
}

void vlcPlayer::updatePosTime()
{
    if(positionSlider->isSliderDown() && qtVlcOut->getUrl() != "")
    {
        statusTime->setText(
                    QTime(0,0,0,0).addMSecs(positionSlider->value()).toString("hh:mm:ss")
                    + " / "
                    + QTime(0,0,0,0).addMSecs(qtVlcSource->currentTime()).toString("hh:mm:ss")
                    );
    }
}

Button *vlcPlayer::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void vlcPlayer::restartSource()
{
    if(bPlaying)
    {
        qDebug() << "restartSource()";
        if(!qtVlcOut->isPlaying())
        {
            pause();

            QMutex dummy;
            dummy.lock();
            QWaitCondition waitCondition;
            waitCondition.wait(&dummy, 500);

            play();
        }
        else
        {
            playSource();
        }
    }
}

void vlcPlayer::playSource()
{
    if(qtVlcSource->getUrl() == "" || !qtVlcSource->isPlaying())
    {
        if(trackUrl == "")
        {
            statusBar()->showMessage("Getting playlist...");
            qDebug() << "getPlsFirstTrack...";
            playList *pl = new playList(mUrl);
            if (pl->getPlsFirstTrack() == "")
            {
                stop();
                return;
            }
            trackUrl = pl->getPlsFirstTrack();
        }
        qtVlcSource->init(trackUrl.toUtf8().constData(), "tmp.wav"); //"http://scfire-ntc-aa06.stream.aol.com:80/stream/1011"
        qtVlcSource->play();
        statusBar()->showMessage("Buffering...");
        while(qtVlcSource->currentTime() <= 0)
        {
            qDebug() << "buffering...";
            QMutex dummy;
            dummy.lock();
            QWaitCondition waitCondition;
            waitCondition.wait(&dummy, 1000);
        }
    }
}

void vlcPlayer::play()
{
    qDebug() << "play";
    playBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    stopBtn->setEnabled(true);
    bPlaying = true;
    playSource();
    if(qtVlcOut->getUrl() == "")
    {
        qtVlcOut->init("tmp.wav", 0);
    }
    qtVlcOut->setVolume(volumeSlider->value());
    qtVlcOut->play();
    qDebug() << qtVlcSource->getArtist() << qtVlcSource->getTitle() << qtVlcSource->getNowPlaying();
    //this->setWindowTitle(qtVlcSource->getNowPlaying());
    statusBar()->showMessage(qtVlcSource->getNowPlaying());
}

void vlcPlayer::stop()
{
    qDebug() << "stop";
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(false);
    bPlaying = false;
    if(qtVlcOut->isPlaying())
    {
        qtVlcOut->stop();
    }
    if(qtVlcSource->isPlaying())
    {
        qtVlcSource->stop();
    }
    positionSlider->setValue(0);
    statusBar()->showMessage("Stopped");
}
void vlcPlayer::pause()
{
    qDebug() << "pause";
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    qtVlcOut->pause();
    statusBar()->showMessage("Paused");
}
