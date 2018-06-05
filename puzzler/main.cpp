// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#include "puzzlerwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PuzzlerWindow w;
    w.show();

    return a.exec();
}
