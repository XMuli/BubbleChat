#ifndef MAINUI_H
#define MAINUI_H

#include <QPointer>
#include <QWidget>
#include "widgets/chathistory.h"
#include "widgets/chatinputedit.h"

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

private:
    void initUI();

private:
    Ui::MainUI *ui;
};

#endif // MAINUI_H
