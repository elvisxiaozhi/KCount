#ifndef MOSTUSED_H
#define MOSTUSED_H

#include <QWidget>
#include <QVBoxLayout>
#include <QElapsedTimer>
#include <label.h>
#include <QMap>
#include "custombutton.h"
#include <QHBoxLayout>
#include <QScrollArea>

class MostUsed : public QWidget
{
    Q_OBJECT
public:
    explicit MostUsed(QWidget *parent = nullptr, int mode = 1, QString title = "Most Used");

    void setData();

private:
    QVBoxLayout *mainVLayout, *contVLayout;
    QHBoxLayout *headerHLayout;
    QWidget *contWidget;
    QElapsedTimer timer;
    QVector<std::pair<QString, int> > mostUsedVec;
    QMap<QString, int> tempAppMap;
    QVector<Label *> contents;
    QString lastAppName;
    CustomButton *showMoreBtn, *showLessBtn;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollContVLayout;
    QVector<Label *> scrollConts;
    QVector<QString> alertedApp;

    void setMainLayout(QString);
    void setWindowStyleSheet();
    void setContents();
    void setLblText(Label *, QString, int);
    void createScrollWidget();
    void createScrollConts();
    bool hasAppReachedLimit();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void limitAppAlert(QString);

public slots:
    void updateDatabase();

private slots:
    void appChanged(QString);
};

#endif // MOSTUSED_H
