#include "chatinputedit.h"

#include <QDebug>

ChatInputEdit::ChatInputEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{

}

void ChatInputEdit::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        event->ignore(); // 屏蔽 enter/return 按键

        emit sigSendBtnReleased();
//        qDebug() << "ChatInputPlainTextEdit: emit sigSendBtnReleased()!";
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}
