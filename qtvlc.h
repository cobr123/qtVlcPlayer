#ifndef QTVLC_H
#define QTVLC_H

#include <QtCore>

#include <vlc/vlc.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

class qtVlc : public QObject
{
    Q_OBJECT
public:
    explicit qtVlc(QObject *parent);
    void init(const char* url, const char* TmpFile);
    ~qtVlc();
    void play();
    void stop();
    void pause();
    QString getUrl() { return mUrl; }
    int    isPlaying() { return libvlc_media_player_is_playing(mp); }
    int    currentTime();
    void   setPosition(int, int);
    void   setVolume(int);
    QString  getTitle();
    QString  getArtist();
    QString  getNowPlaying();

signals:
    void playbackCommenced();
    void timeChanged();
private slots:

private:
    libvlc_event_manager_t *m_eventManager;
    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    QString    mUrl;

    void connectToPlayerVLCEvents();
    static void eventCallback(const libvlc_event_t *event, void *data);
};

#endif // QTVLC_H
