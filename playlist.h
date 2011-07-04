#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDebug>

class playList : public QObject
{
    Q_OBJECT
public:
    explicit playList(QUrl url, QObject *parent = 0);
    QString  getPlsFirstTrack();

private slots:
    void httpReadyRead();

private:
    QNetworkReply *reply;
    QNetworkAccessManager qnam;
    QByteArray   data;
    QString      playListType;
    // event loop used to block until request finished
    QEventLoop loop;

};

#endif // PLAYLIST_H
