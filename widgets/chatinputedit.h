#ifndef CHATINPUTEDIT_H
#define CHATINPUTEDIT_H

#include <QPlainTextEdit>

class ChatInputEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ChatInputEdit(QWidget* parent = nullptr);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void sigSendBtnReleased();
};


#endif // CHATINPUTEDIT_H
