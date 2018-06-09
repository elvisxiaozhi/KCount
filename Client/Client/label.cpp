#include "label.h"
#include <QDebug>

Label::Label(unsigned long int times, int size)
{
    lblTimes = times;
    fontSize = size;
    commomStyleSheet = QString("QLabel { color: #FDFEFE; font-size: %1px; }").arg(fontSize);

    setObjectName("Label");
    setAlignment(Qt::AlignCenter);
    setLabelColor(times);
}

void Label::setLabelColor(const unsigned long int &times)
{
    if(times < 100) {
        setStyleSheet(QString("%1 QLabel#Label { background: #FAD7A0; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(times >= 100 && times < 500) {
        setStyleSheet(QString("%1 QLabel#Label { background: #82E0AA; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(times >= 500 && times < 1000) {
        setStyleSheet(QString("%1 QLabel#Label { background: #5DADE2; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(times >= 1000 && times < 5000) {
        setStyleSheet(QString("%1 QLabel#Label { background: #7D3C98; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(times >= 5000 ) {
        setStyleSheet(QString("%1 QLabel#Label { background: #E74C3C; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(times >= 10000) {
        setStyleSheet(QString("%1 QLabel#Label { background: #A93226; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
}

void Label::setAppUsageLblColor(const unsigned long int &usedTime)
{
    if(usedTime < 1800) { //30 mins
        setStyleSheet(QString("%1 QLabel#Label { background: #FAD7A0; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(usedTime >= 1800 && usedTime < 3600) { // 30 mins to 1 hour
        setStyleSheet(QString("%1 QLabel#Label { background: #82E0AA; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(usedTime >= 3600 && usedTime < 5400) { //1 hour to 1 hour and a half
        setStyleSheet(QString("%1 QLabel#Label { background: #5DADE2; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(usedTime >= 5400 && usedTime < 7200) { //1 hour and a half to 2 hours
        setStyleSheet(QString("%1 QLabel#Label { background: #7D3C98; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(usedTime >= 7200 && usedTime < 9000) { //2 hours to 2 hours and a half
        setStyleSheet(QString("%1 QLabel#Label { background: #E74C3C; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(usedTime >= 9000) { //more than 2 hours
        setStyleSheet(QString("%1 QLabel#Label { background: #A93226; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
}
