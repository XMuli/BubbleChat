#include "bubble.h"
#include "ui_bubble.h"
#include <QDebug>
#include <QPalette>
#include <QTextLayout>
#include <QTextOption>
#include <QTextDocument>
#include <QScrollBar>
#include <QFontMetricsF>
#include <QFontMetricsF>

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
        ui->tbBot->setVisible(false);
        ui->tbLove->setVisible(false);
//        setWidgetBackgroundColor(this, QColor("#e9e9e8"));
    } else if (m_role == BUBBLE_ROLE::BR_AICHAT) {
        ui->tbEditor->setVisible(false);
        ui->tbBot->setVisible(true);
        ui->tbLove->setVisible(true);
    } else if (m_role == BUBBLE_ROLE::BR_STATUS) {
        ui->tbEditor->setVisible(false);
        ui->tbBot->setVisible(false);
        ui->tbLove->setVisible(false);

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

#if 1
        // [最终解决方法] 此时 AI 和 ME Bubble 已经显示出来了，通过已知道的 长度通过 文字的行和高度来重新计算实际高度
        const auto& textBrowser = ui->textBrowser;
        const QString& text = textBrowser->toPlainText();
        int allRowCount = 0;
        int i = 1;

        QFontMetrics fm(textBrowser->font());
        int maxWidth = textBrowser->size().width();

        for (const QString& line : text.split('\n')) {
            int lineWidth = fm.horizontalAdvance(line);
            int realRowCount = static_cast<int>(lineWidth / maxWidth) + 1;
            allRowCount += realRowCount;
            qDebug() << "i:" << i++ << "  lineWidth:" << lineWidth <<"  realRowCount:" << realRowCount << "  allRowCount:" << allRowCount << "  line:" << line;
        }

        if (text.isEmpty()) allRowCount = 0;  // 文本为空是 0 行
        int height = allRowCount * fm.lineSpacing();
#else

        // [其它方法] 通过滚动条来计算还是会有问题的，数值会略偏小
        const auto& scrollBar = ui->textBrowser->verticalScrollBar();
        int height = scrollBar->maximum() - scrollBar->minimum() + scrollBar->pageStep();

        QFontMetrics fm(ui->textBrowser->font());

        qDebug() << "max:" << scrollBar->maximum() << "mix:" << scrollBar->minimum() << "pageStep:" << scrollBar->pageStep()
                 << "height:" << height << "fm.lineSpacing():" << fm.lineSpacing();

        const auto& textBrowser = ui->textBrowser;
        qDebug() << ":" << textBrowser->contentsMargins() << textBrowser->document()->documentMargin();

        // 不能在 Bubble 里面修改高度，必须得在外面修改设置 QListWidgetItem 得高度; 解开注释可查看压缩得效果
//        ui->textBrowser->setFixedHeight(height);
//        setFixedHeight(50 + height);

#endif

        const auto h1 = textBrowser->contentsMargins().top() * 2;
        const auto h2 = textBrowser->document()->documentMargin() * 2;
        emit sigChangedHeight(height + h1 + h2);  // 记得加上间隔
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

// 初次创建 Bubble对象时，一次性给予所有的文本，其高度给 items 会不对，这里重新计算一下
const int Bubble::height() const
{
    const auto& margin = ui->gridLayout->contentsMargins();
    const int height = ui->textBrowser->size().height() + ui->labPhoto->size().height()
                       + margin.top() + margin.bottom() + ui->gridLayout->verticalSpacing();

//    qDebug() << "textBrowser:" << ui->textBrowser->size().height() <<  "labPhoto:" << ui->labPhoto->size().height();
//    qDebug() << "margin:" << margin <<  "verticalSpacing:" << ui->gridLayout->verticalSpacing();
    return height;
}

const int Bubble::textBrowserheight() const
{
    return ui->textBrowser->size().height();
}

void Bubble::printfInfo() const
{
    qDebug() << "textBrowser-> size():" << ui->textBrowser->size() <<  "size():" << ui->textBrowser->sizeHint();
    qDebug() << "this Bubble-> size():" << this->size() <<  "sizeHint():" << this->sizeHint();
    qDebug() << "labPhoto -> size():" << ui->labPhoto->size() <<  "sizeHint():" << ui->labPhoto->sizeHint();
    qDebug() << "labName -> size():" << ui->labName->size() <<  "sizeHint():" << ui->labName->sizeHint();
    qDebug() << "labStatus -> size():" << ui->labStatus->size() <<  "sizeHint():" << ui->labStatus->sizeHint();
    qDebug() << "tbEditor -> size():" << ui->tbEditor->size() <<  "sizeHint():" << ui->tbEditor->sizeHint();

}
