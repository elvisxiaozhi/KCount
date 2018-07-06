#ifndef APPLIMITS_H
#define APPLIMITS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

class AppLimits : public QWidget
{
    Q_OBJECT
public:
    explicit AppLimits(QWidget *parent = nullptr);

    void createMainLayout();
    void updateLimitedList(bool clicked);
    void removeLimitedList();

private:
    QVBoxLayout *mainVLayout, *tabVLayout, *listsVLayout, *btmVLayout;
    QTabWidget *tabWidget;
    QWidget *limitedTab, *allowedTab;
    QCheckBox *checkBox;
    QLineEdit *limitsEdit;
    QWidget *limitedList;
    QVector<QLineEdit *> lineEditVec;
    QVector<QPushButton *> delBtnVec;
    QPushButton *addBtn, *okBtn, *cancelBtn;

    void setWidgetStyleSheet();
    void createTabConts();
    QWidget *createCBLayout();
    void createBtmLayout();
    void initWidgets();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void delBtnClicked(int);

private slots:
    void addBtnClicked();
    void delLimitedApp(int);
    void addOrCxlClicked();
};

#endif // APPLIMITS_H
