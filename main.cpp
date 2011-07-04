#include <QApplication>

#include "vlcplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    vlcPlayer player;

//    QStatusBar  *statusBar = new QStatusBar();
//    QLabel      *statusLabel = new QLabel("Ready");
//    statusBar->addPermanentWidget(statusLabel);


    player.show();

    return app.exec();
}

