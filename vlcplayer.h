#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include "qtvlc.h"
#include "calendar.h"

#include <QtGui>

class Button;

class vlcPlayer : public QMainWindow
{
    Q_OBJECT
public:
    vlcPlayer(QWidget *parent = 0);
    ~vlcPlayer();
    bool isPlaying() { return bPlaying; }

signals:

public slots:

private slots:
    void play();
    void stop();
    void pause();
    void timeChanged();
    void positionChanged();
    void volumeChanged();
    void updatePosTime();
    void restartSource();
    void updateInterface();

private:
    void    playSource();
    void    readSettings();
    void    writeSettings();
    Button *createButton(const QString &text, const char *member);
    qtVlc  *qtVlcSource;
    qtVlc  *qtVlcOut;
    Button *playBtn;
    Button *stopBtn;
    Button *pauseBtn;
    QSlider *positionSlider;
    QSlider *volumeSlider;
    QLabel  *statusTime;
    QUrl    mUrl;
    QString trackUrl;
    bool    bPlaying;
    QTimer  *poller;
    calendar    *cal;
    QLabel      *timeLeft;
};

#endif // VLCPLAYER_H
