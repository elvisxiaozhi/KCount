#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <QAction>

class Label : public QLabel
{
    Q_OBJECT
public:
    Label();
    void setLblColor(int);

private:
    void setContextMenu();
    QAction *hourAct;
    QAction *dayAct;
    QAction *weekAct;
    QAction *monthAct;
    QAction *yearAct;

signals:
    void viewNodeChanged(int);

private slots:
    void emitViewModeSignal();
};

#endif // LABEL_H
