#include "playlist.h"

playList::playList(QUrl url, QObject *parent) :
    QObject(parent)
{
    //url = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1377323";
    //url = lurl; //"http://stream.radio-t.com:8181/stream.m3u";
    reply = qnam.get(QNetworkRequest(url));

    QFileInfo fileInfo(url.path());
    //qDebug() << fileInfo.fileName();
    playListType = fileInfo.suffix().toLower();

    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    loop.exec();
}

void playList::httpReadyRead()
{
    loop.exit();
    data = reply->readAll();
    if (playListType == "pls")
    {
        qDebug() << "httpReadyRead" << getPlsFirstTrack();
    }
    else
    {
        qDebug() << "httpReadyRead" << "unknown playlist format";
    }
    reply->deleteLater();
    reply = 0;
}

/*
Example:

[Playlist]
NumberOfEntries=1
File1=http://www.panix.com/web/faq/multimedia/sample.mp3
Title1=Bird Song
Length1=21
Version=2
*/
QString playList::getPlsFirstTrack()
{
    QString res;
    QRegExp rx("File1=([^\n]+)");
    if(rx.indexIn(data, 0) != -1)
    {
        res = rx.cap(1);
    }
    //qDebug() << "getPlsFirstTrack" << res <<data.size();
    return res;
}
