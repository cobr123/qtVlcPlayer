QT += network

LIBS += -I"C:\Users\rtabulov\qt\qtVlc\vlcSrc\include"
LIBS += -L"C:\Users\rtabulov\qt\qtVlc\vlcBin" #if you are at windows os
LIBS += -llibvlc

SOURCES += \
    main.cpp \
    button.cpp \
    vlcplayer.cpp \
    qtvlc.cpp \
    playlist.cpp

HEADERS += \
    button.h \
    vlcplayer.h \
    qtvlc.h \
    playlist.h
