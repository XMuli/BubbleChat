## Bubble ChatChat

A control for bubble chat window messages created using QListWidget and adapts to the size of the bubble.

Adaptive Size
- Bubble messages append text content and the height is automatically recalculated
- Stretch to change the size of the main window, the width and height are automatically recalculated



使用 QListWidget 创建的一个 气泡聊天窗口消息的控件，并且可以自适应气泡的大小。

自适应大小
- 气泡消息追加文本内容，高度会自动重新计算
- 拉伸改变主窗口的大小，宽度和高度会自动重新计算



## 演示效果

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/demonstrate.gif" width="100%"/>





##### 【本质问题】

如何通计算去掉那个魔数字，优雅和完美显示；


   - 在那个 魔数 在 BubbleHistory::addBubble 里添加，或 Bubble::initUI()  里面添加；这两个本质没有任何区别
   - connect(ui->textBrowser->document(), &QTextDocument::contentsChanged 改为后者了
      connect(ui->textBrowser, &QTextBrowser::textChanged =》 属于初次赋值也可获取到



##### 【已有三种尝试】

​     **背景**：- 在 textBrowser/Bubble 还没显示时候，就需要够获取高度；计算高度的三种方思路
​	 

1. 通过滚动条来获取；但是一行的数值应该为 24 左右，而这里实际为 10，所需要添加一个魔术

   ```cpp
   const auto& scrollBar = ui->textBrowser->verticalScrollBar();
   int height = scrollBar->maximum() - scrollBar->minimum() + scrollBar->pageStep();
   ```



2. 通过此时已知道文本的 text ， 通过 QFontMetrics 来获取 总的字符串的宽度（长），除以 一行能够显示的长度（未知，痛点无法知道），进而得到行数，再乘以  fm.lineSpacing() 获取真实的高度。**如果此时此控件已经显示出来，此则是最好的显示方案**

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
   
   int height = allRowCount * fm.lineSpacing();  // 最佳的实际行高
   
   // 若是 QTextBrower 之类，记得加上 margins 这中的间隔
   // const auto h1 = textBrowser->contentsMargins().top() * 2;
   // const auto h2 = textBrowser->document()->documentMargin() * 2;
   // height = height + h1 + h2；
   ```



3. 通过 textBrowser->document()->size() 作为实际的高度，这里面的 document->size()实际也是偏小，还一开始是 0；

   ```cpp
   ui->textBrowser->setFixedHeight(document->size().height()+20);
   emit sigChangedHeight(ui->textBrowser->height()+56);
   ```
   
   
   
4. 记得有一个函数，是可以刷新出尺寸，但是窗体不显示；实现起来更加优雅；函数名忘记没找到

   



##### 【解决方案】

当 Bubble 追加文字时候，此时属于已经显示出来，已可以获取其实际宽和高，然后通过 “尝试的方法二” 来计算里面详细的高度，然后结合 （Bubble - textBrowser） 的高度，**在外面重新设置 QListWidgetItem 的高度**， 最终，完美显示效果



<br>

## 贡献

若是帮助到了你，或者觉得有用，<font color=#FE7207  size=4 face="幼圆">可以点击该项目的的 <font color=#D0087E size=4 face="幼圆">**⭐Star** </font>和<font color=#D0087E size=4 face="幼圆">**🍴 Fork**</font> 的两个图标，方便抬手之间，表示点个赞，手有余香，</font>其次才是一份冰的肥宅快乐水。

<details>
    <summary> 当然也可以赠与一杯冰阔落[捐赠/打赏  ← 点击展开二维码]</summary>
  <p> - 若是此项目帮助到了你，或者觉得有用，或是想帮助此项目的发展，你也能够邀请我喝一杯杯肥仔快乐水。 - </p>
  <pre><img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202302282339037.png" width="80%"/></pre>
</details>

<br>

## 系列

[QtExamples](https://github.com/XMuli/QtExamples)     欢迎 `star` ⭐ 和 `fork` 🍴这个系列的 `C++ / QT / DTK` 学习，附学习由浅入深的目录
