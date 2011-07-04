#include <QApplication>

#include "vlcplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    vlcPlayer player(QUrl("http://yp.shoutcast.com/sbin/tunein-station.pls?id=1177953"));
    player.show();

    return app.exec();
}

