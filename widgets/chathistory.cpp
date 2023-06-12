#include "chathistory.h"

void ChatHistory::addBubble(bool isMyself, const QString &text, const QString &name, const QDateTime &time, const QPixmap *avatar)
{
    ChatBubble *bubble = new ChatBubble(isMyself ? ChatBubble::Right : ChatBubble::Left, this);

    qDebug () << "bubble:" << bubble << "  this:" << this;
    if (name.isEmpty()) bubble->setName(isMyself ? "Venn" : "ChatGPT");
    bubble->setTime(time.toString("yyyy-MM-dd hh:mm:ss"));
    bubble->setText(text);
    bubble->setAvatar(avatar);
    QListWidgetItem *item = new QListWidgetItem(this);
    item->setSizeHint(bubble->sizeHint());
    addItem(item);
    setItemWidget(item, bubble);

    lastChatBubble();
}

void ChatHistory::setLastChatBubbleText(const QString &text, const QDateTime &time)
{
    Q_UNUSED(time);

    ChatBubble* chatBubble = lastChatBubble();
    if (!chatBubble) return;

    chatBubble->setText(text); // update text
}

ChatBubble *ChatHistory::lastChatBubble()
{
    // 获取最后一个 QListWidgetItem
    QListWidgetItem* lastItem = item(this->count() - 1);

    // 将QListWidgetItem转换为ChatBubble*
    ChatBubble* chatBubble = qobject_cast<ChatBubble *>(this->itemWidget(lastItem));

    qDebug() << "chatBubble:" << chatBubble << "text:" << chatBubble->text() << "  count():" << count();
    return chatBubble;
}
