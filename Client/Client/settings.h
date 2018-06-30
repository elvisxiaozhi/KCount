#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    static QSettings startOnBootSettings;

private:
    QVBoxLayout *mainVLayout, *scrollVLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *limitsHLayout;
    QToolButton *limitsBtn;
    QWidget *limitsWidget;
    QVBoxLayout *limitsVLayout, *limitsTabVLayout, *limitedListVLayout;
    QTabWidget *tabWidget;
    QWidget *limitedTab, *allowedTab;
    QVector<QString> limitedAppVec;
    QVector<QLineEdit *> lineEditVec;
    QVector<QPushButton *> deleteBtnVec;
    QHBoxLayout *limitsAddHLayout;

    void setWindowStyleSheet();
    void setWindowLayout();
    void createLimitsLayout();
    void createLimitsWidget();
    void createLimitsTabConts();
    void readXml();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void updateLimitsWidget();
};

#endif // SETTINGS_H
