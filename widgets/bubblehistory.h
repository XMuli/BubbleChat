#ifndef BUBBLEHISTORY_H
#define BUBBLEHISTORY_H

#include <QListWidget>
#include <QDateTime>
#include <QString>
#include "bubble.h"

class BubbleHistory : public QListWidget
{
    Q_OBJECT
public:
    BubbleHistory(QWidget *parent = nullptr);

    void addBubble(const BubbleParas &paras);
    void setLastBubbleText(const QString &text, const QDateTime &time = QDateTime::currentDateTime());
    void appendLastBubbleText(const QString &text, const QDateTime &time = QDateTime::currentDateTime());

private:
    Bubble* lastBubble();
    void initUI();
};

#endif // BUBBLEHISTORY_H
