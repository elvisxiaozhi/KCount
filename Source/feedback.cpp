#include "feedback.h"
#include <QHBoxLayout>

Feedback::Feedback(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Feedback");
    setWindowIcon(QIcon(":/Icons/Icons/feedback.png"));
    mainVLayout = new QVBoxLayout;
    setLayout(mainVLayout);
    feedbackEdit = new QTextEdit(this);
    QHBoxLayout *btnHLayout = new QHBoxLayout;
    sendBtn = new QPushButton(this);
    sendBtn->setText("Send");
    btnHLayout->addStretch();
    btnHLayout->addWidget(sendBtn);
    mainVLayout->addWidget(feedbackEdit);
    mainVLayout->addLayout(btnHLayout);

    setWindowStyleSheet();
}

void Feedback::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #FAE5D3; font-family: Comic Sans MS; }"
                ".QPushButton { background-color: #3498DB; font-size: 15px; font-family: Comic Sans MS; color: white; border-radius: 15px; border: 2px solid #FF5A5F; padding: 5px 10px; margin: 5px 5px; }"
                ".QPushButton:hover { background-color: #BB8FCE; font-size: 18px; margin: 5px 2px; }"
                ".QPushButton:pressed { background-color: #EC7063 }"
                );
}
