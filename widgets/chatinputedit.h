// SPDX-SnippetCopyrightText: 2023-2025 Wengeng Zhang
// SPDX-Author: Wengeng Zhang <xmulitech@gmail.com>
// SPDX-License-Identifier: MIT
#ifndef CHATINPUTEDIT_H
#define CHATINPUTEDIT_H

#include <QPlainTextEdit>

enum class CustomKey {
    CK_EnterSend,   // 忽略回车，发送消息
    CK_CRLF           // carriage return/line feed 回车换行
};

class ChatInputEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ChatInputEdit(QWidget* parent = nullptr);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void sigCustomKeyBtnReleased(const CustomKey& customKey);
};


#endif // CHATINPUTEDIT_H
