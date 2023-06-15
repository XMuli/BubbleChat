#include "bubble.h"
#include "ui_bubble.h"
#include <QDebug>
#include <QPalette>
#include <QTextLayout>
#include <QTextOption>
#include <QTextDocument>
#include <QScrollBar>

Bubble::Bubble(BUBBLE_ROLE role, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Bubble)
    , m_role(role)

{
    ui->setupUi(this);
    initUI();
}

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

// 可以优化的点，还可以更加细致的计算
void Bubble::setText(const QString &text)
{
    auto& bubble = ui->browerText;
    bubble->setText(text);

//    QFontMetricsF fm(this->font());
//    int maxWidth = bubble->viewport()->width();
//    qDebug() << "**********1>maxWidth:" << maxWidth;
//    const QWidget* parent = this->nativeParentWidget();  // 可以调整
//    if (parent) {
//        maxWidth = parent->width() - 170;
//        qDebug() << "**********2>maxWidth:" << maxWidth;
//    }

//    int allRowCount = 0;
//    int i = 1;
//    for (const QString& line : text.split('\n')) {
//        int lineWidth = fm.horizontalAdvance(line);
//        int realRowCount = static_cast<int>(lineWidth / maxWidth) + 1;
//        allRowCount += realRowCount;
////        qDebug() << "i:" << i++ << "  lineWidth:" << lineWidth <<"  realRowCount:" << realRowCount << "  allRowCount:" << allRowCount << "  line:" << line;
//    }

//    const double lineHeight = fm.lineSpacing();
//    const QMargins margins(0, 5, 0, 5);
////    maxWidth + margins.left() + margins.right(),  宽度
//    bubble->setFixedHeight(lineHeight * allRowCount + margins.top() + margins.bottom());
}

void Bubble::appendText(const QString &text)
{
    ui->browerText->moveCursor(QTextCursor::End); // 将光标移动到文本末尾
    ui->browerText->insertPlainText(text); // 追加字符串，不添加换行符

    setText(ui->browerText->toPlainText());  // 自动适配文字行高， 此处还可以优化
}

void Bubble::setName(const QString &name)
{
    QString t = "";
    if (m_role == BUBBLE_ROLE::BR_AICHAT) {
        t = "AI";
    } else {
        t = name.isEmpty() ? "User" : name;
    }

    ui->labName->setText(t);
}

void Bubble::setPhoto()
{
    auto& labPhoto = ui->labPhoto;/*
    labPhoto->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);*/

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
    return ui->browerText->toPlainText();
}

// 根据当前气泡的角色，自动显示顶部右侧的的多个按钮，显隐状态
void Bubble::autoRoleBtn()
{
    ui->labStatus->setVisible(false);
    ui->browerText->setVisible(true);

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
        ui->browerText->setVisible(false);
    }
}

void Bubble::initUI()
{
    autoRoleBtn();
    auto& browerText = ui->browerText;
    browerText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    browerText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    browerText->setLineWrapMode(QTextEdit::WidgetWidth);

    connect(browerText->document(), &QTextDocument::contentsChanged, [browerText, this](){

        const auto& scrollBar = browerText->verticalScrollBar();
        const auto& text = browerText->document()->toPlainText();

        QFontMetricsF fm(browerText->font());
        int maxWidth  = browerText->viewport()->width();
            qDebug() << "**********1>maxWidth:" << maxWidth;
            const QWidget* parent = this->nativeParentWidget();  // 可以调整
            if (parent) {
                maxWidth = parent->width() - 170;
                qDebug() << "**********2>maxWidth:" << maxWidth;
            }

            int allRowCount = 0;
            int i = 1;
            for (const QString& line : text.split('\n')) {
                int lineWidth = fm.horizontalAdvance(line);
                int realRowCount = static_cast<int>(lineWidth / maxWidth) + 1;
                allRowCount += realRowCount;
        //        qDebug() << "i:" << i++ << "  lineWidth:" << lineWidth <<"  realRowCount:" << realRowCount << "  allRowCount:" << allRowCount << "  line:" << line;
            }

            const double lineHeight = fm.lineSpacing();
//            const QMargins margins(0, 5, 0, 5);
        //    maxWidth + margins.left() + margins.right(),  宽度

        int height = scrollBar->maximum() - scrollBar->minimum() + lineHeight * allRowCount/*scrollBar->pageStep()*/;

        qDebug() << "-----@1---->max:" << scrollBar->maximum() << "  min:" << scrollBar->maximum() << "  pageStep:" << scrollBar->pageStep() << height;
        ui->browerText->setFixedHeight(height);
        qDebug() << "-----@2---->browerText size:" << ui->browerText->size() << ui->browerText->document()->toPlainText();
    });

}

void Bubble::setWidgetBackgroundColor(QWidget *widget, const QColor &color)
{
    if (!widget)
        return;

    widget->setStyleSheet(QString("background-color: %1;").arg(color.name()));
//    QPalette palette = widget->palette();
//    widget->setAutoFillBackground(true);
//    palette.setColor(QPalette::Background, color);
//    widget->setPalette(palette);

    // 递归设置子控件的背景色
    for (auto& childWidget : widget->findChildren<QWidget *>())
        setWidgetBackgroundColor(childWidget, color);
}

BUBBLE_ROLE Bubble::role() const
{
    return m_role;
}
