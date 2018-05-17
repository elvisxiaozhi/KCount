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
    if(times >= 5000) {
        setStyleSheet(QString("%1 QLabel#Label { background: #E74C3C; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
    if(times >= 10000) {
        setStyleSheet(QString("%1 QLabel#Label { background: #A93226; border: 1px solid #F1C40F; }").arg(commomStyleSheet));
    }
}
