#include "bubblehistory.h"
#include <QListWidgetItem>
#include <QTimer>
#include <QDebug>

BubbleHistory::BubbleHistory(QWidget *parent)
    : QListWidget(parent)
{
    initUI();
}

// 创建一个文本为空的 气泡控件，先显示出来，然后再使用追加文字的方式计算宽和高度
void BubbleHistory::addBubble(const BubbleParas& paras)
{
    Bubble *bubble = new Bubble("", paras.role, paras.time, this);
    QListWidgetItem *item = new QListWidgetItem(this);
    addItem(item);

    const int& bubbleHeight = paras.role ==BUBBLE_ROLE::BR_ME ? bubble->height() : bubble->size().height();
//    const QSize size(bubble->size().width(),  bubbleHeight);
    item->setSizeHint(QSize(bubble->size().width(), bubble->size().height()));  // 初次添加进来得时候，需要给定一个大小，此处不能参考下面赋值 item->sizeHint()，一样会有问题
    setItemWidget(item, bubble);
//    bubble->show();

    connect(bubble, &Bubble::sigChangedHeight, [=](int height){
        // 拉升整体窗口大小时候，效果会更好 比起此处使用 QSize(bubble->width(), height + 60) 赋值; 60 为头像等其 Bubble 里面其它的控件的高度，如何计算出来？

        int itemHeight;
        int browserHeight;
        const auto& role = bubble->role();
        int ResidualHeight = lastBubbleHeight(role, itemHeight, browserHeight);
        qDebug() << "change height:" << height << bubble->textBrowserheight() << bubble->size() << "bubble->height():" << bubble->height();
        QListWidgetItem * lastItem = lastListItem(role);

        qDebug() << "--@2->" <<itemHeight << browserHeight << ResidualHeight << bubble->isVisible();
        if (lastItem) lastItem->setSizeHint(QSize(lastItem->sizeHint().width(), height + ResidualHeight));  // 通过 UI Design 计算和 直接相减，理论都应该是 48， 莫非是 48 * 此屏幕的缩放比？？
    });

    if (!paras.text.isEmpty()) // fix: Me 气泡创建时候，先创建一个空白气泡，此时立即 追加文字 时刻，此刻此 item 还没有显示，故计算是错误的
        QTimer::singleShot(10, [bubble, paras]() { bubble->appendText(paras.text); });


    qDebug () << "addBubble() 结束位置打印 -> bubble:" << bubble << "  this:" << this << "  bubble->size():" << bubble->size()
             << "paras.text:" << paras.text << "isVisible:" << isVisible();
}

void BubbleHistory::appendLastBubbleText(const BUBBLE_ROLE role, const QString &text, const QDateTime &time)
{
    // 返回已存在的最后一个，不管是 ME 还是 AIChat 均返回
    QListWidgetItem* lastItem = item(this->count() - 1);
    Bubble* bubble = qobject_cast<Bubble *>(this->itemWidget(lastItem));
    if (!bubble) return;

    qDebug () << "appendLastBubbleText() 开始位置打印 -> bubble:" << bubble << "  this:" << this << "  bubble->size():" << bubble->size()
             << "text:" << text << "isVisible:" << isVisible();

    if (bubble->role() == BUBBLE_ROLE::BR_ME) {  // 不存在则先创建 BR_AICHAT 的气泡
        BubbleParas bubbleParas(text, BUBBLE_ROLE::BR_AICHAT);
        addBubble(bubbleParas);
    } else if (bubble->role() == BUBBLE_ROLE::BR_AICHAT) {
        bubble->appendText(text); // update text
    }

    scrollToBottom();
}

Bubble *BubbleHistory::lastBubble(BUBBLE_ROLE role)
{
    QListWidgetItem* item = lastListItem(role);
    return item ? qobject_cast<Bubble *>(itemWidget(item)) : nullptr;
}

const int BubbleHistory::lastBubbleHeight(BUBBLE_ROLE role, int &itemHeight, int &browserHeight)
{
    const auto& bubble = lastBubble(role);
    if (!bubble) return 0;

    itemHeight = bubble->size().height();
    browserHeight = bubble->textBrowserheight();
    qDebug() << "--@1->role:" << int(role) << bubble->size() << browserHeight << itemHeight - browserHeight << bubble->isVisible();
    return itemHeight - browserHeight;
}


QListWidgetItem *BubbleHistory::lastListItem(BUBBLE_ROLE role)
{
    for (int i = this->count() - 1; i >= 0 ; --i) {
        QListWidgetItem* lastItem = item(i);
        Bubble* bubble = qobject_cast<Bubble *>(this->itemWidget(lastItem));
        if (!bubble) return nullptr;

        if (bubble->role() == role) {
            qDebug() << "lastListItem:" << bubble << "i:" << i << "text:" << "  count():" << count() << bubble->text();
            return lastItem;
        } else {
            continue;
        }
    }

    qDebug() << "lastListItem is nullptr";
    return nullptr;
}

void BubbleHistory::initUI()
{
    // 设置样式表禁用悬浮高亮效果
    //    setStyleSheet("QListWidget::item:hover { background-color: transparent; }");
    resize(1200, 760);
}
