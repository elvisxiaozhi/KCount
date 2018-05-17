#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel
{
public:
    Label(unsigned long int, int);

    void setLabelColor(const unsigned long int &);

private:
    unsigned long int lblTimes;
    int fontSize;
    QString commomStyleSheet;
};

#endif // LABEL_H
