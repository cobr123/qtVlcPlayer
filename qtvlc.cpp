#include "qtvlc.h"

#include <QtCore>
#include <QMutex>
#include <QWaitCondition>

qtVlc::qtVlc(QObject *parent)
    : QObject(parent)
{
    //qDebug() << "qtVlc";
    mUrl = "";
}

void qtVlc::connectToPlayerVLCEvents()
{
    // Get the event manager from which the media player send event
    m_eventManager = libvlc_media_player_event_manager(mp);
    libvlc_event_type_t eventsMediaPlayer[] = {
        libvlc_MediaPlayerPlaying,
        libvlc_MediaPlayerPaused,
        libvlc_MediaPlayerEndReached,
        libvlc_MediaPlayerStopped,
        libvlc_MediaPlayerEncounteredError,
        libvlc_MediaPlayerTimeChanged,
        libvlc_MediaPlayerTitleChanged,
        //libvlc_MediaPlayerPositionChanged, // What does this event do???
        libvlc_MediaPlayerSeekableChanged,
        //libvlc_MediaPlayerPausableChanged, // Phonon has no use for this
        libvlc_MediaPlayerBuffering,
        libvlc_MediaPlayerLengthChanged
    };
    int i_nbEvents = sizeof(eventsMediaPlayer) / sizeof(*eventsMediaPlayer);
    for (int i = 0; i < i_nbEvents; i++) {
        libvlc_event_attach(m_eventManager, eventsMediaPlayer[i],
                            eventCallback, this);
    }
}

void qtVlc::eventCallback(const libvlc_event_t *event, void *data)
{
    //static int i_first_time_media_player_time_changed = 0;
    //static bool b_media_player_title_changed = false;

    qtVlc *const that = static_cast<qtVlc *>(data);

    //qDebug() << "event:" << libvlc_event_type_name(event->type) << that->getUrl();

    // Media player events
    if (event->type == libvlc_MediaPlayerPlaying) {
        //qDebug() << "event:" << libvlc_event_type_name(event->type) << that->getUrl();
        emit that->playbackCommenced();
    }

    if (event->type == libvlc_MediaPlayerBuffering) {
        //qDebug() << "event:" << libvlc_event_type_name(event->type) << that->getUrl();
        //emit that->stateChanged(Phonon::BufferingState);
    }

    if (event->type == libvlc_MediaPlayerTimeChanged) {
        //qDebug() << "event:" << libvlc_event_type_name(event->type) << that->getUrl();
        emit that->timeChanged();
        //that->timeChanged();

    }
}

void qtVlc::setPosition(int pos, int max)
{
    float newPos = (float)pos / (float)max;
    //qDebug() << "newPosition" << newPos << (float)max;
    libvlc_media_player_set_position(mp, newPos);
}

int  qtVlc::currentTime()
{
    return libvlc_media_player_get_time(mp);
}

void qtVlc::init(const char* url, const char* TmpFile)
{
    if(TmpFile)
    {
        char result[1000];   // array to hold the result.

            strcpy(result,"--sout=file/ps:");

            strcat(result,TmpFile); // append string two to the result.
            qDebug() << QString(TmpFile);

        const char * const vlc_args[] = {
              "-I", "dummy", // Don't use any interface
              "--ignore-config", // Don't use VLC's config
              "--extraintf=logger", // Log anything
              "--verbose=2", // Be much more verbose then normal for debugging purpose
              result  // Stream to file
               };
        // We launch VLC
        inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    }
    else
    {
        inst = libvlc_new(0, NULL);
    }
    // Check if instance is running
    if(inst) {
        qDebug() << "libvlc-qt initialised by " << QString(url);
    } else {
        qDebug() << "libvlc-qt Error: libvlc failed to load!";
        exit(-100);
    }
    /* Create a new item */
    m = libvlc_media_new_path (inst, url);

    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);

    /* No need to keep the media now */
    libvlc_media_release (m);

    connectToPlayerVLCEvents();
    mUrl = QString(url);
}
void qtVlc::play()
{
    /* play the media_player */
    libvlc_media_player_play (mp);
}
void qtVlc::stop()
{
    /* stop the media_player */
    libvlc_media_player_stop (mp);
}
void qtVlc::pause()
{
    /* pause the media_player */
    libvlc_media_player_pause (mp);
}
qtVlc::~qtVlc()
{
    qDebug() << "~qtVlc" << mUrl;
    if(isPlaying())
    {
        /* Stop playing */
        stop();
    }
    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);
}

