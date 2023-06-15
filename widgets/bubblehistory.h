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
    void appendLastBubbleText(const QString &text, const QDateTime &time = QDateTime::currentDateTime());

//private slots:
//    void onBubbleHeightChanged(const int height);

private:

    void initUI();
    Bubble* lastBubble();
    Bubble* lastBubbleAI();
    QListWidgetItem* lastListItemAI();
};

#endif // BUBBLEHISTORY_H
