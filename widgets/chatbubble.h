#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QTextBrowser>
#include <QtWidgets>

namespace Ui {
class ChatBubble;
}

class ChatBubble : public QWidget
{
    Q_OBJECT

public:
    enum Position {
        Left,
        Right
    };

    ChatBubble(Position position = Left, QWidget *parent = nullptr);
    ~ChatBubble();

    void setText(const QString &text);
    void setStatus(const QString &text);
    void setName(const QString &name);
    void setAvatar(const QPixmap *pixmap);
    void setTime(const QString &time);
    const QString text();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ChatBubble *ui;
    Position m_position;
};



#endif // CHATBUBBLE_H
