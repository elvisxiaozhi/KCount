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
    QPushButton *addBtn, *okBtn, *cancelBtn;

    void setWidgetStyleSheet();
    void createTabConts();
    QWidget *createCBLayout();
    void createBtmLayout();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void addBtnClicked();
};

#endif // APPLIMITS_H
