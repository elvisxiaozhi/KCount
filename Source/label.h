#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT
public:
    Label();

private:
    void setContextMenu();
};

#endif // LABEL_H
