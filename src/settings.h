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
    QVBoxLayout *limitsVLayout, *limitsTabVLayout, *listsVLayout, *limitsBottomVLayout;
    QTabWidget *tabWidget;
    QWidget *limitedTab, *allowedTab;
    QWidget *limitedListWidget;
    QVector<QLineEdit *> lineEditVec;
    QVector<QPushButton *> deleteBtnVec;
    QPushButton *limitedAddBtn, *okBtn, *cancelBtn;

    void setWindowStyleSheet();
    void setWindowLayout();
    void createLimitsLayout();
    void createLimitsWidget();
    void createLimitsTabConts();
    void createLimitsBottomWidget();
    void removeLimitsListWidget();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void delBtnClicked(int);

private slots:
    void updateLimitsWidget(bool clicked);
    void limitsBtnClicked(bool);
    void deleteBtnClicked(int);
    void limitedAddBtnClicked();
};

#endif // SETTINGS_H
