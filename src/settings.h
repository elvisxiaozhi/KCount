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
#include <QCheckBox>
#include "applimits.h"

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
    QToolButton *limitsBtn, *aboutBtn;
    QWidget *limitsWidget;
    QVBoxLayout *limitsVLayout, *limitsTabVLayout, *listsVLayout, *limitsBottomVLayout;
    QTabWidget *tabWidget;
    QWidget *limitedTab, *allowedTab;
    QWidget *limitedListWidget;
    QVector<QToolButton *> btnVec;
    QVector<QLineEdit *> lineEditVec;
    QVector<QPushButton *> deleteBtnVec;
    QPushButton *limitedAddBtn, *okBtn, *cancelBtn;
    QCheckBox *limitsCheckBox;
    QLineEdit *limitsEdit;
    AppLimits *appLimits;

    void setWindowStyleSheet();
    void setWindowLayout();
    QToolButton *createToolBtn(QString);
    void createLimitsWidget();
    void createLimitsTabConts();
    void createLimitsBottomWidget();
    void removeLimitsListWidget();
    QWidget *createLimitCBLayout();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void delBtnClicked(int);

private slots:
    void updateLimitsWidget(bool);
    void limitsBtnClicked(QString, bool);
    void deleteBtnClicked(int);
    void limitedAddBtnClicked();
    void okBtnClicked();
    void showToolBtn(QString, bool);
};

#endif // SETTINGS_H
