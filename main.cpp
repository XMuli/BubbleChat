// SPDX-SnippetCopyrightText: 2023-2025 Wengeng Zhang
// SPDX-Author: Wengeng Zhang <xmulitech@gmail.com>
// SPDX-License-Identifier: MIT
#include <QApplication>
#include <QFont>
#include "mainui.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Microsoft YaHei", 9);
    a.setFont(font);

    MainUI w;
    w.show();

    return a.exec();
}
