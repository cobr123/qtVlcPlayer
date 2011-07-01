#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include "qtvlc.h"

#include <QWidget>

class Button;

class vlcPlayer : public QWidget
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

private:
    Button *createButton(const QString &text, const char *member);
    qtVlc  *qtVlcSource;
    qtVlc  *qtVlcOut;
    Button *playBtn;
    Button *stopBtn;
    Button *pauseBtn;
};

#endif // VLCPLAYER_H
