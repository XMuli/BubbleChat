﻿// SPDX-SnippetCopyrightText: 2023-2025 Wengeng Zhang
// SPDX-Author: Wengeng Zhang <xmulitech@gmail.com>
// SPDX-License-Identifier: MIT
#include "mainui.h"
#include "ui_mainui.h"
#include <QDebug>
#include <QChar>
#include <QString>
#include <QRandomGenerator>

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
    , m_bubbleHistory(new BubbleHistory())
    , m_bBubbleDisplay(true)
{
    ui->setupUi(this);
    initUI();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainUI::onGenerateText);
    connect(ui->editChatInput, &ChatInputEdit::sigCustomKeyBtnReleased, this, &MainUI::onCustomKeyBtnReleased);
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::onCustomKeyBtnReleased(const CustomKey &customKey)
{
    if (customKey == CustomKey::CK_EnterSend) {
        on_btnSend_released();
    } else if (customKey == CustomKey::CK_CRLF) {
        // 什么都不做，即会自动添加换行的
    }
}

void MainUI::on_btnSend_released()
{
    qDebug() << "on_btnSend_released:" <<  sender();
    auto& chatInput = ui->editChatInput;

    const QString originMsg = chatInput->toPlainText();
    if (originMsg.isEmpty()) return;
    QString msg = originMsg;
    if (msg.endsWith('\n')) msg.chop(1);  // fix: end is '\n' change  chatPara.m_messages is error!!!

    m_bubbleHistory->addBubble(BubbleParas(msg, BUBBLE_ROLE::BR_ME));
    chatInput->clear();

    m_bubbleHistory->scrollToBottom();
}


void MainUI::on_btnGenerate_released()
{
    m_timer->stop();
    m_text.clear();
}


void MainUI::on_btnGenerate_pressed()
{
    m_timer->start(100);  // 每100毫秒生成一次文字
}

void MainUI::initUI()
{
    // 消息记录控件的切换************************************************
    m_bubbleHistory->addBubble(BubbleParas("question", BUBBLE_ROLE::BR_ME));
    m_bubbleHistory->addBubble(BubbleParas("当然可以！下面是一篇稍长一点的介绍：\n\n人aqwer", BUBBLE_ROLE::BR_AICHAT));

    ui->editChatHistory->show();

    if (m_bBubbleDisplay) { // 高级气泡窗口
        ui->editChatHistory->setParent(nullptr);
        //        ui->editChatHistory->hide();

        ui->splitter->insertWidget(0, m_bubbleHistory);
        m_bubbleHistory->show();
    } else {
        m_bubbleHistory->setParent(nullptr);
        //       m_bubbleHistory->hide();
    }


    m_bubbleHistory->scrollToBottom();

    ui->btnGenerate->setToolTip("按下自动生成回答，松开停止回答");
    ui->btnSend->setToolTip("松开发送消息；直接在输入框按下回车也直接发送消息");
    setWindowTitle("Bubble Chat History");

    ui->splitter->setStretchFactor(0, 9);
    ui->splitter->setStretchFactor(1, 2);
    resize(1200, 800);
}

void MainUI::onGenerateText()
{
    QString characters = "\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    QRandomGenerator generator;
    generator.seed(QRandomGenerator::global()->generate());
    int length = generator.bounded(10, 20); // 生成长度为10到20的随机数
    QString randomText;

    for (int i = 0; i < length; ++i) {
        int index = generator.bounded(characters.length());
        QChar randomChar = characters.at(index);
        randomText.append(randomChar);
    }

//    randomText += "\n";
    m_text = randomText;
    m_bubbleHistory->appendLastBubbleText(BUBBLE_ROLE::BR_AICHAT, m_text);
}
