#include "puzzlerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PuzzlerWindow w;
    w.show();

    return a.exec();
}
