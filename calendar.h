#ifndef CALENDAR_H
#define CALENDAR_H

#include <QtGui>

class calendar
{
public:
    calendar(int, int, int, int);
    bool        isOnAir();
    QDateTime   getNextOnAirDateTime(int f = 0);
    QString     timeLeft();
private:
    QDateTime   onAir;
    int         dayOfWeek;
    int         hh;
    int         mm;
    int         len;
};

#endif // CALENDAR_H
