#include "calendar.h"

/*
1 = "Monday"
2 = "Tuesday"
3 = "Wednesday"
4 = "Thursday"
5 = "Friday"
6 = "Saturday"
7 = "Sunday"
*/
calendar::calendar()
{
    //6 = "Saturday" onAir day, 23 - hh onAir time, 00 - mm onAir time, 3 - onAir hours length
    readSettings();
    onAir = getNextOnAirDateTime();
    qDebug() << onAir.toString("dd.MM.yyyy hh:mm:ss")
             << "onAir day = " + onAir.date().longDayName(onAir.date().dayOfWeek())
             << "onAir length = " + QString::number(len);
    qDebug() << "is onAir: " << isOnAir();
}

calendar::~calendar()
{
    writeSettings();
}

void calendar::readSettings()
{
    QSettings settings("Cobr Soft", "Player");

    dayOfWeek = settings.value("onAir/day", 6).toInt();
    hh = settings.value("onAir/hour", 23).toInt();
    mm = settings.value("onAir/minute", 0).toInt();
    len = settings.value("onAir/length", 3).toInt();
}

void calendar::writeSettings()
{
    QSettings settings("Cobr Soft", "Player");

    settings.setValue("onAir/day", dayOfWeek);
    settings.setValue("onAir/hour", hh);
    settings.setValue("onAir/minute", mm);
    settings.setValue("onAir/length", len);
}

bool    calendar::isOnAir()
{
    QDateTime min = onAir;
    QDateTime max = onAir.addSecs(60*60*len);
    QDateTime now = QDateTime().currentDateTime();
//    qDebug() << min.toString();
//    qDebug() << now.toString();
//    qDebug() << max.toString();
    return (min <= now) && (now <= max);
}

QDateTime   calendar::getNextOnAirDateTime(int f)
{
    int y = QDate().currentDate().year();
    int m = QDate().currentDate().month();
    int d = QDate().currentDate().day();

    while((QDate(y, m, d).dayOfWeek() != dayOfWeek) || (QDateTime().currentDateTime() > QDateTime(QDate(y, m, d), QTime(hh, mm)) && f == 1))
    {
        ++d;
    }
    return QDateTime(QDate(y, m, d), QTime(hh, mm));
}

QString   calendar::timeLeft()
{
    QString msg;
    if(isOnAir())
    {
        msg = "onAir";
    }
    else
    {
        if(onAir < QDateTime().currentDateTime())
        {
            onAir = getNextOnAirDateTime(1);
        }
        int secTo = QDateTime().currentDateTime().secsTo(onAir);
        int days = secTo/60/60/24;
        int daysSec = days*60*60*24;
        int hours = (secTo - daysSec)/60/60;
        int hoursSec = hours*60*60;
        int minutes = (secTo - daysSec - hoursSec)/60;
        int seconds = (secTo - daysSec - hoursSec - minutes*60);
        msg =      QString::number(days) + "d "
                 + QString::number(hours) + "h "
                 + QString::number(minutes) + "m "
                 + QString::number(seconds) + "s";
    }
    return msg;
}
