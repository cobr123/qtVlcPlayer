#include <QApplication>

#include "vlcplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    vlcPlayer player;

#if defined(Q_OS_SYMBIAN)
    player.showMaximized();
#else
    player.show();
#endif
    return app.exec();
}

