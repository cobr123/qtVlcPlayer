#include <QApplication>

#include "vlcplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    vlcPlayer player;
    player.show();

    return app.exec();
}

