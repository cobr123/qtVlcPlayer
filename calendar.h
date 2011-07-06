#ifndef CALENDAR_H
#define CALENDAR_H

#include <QtGui>

class calendar
{
public:
    calendar();
    ~calendar();
    bool        isOnAir();
    QDateTime   getNextOnAirDateTime(int f = 0);
    QString     timeLeft();
private:
    void    readSettings();
    void    writeSettings();
    QDateTime   onAir;
    int         dayOfWeek;
    int         hh;
    int         mm;
    int         len;
};

#endif // CALENDAR_H
