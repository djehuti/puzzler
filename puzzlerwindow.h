#ifndef PUZZLERWINDOW_H
#define PUZZLERWINDOW_H

#include <QMainWindow>

namespace Ui {
class PuzzlerWindow;
}

class PuzzlerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PuzzlerWindow(QWidget *parent = 0);
    ~PuzzlerWindow();

private slots:
    void showAboutBox();
    void solvePuzzle();
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::PuzzlerWindow *ui;
};

#endif // PUZZLERWINDOW_H
