#include "bubble.h"
#include "ui_bubble.h"
#include <QDebug>
#include <QPalette>
#include <QTextLayout>
#include <QTextOption>
#include <QTextDocument>
#include <QScrollBar>

Bubble::Bubble(const QString &text, BUBBLE_ROLE role, const QDateTime &time, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Bubble)
    , m_role(role)
{
    ui->setupUi(this);
    initUI();
    setText(text);
    setName();
    setPhoto();
    setTime(time);
}

Bubble::~Bubble()
{
    delete ui;
}

void Bubble::setText(const QString &text)
{
    ui->textBrowser->setText(text);
}

/***********************************************************************************************************
ui->textBrowser->insertPlainText(text) 追加字符串，不添加换行符; 其高度会触发 ui->textBrowser->document()
的 QTextDocument::contentsChanged 信号，然后自动将实际 textBrowser 的高度通过信号发射出去，在外面通过设置其承载
的 “QListWidgetItem 高度 = textBrowser 高度 + 其它控件布局的高度”。

即可以自然而然的得到预期的效果 --->  textBrowser 随着 text 的增加而高度变高，从而使得整个 Bubble 也变高
************************************************************************************************************/
void Bubble::appendText(const QString &text)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // 将光标移动到文本末尾
    ui->textBrowser->insertPlainText(text);        // 追加字符串，不添加换行符
}

void Bubble::setName(const QString &name)
{
    QString t = "";
    if (m_role == BUBBLE_ROLE::BR_AICHAT) {
        t = "AI";
    } else {
        t = name.isEmpty() ? "Venn" : name;
    }

    ui->labName->setText(t);
}

void Bubble::setPhoto()
{
    auto& labPhoto = ui->labPhoto;
    const QString& path = m_role == BUBBLE_ROLE::BR_AICHAT ? ":/resources/avatar/openai.svg" : ":/resources/avatar/avatar.png";
    const QPixmap *pixmap = new QPixmap(path);
    labPhoto->setPixmap(*pixmap);
}

void Bubble::setTime(const QDateTime &time)
{
    if (m_role != BUBBLE_ROLE::BR_STATUS)
        ui->labStatus->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}

const QString Bubble::text()
{
    return ui->textBrowser->toPlainText();
}

// 根据当前气泡的角色，自动显示顶部右侧的的多个按钮，显隐状态
void Bubble::autoRoleBtn()
{
    ui->labStatus->setVisible(false);
    ui->textBrowser->setVisible(true);

    if (m_role == BUBBLE_ROLE::BR_ME) {
        ui->tbEditor->setVisible(true);
        ui->tbStopGenerate->setVisible(false);
        ui->tbPlayAudio->setVisible(false);
//        setWidgetBackgroundColor(this, QColor("#e9e9e8"));
    } else if (m_role == BUBBLE_ROLE::BR_AICHAT) {
        ui->tbEditor->setVisible(false);
        ui->tbStopGenerate->setVisible(true);
        ui->tbPlayAudio->setVisible(true);
    } else if (m_role == BUBBLE_ROLE::BR_STATUS) {
        ui->tbEditor->setVisible(false);
        ui->tbStopGenerate->setVisible(false);
        ui->tbPlayAudio->setVisible(false);

        ui->labStatus->setVisible(true);
        ui->textBrowser->setVisible(false);
    }
}

void Bubble::initUI()
{
    autoRoleBtn();
    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 只是不显示，而实际也是存在的，高度得自行计算
    ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 将文本自动换行以适应QTextBrowser的宽度。当文本的宽度超过控件的宽度时，会自动将文本进行换行显示，以确保文本内容在控件内部完全可见。
    ui->textBrowser->setLineWrapMode(QTextEdit::WidgetWidth);
    connect(ui->textBrowser->document(), &QTextDocument::contentsChanged, [=](){
        const auto& scrollBar = ui->textBrowser->verticalScrollBar();
        int height = scrollBar->maximum() - scrollBar->minimum() + scrollBar->pageStep();

        // 不能在 Bubble 里面修改高度，必须得在外面修改设置 QListWidgetItem 得高度; 解开注释可查看压缩得效果
//        ui->textBrowser->setFixedHeight(height);
//        setFixedHeight(50 + height);
        emit sigChangedHeight(height);
    });

}

void Bubble::setWidgetBackgroundColor(QWidget *widget, const QColor &color)
{
    if (!widget)
        return;

#if 1
    widget->setStyleSheet(QString("background-color: %1;").arg(color.name()));
#else
//    QPalette palette = widget->palette();
//    widget->setAutoFillBackground(true);
//    palette.setColor(QPalette::Background, color);
//    widget->setPalette(palette);
#endif

    // 递归设置子控件的背景色
    for (auto& childWidget : widget->findChildren<QWidget *>())
        setWidgetBackgroundColor(childWidget, color);
}

BUBBLE_ROLE Bubble::role() const
{
    return m_role;
}
