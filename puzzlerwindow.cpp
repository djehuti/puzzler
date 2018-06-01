#include "puzzlerwindow.h"
#include "ui_puzzlerwindow.h"

#include <QMessageBox>

PuzzlerWindow::PuzzlerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PuzzlerWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(solvePuzzle()));
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(solvePuzzle()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
}

PuzzlerWindow::~PuzzlerWindow()
{
    delete ui;
}

void PuzzlerWindow::showAboutBox()
{
    QMessageBox messageBox;
    //messageBox.setIconPixmap(QPixmap(":/icon.png"));
    messageBox.setText("Twice Removed Puzzle Solver");
    messageBox.setWindowTitle("About");
    messageBox.exec();
}

void PuzzlerWindow::solvePuzzle()
{
    bool textEmpty = ui->lineEdit->text().length() == 0;
    if (textEmpty) {
        QApplication::beep();
        return;
    }

    // TODO(bcox): Write this!
}

void PuzzlerWindow::on_lineEdit_textChanged(const QString &text)
{
    bool textEmpty = text.length() == 0;
    ui->solveButton->setEnabled(!textEmpty);
}
