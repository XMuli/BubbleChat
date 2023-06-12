#include "chatbubble.h"
#include "ui_chatbubble.h"

#include <QFontMetrics>
#include <QTextDocument>
#include <QFontMetricsF>

ChatBubble::ChatBubble(Position position, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatBubble)
    , m_position(position)
{
    ui->setupUi(this);

    // Design 中暂定了一个布表示美观，故此处需要先移除一次，再布局
    QLayoutItem *child;
    while ((child = ui->mainLayout->takeAt(0)) != nullptr)
            child->layout()->setParent(nullptr);

    auto& bubble = ui->textBubble;
    QSpacerItem *spacer = new QSpacerItem(52 + 6, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    if (m_position == Left) {
        ui->mainLayout->addLayout(ui->vLayoutAvatar);
        ui->mainLayout->addLayout(ui->layoutMsg);
        // 将弹簧添加到水平布局中
        ui->mainLayout->insertSpacerItem(ui->mainLayout->count(), spacer);
        ui->mainLayout->insertStretch(ui->mainLayout->count());
        ui->labName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->labStatus->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setAvatar(new QPixmap(":/resources/avatar/chatgpt3.5.png"));
        bubble->setStyleSheet("background-color: #CCCCCC; border-radius: 10px;");
    } else {
        ui->mainLayout->insertStretch(0);
        ui->mainLayout->addLayout(ui->layoutMsg);
        ui->mainLayout->addLayout(ui->vLayoutAvatar);
        ui->mainLayout->insertSpacerItem(0, spacer);
        ui->labName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->labStatus->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        setAvatar(new QPixmap(":/resources/avatar/avatar.png"));

        bubble->setStyleSheet("background-color: #CCCCCC; border-radius: 10px;"); // E2F3FF
    }

    bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    bubble->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bubble->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bubble->setFrameStyle(QFrame::NoFrame);
    bubble->setOpenExternalLinks(true); // 支持 HTML 链接
}

ChatBubble::~ChatBubble()
{
    delete ui;
}

// 仅仅初步计算，还不够精细；后续要重新优化下这个控件
void ChatBubble::setText(const QString &text)
{
    auto& bubble = ui->textBubble;
    bubble->setText(text);

    QFontMetricsF fm(this->font());
    int maxWidth = 500;
    const QWidget* parent = this->nativeParentWidget();  // 可以调整
    if (parent) {
        maxWidth = parent->width() - 170;
//        qDebug() << "**********>parent->width() - 170:" << parent->width() - 170;
    }

    int allRowCount = 0;
    int i = 1;
    for (const QString& line : text.split('\n')) {
        int lineWidth = fm.horizontalAdvance(line);
        int realRowCount = static_cast<int>(lineWidth / maxWidth) + 1;
        allRowCount += realRowCount;
//        qDebug() << "i:" << i++ << "  lineWidth:" << lineWidth <<"  realRowCount:" << realRowCount << "  allRowCount:" << allRowCount << "  line:" << line;
    }

    // 单行文字，气泡会短一点
    if (allRowCount == 1) maxWidth = fm.horizontalAdvance(text);
    const double lineHeight = fm.lineSpacing();
    const QMargins margins(15, 7, 15, 7);
    bubble->setFixedSize(maxWidth + margins.left() + margins.right(), lineHeight * allRowCount + margins.top() + margins.bottom());
}

void ChatBubble::setStatus(const QString &text)
{
    ui->labStatus->setText(text);
}

void ChatBubble::setName(const QString &name)
{
    ui->labName->setText(name);
}

void ChatBubble::setAvatar(const QPixmap* pixmap)
{
    if (!pixmap)
        return;

//    qDebug() << "--1" << ui->labAvatar->size() << "  " << pixmap->size();
    ui->labAvatar->setMaximumSize(QSize(36, 36));
    ui->labAvatar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ui->labAvatar->setPixmap(*pixmap);
//    qDebug() << "--2" << ui->labAvatar->size() << "  " << pixmap->size();
}

void ChatBubble::setTime(const QString &time)
{
    ui->labStatus->setText(time);
}

const QString ChatBubble::text()
{
    return ui->textBubble->toPlainText();
}

void ChatBubble::resizeEvent(QResizeEvent *event)
{
    auto& bubble = ui->textBubble;
    setText(bubble->toPlainText());  // 手动刷新

//    qDebug() << "resizeEvent" << bubble->size() << "  " << parentWidget()->size();
    QWidget::resizeEvent(event);
}
