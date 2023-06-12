#include "mainui.h"
#include "ui_mainui.h"

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
{
    ui->setupUi(this);

//    auto& m_bubbleHistory = ui->textBrowser;
//    m_bubbleHistory->addBubble(BubbleParas("question", BUBBLE_ROLE::BR_ME));
//    m_bubbleHistory->addBubble(BubbleParas("当然可以！下面是一篇稍长一点的介绍：\n\n人aqwer", BUBBLE_ROLE::BR_ME));
//    m_bubbleHistory->addBubble(BubbleParas("当然可以！下面是一篇稍长一点的介绍：\n\n人aqwer", BUBBLE_ROLE::BR_AICHAT));
//    m_bubbleHistory->addBubble(BubbleParas("question", BUBBLE_ROLE::BR_ME));
//    m_bubbleHistory->addBubble(BubbleParas("用于获取文本的字符数，blockCount() 用于获", BUBBLE_ROLE::BR_AICHAT));
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::on_btnSend_released()
{

}


void MainUI::on_btnGenerate_released()
{

}


void MainUI::on_btnGenerate_pressed()
{

}

