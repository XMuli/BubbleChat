#ifndef CHATHISTORY_H
#define CHATHISTORY_H

#include <QListWidget>
#include <QDateTime>
#include <QPixmap>
#include "chatbubble.h"

class ChatHistory : public QListWidget {
public:
    ChatHistory(QWidget *parent = nullptr) : QListWidget(parent) {
        resize(1200, 760);
    }

    void addBubble(bool isMyself, const QString &text, const QString &name = "", const QDateTime &time = QDateTime::currentDateTime(), const QPixmap* avatar = nullptr);
    void setLastChatBubbleText(const QString &text, const QDateTime &time = QDateTime::currentDateTime());

private:
    ChatBubble* lastChatBubble();
};

#endif // CHATHISTORY_H
