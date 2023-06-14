#include "bubblehistory.h"
#include <QListWidgetItem>
#include <QDebug>

BubbleHistory::BubbleHistory(QWidget *parent)
    : QListWidget(parent)
{
    initUI();
}

void BubbleHistory::addBubble(const BubbleParas& paras)
{
    Bubble *bubble = new Bubble(paras.text, paras.role, paras.time, this);


    qDebug () << "addBubble（） -> bubble:" << bubble << "  this:" << this;

    QListWidgetItem *item = new QListWidgetItem(this);
    item->setSizeHint(bubble->sizeHint());
    addItem(item);
    setItemWidget(item, bubble);

    QListWidget::updateGeometry();
    scrollToBottom();
}

void BubbleHistory::setLastBubbleText(const QString &text, const QDateTime &time)
{
    Bubble* bubble = lastBubble();
    if (!bubble) return;

    bubble->setText(text); // update text
}

void BubbleHistory::appendLastBubbleText(const QString &text, const QDateTime &time)
{
    Bubble* bubble = lastBubble();
    if (!bubble) return;

    if (bubble->role() == BUBBLE_ROLE::BR_ME) {
        // 不存在则先创建 BR_AICHAT 的气泡
        BubbleParas bubbleParas(text, BUBBLE_ROLE::BR_AICHAT);
        addBubble(bubbleParas);
    } else if (bubble->role() == BUBBLE_ROLE::BR_AICHAT) {
        bubble->appendText(text); // update text
    }

    QListWidget::updateGeometry();
    scrollToBottom();
}

Bubble *BubbleHistory::lastBubble()
{
    // 获取最后一个 QListWidgetItem
    QListWidgetItem* lastItem = item(this->count() - 1);

    // 将QListWidgetItem转换为ChatBubble*
    Bubble* bubble = qobject_cast<Bubble *>(this->itemWidget(lastItem));

    qDebug() << "bubble:" << bubble << "text:" << bubble->text() << "  count():" << count();
    return bubble;
}

void BubbleHistory::initUI()
{
    // 设置样式表禁用悬浮高亮效果
//    setStyleSheet("QListWidget::item:hover { background-color: transparent; }");
//    resize(1200, 760);
    setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
}
