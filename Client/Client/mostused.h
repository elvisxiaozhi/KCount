#ifndef MOSTUSED_H
#define MOSTUSED_H

#include <QWidget>
#include <QVBoxLayout>
#include <QElapsedTimer>
#include <label.h>
#include <QMap>

class MostUsed : public QWidget
{
    Q_OBJECT
public:
    explicit MostUsed(QWidget *parent = nullptr);

    void setData();

private:
    QVBoxLayout *mainVLayout, *contVLayout;
    QElapsedTimer timer;
    QVector<std::pair<QString, float> >mostUsedVec;
    QMap<QString, float> tempAppMap;
    QVector<Label *> contents;

    void setWindowStyleSheet();
    void setContents();

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void updateDatabase();
    void reloadData(int);

private slots:
    void appChanged(QString);
};

#endif // MOSTUSED_H
