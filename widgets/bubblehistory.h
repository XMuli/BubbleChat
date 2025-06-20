﻿// SPDX-SnippetCopyrightText: 2023-2025 Wengeng Zhang
// SPDX-Author: Wengeng Zhang <xmulitech@gmail.com>
// SPDX-License-Identifier: MIT
#ifndef BUBBLEHISTORY_H
#define BUBBLEHISTORY_H

#include <QListWidget>
#include <QDateTime>
#include <QString>
#include <QBoxLayout>
#include "bubble.h"


class BubbleHistory : public QListWidget
{
    Q_OBJECT
public:
    BubbleHistory(QWidget *parent = nullptr);

    void addBubble(const BubbleParas &paras);
    void appendLastBubbleText(const BUBBLE_ROLE role, const QString &text, const QDateTime &time = QDateTime::currentDateTime());

//private slots:
//    void onBubbleHeightChanged(const int height);

private:

    void initUI();
    Bubble* lastBubble(BUBBLE_ROLE role);  // 指定最后一个 ME， 或者最后一个 AIChat
    QListWidgetItem* lastListItem(BUBBLE_ROLE role);
    const int lastBubbleHeight(BUBBLE_ROLE role, int& itemHeight, int& browserHeight);
};

#endif // BUBBLEHISTORY_H
