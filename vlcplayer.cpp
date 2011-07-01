#include <QtGui>
#include <QDebug>

#include "qtvlc.h"
#include "vlcplayer.h"
#include "button.h"

vlcPlayer::vlcPlayer(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *Layout1 = new QHBoxLayout;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
#else
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
#endif

    playBtn = createButton("Play", SLOT(play()));
    stopBtn = createButton("Stop", SLOT(stop()));
    pauseBtn = createButton("Pause", SLOT(pause()));

    Layout1->addWidget(playBtn);
    Layout1->addWidget(pauseBtn);
    Layout1->addWidget(stopBtn);

    mainLayout->addLayout(Layout1);
    //mainLayout->addLayout(Layout2);

    QSlider *seekPosSlider = new QSlider(Qt::Orientation(Qt::Horizontal));
    //seekPosSlider->set

    mainLayout->addWidget(seekPosSlider);

    setLayout(mainLayout);
    setWindowTitle(tr("Player"));

    qtVlcSource = new qtVlc(0);
    qtVlcOut = new qtVlc(0);

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
        while(!qtVlcSource->isPlaying())
        {
            qDebug() << "buffering...";
            QMutex dummy;
            dummy.lock();
            QWaitCondition waitCondition;
            waitCondition.wait(&dummy, 1000);
        }
    }
    qtVlcOut->init("tmp.wav", 0);
    qtVlcOut->play();
}
void vlcPlayer::stop()
{
    qDebug() << "stop";
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(false);
    qtVlcOut->stop();
    qtVlcSource->stop();
}
void vlcPlayer::pause()
{
    qDebug() << "pause";
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    qtVlcOut->pause();
}
