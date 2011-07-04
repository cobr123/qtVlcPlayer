#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include "qtvlc.h"

#include <QtGui>

class Button;

class vlcPlayer : public QMainWindow
{
    Q_OBJECT
public:
    vlcPlayer(QWidget *parent = 0);
    ~vlcPlayer();

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

private:
    Button *createButton(const QString &text, const char *member);
    qtVlc  *qtVlcSource;
    qtVlc  *qtVlcOut;
    Button *playBtn;
    Button *stopBtn;
    Button *pauseBtn;
    QSlider *positionSlider;
    QSlider *volumeSlider;
    QLabel  *statusTime;
};

#endif // VLCPLAYER_H
