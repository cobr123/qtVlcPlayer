#include <QtGui>
#include <QDebug>

#include "qtvlc.h"
#include "vlcplayer.h"
#include "button.h"

vlcPlayer::vlcPlayer(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *Layout1 = new QHBoxLayout;

    playBtn = createButton("Play", SLOT(play()));
    stopBtn = createButton("Stop", SLOT(stop()));
    pauseBtn = createButton("Pause", SLOT(pause()));

    volumeSlider = new QSlider(Qt::Orientation(Qt::Horizontal));
    volumeSlider->setValue(100);
    connect(volumeSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(volumeChanged()));

    Layout1->addWidget(playBtn);
    Layout1->addWidget(pauseBtn);
    Layout1->addWidget(stopBtn);
    Layout1->addWidget(volumeSlider);

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

}

vlcPlayer::~vlcPlayer()
{
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

void vlcPlayer::positionChanged()
{
    qtVlcOut->setPosition(positionSlider->value(), qtVlcSource->currentTime());
}

void vlcPlayer::volumeChanged()
{
    qtVlcOut->setVolume(volumeSlider->value());
}

void vlcPlayer::timeChanged()
{
    if(!positionSlider->isSliderDown())
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
    if(positionSlider->isSliderDown())
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


void vlcPlayer::play()
{
    qDebug() << "play";
    playBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    stopBtn->setEnabled(true);
    if(qtVlcSource->getUrl() == "" || !qtVlcSource->isPlaying())
    {
        qtVlcSource->init("http://scfire-ntc-aa06.stream.aol.com:80/stream/1011", "tmp.wav");
        qtVlcSource->play();
        while(qtVlcSource->currentTime() <= 0)
        {
            qDebug() << "buffering...";
            QMutex dummy;
            dummy.lock();
            QWaitCondition waitCondition;
            waitCondition.wait(&dummy, 1000);
        }
    }
    if(qtVlcOut->getUrl() == "")
    {
        qtVlcOut->init("tmp.wav", 0);
    }
    qtVlcOut->setVolume(volumeSlider->value());
    qtVlcOut->play();
    qDebug() << qtVlcSource->getArtist() << qtVlcSource->getTitle() << qtVlcSource->getNowPlaying();
    this->setWindowTitle(qtVlcSource->getNowPlaying());
}
void vlcPlayer::stop()
{
    qDebug() << "stop";
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(false);
    qtVlcOut->stop();
    qtVlcSource->stop();
    statusTime->setText(
                QTime(0,0,0,0).toString("hh:mm:ss")
                + " / "
                + QTime(0,0,0,0).toString("hh:mm:ss")
                );
}
void vlcPlayer::pause()
{
    qDebug() << "pause";
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    qtVlcOut->pause();
}
