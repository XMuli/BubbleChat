

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



##### 【解决方案】

当 Bubble 追加文字时候，此时属于已经显示出来，已可以获取其实际宽和高，然后通过 “尝试的方法二” 来计算里面详细的高度，然后结合 （Bubble - textBrowser） 的高度，**在外面重新设置 QListWidgetItem 的高度**， 最终，完美显示效果
