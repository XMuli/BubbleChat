#ifndef MAINUI_H
#define MAINUI_H

#include <QPointer>
#include <QString>
#include <QTimer>
#include <QWidget>
#include "widgets/bubble.h"
#include "widgets/bubblehistory.h"

namespace Ui {
class MainUI;
}

class MainUI : public QWidget
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = nullptr);
    ~MainUI();

private slots:
    void on_btnSend_released();
    void on_btnGenerate_released();
    void on_btnGenerate_pressed();
    void onGenerateText();

private:
    void initUI();

private:
    Ui::MainUI *ui;
    QPointer<BubbleHistory>      m_bubbleHistory;
    bool                         m_bBubbleDisplay;

    QTimer *m_timer;
    QString m_text;
};

#endif // MAINUI_H
