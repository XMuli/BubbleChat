#ifndef BUBBLE_H
#define BUBBLE_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class Bubble;
}

enum class BUBBLE_ROLE {
    BR_ME,
    BR_AICHAT,
    BR_STATUS
};

struct BubbleParas
{
    BubbleParas(const QString &tText, BUBBLE_ROLE tRole, const QDateTime &tTime = QDateTime::currentDateTime())
        : text(tText)
        , time(tTime)
        , role(tRole)
    {}

    QString text;
    QDateTime time;
    BUBBLE_ROLE role;
};

class Bubble : public QWidget
{
    Q_OBJECT

public:
    explicit Bubble(BUBBLE_ROLE role = BUBBLE_ROLE::BR_AICHAT, QWidget *parent = nullptr);
    explicit Bubble(const QString &text, BUBBLE_ROLE role = BUBBLE_ROLE::BR_AICHAT
                    , const QDateTime &time = QDateTime::currentDateTime(), QWidget *parent = nullptr);
    virtual ~Bubble();

    void setText(const QString &text);
    void appendText(const QString &text);
    void setName(const QString &name = "");
    void setTime(const QDateTime &time);

    const QString text();
    BUBBLE_ROLE role() const;

private:
    void setPhoto();
    void autoRoleBtn();
    void initUI();
    void setWidgetBackgroundColor(QWidget *widget, const QColor &color);

private:
    Ui::Bubble *ui;
    BUBBLE_ROLE m_role;
};

#endif // BUBBLE_H
