<div align="center">
  <p>
    <br/>
    <h1>BubbleChat</h1>
    <h4>A bubble chat message display that is adaptive in size</h4>
    <h4>气泡聊天的消息展示的效果，且自适应大小</h4>
  </p>
</div>
<div align="center">
  <p align="right"><br><a href="https://thinkymate.xmuli.tech/">English</a> | <a href="https://github.com/XMuli/BubbleChat/blob/master/README.zh_CN.md">简体中文</a></p>
</div>


> **This item is included in**  [Product](https://product.xmuli.tech/)




## Bubble ChatChat

A control for bubble chat window messages created using QListWidget and adapts to the size of the bubble.

Adaptive Size
- Bubble messages append text content and the height is automatically recalculated
- Stretch to change the size of the main window, the width and height are automatically recalculated



A bubble chat window message control created with QListWidget and with the ability to adapt the size of the bubbles.

Adaptive size

- Add text to the bubble message, the height will be automatically recalculated.
- Stretch to change the size of the main window, the width and height will be automatically recalculated.



## Demonstration effect

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/demonstrate.gif" width="100%"/>





##### [Fundamental Issue]

How to eliminate that magic number through calculation for an elegant and perfect display;

- Whether adding that magic number in BubbleHistory::addBubble or in Bubble::initUI(), essentially there is no difference between the two.
- Changed the connection from
   `connect(ui->textBrowser->document(), &QTextDocument::contentsChanged`
   to
   `connect(ui->textBrowser, &QTextBrowser::textChanged`
   so that even the initial assignment can be captured.

##### [Existing Three Approaches]

**Background**: Before the textBrowser/Bubble is displayed, we need to obtain its height; there are three approaches to calculating the height.

1. Obtain it through the scrollbar; however, a single line’s value should be around 24, but here it is actually 10, thus requiring a magic number adjustment.

   ```cpp
   const auto& scrollBar = ui->textBrowser->verticalScrollBar();
   int height = scrollBar->maximum() - scrollBar->minimum() + scrollBar->pageStep();
   ```

2. Given that the text is already known at this point, we can use QFontMetrics to obtain the total width of the string, divide it by the width that can be displayed in a single line (which is unknown—a pain point), thereby determining the number of lines, and then multiply by fm.lineSpacing() to get the actual height. **If the control is already displayed at that point, this is the best display solution.**

   ```cpp
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
   
   int height = allRowCount * fm.lineSpacing();  // Optimum practical row height
   
   // If it's a QTextBrower or something like that, remember to add the margins.
   // const auto h1 = textBrowser->contentsMargins().top() * 2;
   // const auto h2 = textBrowser->document()->documentMargin() * 2;
   // height = height + h1 + h2；
   ```



3. With textBrowser->document()->size() as the actual height, the document->size() in this is actually on the small side as well, and also starts out at 0;

   ```cpp
   ui->textBrowser->setFixedHeight(document->size().height()+20);
   emit sigChangedHeight(ui->textBrowser->height()+56);
   ```
   
   
   
4. Remember that there is a function that can refresh the size, but the form does not display; the implementation of a more elegant; function name forgotten not found

   



##### [Solution]

When Bubble append text, at this time belongs to the already displayed, you can get its actual width and height, and then through the “tried and true method two” to calculate the height of the inside details, and then combined with the height of the (Bubble - textBrowser), ** reset the height of the outside of the QListWidgetItem's height **, finally, perfect display effect!



## Who uses this program?

> [http://thinkymate.xmuli.tech/](http://thinkymate.xmuli.tech/)



<div align="center">
  <p>
      <h1>
      <a href="https://thinkymate.xmuli.tech">
          <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/202312180312618.png" width="100%" alt="ThinkyMate"/>
      </a>
    </h1>
    <br/>
    <h4>Simple and easy to use desktop application for ChatGPT & AI</h4>
    <h4>简洁且易用的 ChatGPT & AI 的桌面应用程序</h4>
    <h4>簡潔且易用的 ChatGPT & AI 的桌面應用程序</h4>
  </p>
</div>
<div align="center">
  <p align="right"><br><a href="https://thinkymate.xmuli.tech/">English</a> | <a href="https://github.com/XMuli/ThinkyMate/blob/master/docs/index.zh_CN.md">简体中文</a></p>
</div>




<br>

## Contribution

If this has helped you or you find it useful, please consider clicking the **⭐ Star** and **🍴 Fork** icons of this project to show your support with a simple gesture. It’s a small way to leave a positive impression—and then, why not enjoy an iced can of your favorite soda?




<br>

## Tutorial Series

[QtExamples](https://github.com/XMuli/QtExamples)     Welcome `star` ⭐ and `fork` 🍴 to this series of `C++ / QT / DTK` studies, with a catalog of studies from the beginning to the end!
