// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#include "puzzlerwindow.h"
#include "puzzler/ui_puzzlerwindow.h"

#include <QtWidgets/QMessageBox>

namespace {

const char *DICT_PATH = "/usr/share/dict/words";

}  // namespace

PuzzlerWindow::PuzzlerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PuzzlerWindow),
    dictionary_(DICT_PATH),
    dictCount_(0),
    dictReady_(false),
    generator_(nullptr),
    expectedCandidateCount_(0),
    candidateCount_(0),
    generatorProgress_(nullptr)
{
    ui->setupUi(this);
    QObject::connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(solvePuzzle()));
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(solvePuzzle()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));

    ui->tableView->setModel(&solutions_);

    // We load the dictionary on a separate thread. It will signal us when it's ready.
    ui->statusBar->showMessage("Loading dictionary...");
    dictionary_.moveToThread(&workerThread_);
    connect(this, SIGNAL(loadDictionary()), &dictionary_, SLOT(load()));
    connect(&dictionary_, SIGNAL(loadProgress(int)), this, SLOT(dictProgress(int)));
    connect(&dictionary_, SIGNAL(finished(bool)), this, SLOT(dictLoaded(bool)));
    // Tell the dictionary to start loading.
    emit loadDictionary();
    workerThread_.start();
}

PuzzlerWindow::~PuzzlerWindow()
{
    workerThread_.quit();
    workerThread_.wait();
    delete ui;
}

void PuzzlerWindow::showAboutBox()
{
    QMessageBox messageBox;
    messageBox.setTextFormat(Qt::RichText);
    messageBox.setText(
        "<p><strong>Twice Removed Puzzle Solver</strong></p>"
        "<p>Copyright (c) 2018 Ben Cox.</p>");
    messageBox.setInformativeText(
        "See the source repository at <a href=\"https://github.com/djehuti/puzzler\">GitHub</a> "
        "for source code, and the file LICENSE in that repository for licensing information."
    );
    messageBox.setWindowTitle("About");
    messageBox.exec();
}

void PuzzlerWindow::solvePuzzle()
{
    // We disable the solve button when we're not ready, but if they manage
    // to invoke the operation before we're ready, just beep.
    QString puzzleWord = ui->lineEdit->text();
    if (puzzleWord.length() == 0 || !dictReady_ || generator_ != nullptr) {
        QApplication::beep();
        return;
    }

    // Wipe out previous solutions that might still be shown.
    solutions_.removeRows(0, solutions_.rowCount());
    ui->statusBar->clearMessage();

    // (Re)Initialize the progress bar.
    expectedCandidateCount_ = candidateCount_ = 0;
    updateGeneratorProgress();
    // And show it in the status bar.
    ui->statusBar->addWidget(&generatorProgress_, 1);
    generatorProgress_.show();

    // Create a solver and invoke it.
    generator_ = new WordGenerator(this, puzzleWord);
    connect(generator_, SIGNAL(expectedCandidateCount(int)), this, SLOT(gotCandidateCount(int)));
    connect(generator_, SIGNAL(candidateGenerated(const QString &)),
            this, SLOT(candidateGenerated(const QString &)));
    connect(generator_, SIGNAL(finished()), this, SLOT(noMoreCandidates()));
    connect(generator_, SIGNAL(finished()), generator_, SLOT(deleteLater()));
    enableUi();
    generator_->start();
}

void PuzzlerWindow::on_lineEdit_textChanged(const QString &text)
{
    Q_UNUSED(text);
    enableUi();
}

void PuzzlerWindow::dictProgress(int numWords)
{
    // Update the dictionary-loading message in the status bar.
    dictCount_ = numWords;
    QString message("Loading (");
    message.append(QString::number(numWords));
    message.append(")...");
    ui->statusBar->showMessage(message);
}

void PuzzlerWindow::dictLoaded(bool success)
{
    if (success) {
        // We successfully loaded the dictionary. Display a message and enable the UI.
        QString message("Loaded ");
        message.append(QString::number(dictCount_));
        message.append(" words from ");
        message.append(DICT_PATH);
        message.append(".");
        // The message will disappear after 10 seconds unless cleared another way.
        ui->statusBar->showMessage(message, 10000);
        dictReady_ = true;
    } else {
        // Oh no! We couldn't load the dictionary. (The UI will not be enabled, and the
        // user will just have to quit.)
        ui->statusBar->showMessage("Dictionary could not be loaded!");
    }
    enableUi();
}

void PuzzlerWindow::gotCandidateCount(int expectedCount)
{
    // Set the maximum for the progress bar and update it.
    expectedCandidateCount_ = expectedCount;
    updateGeneratorProgress();
}

void PuzzlerWindow::candidateGenerated(const QString &candidate)
{
    // Update the progress bar.
    ++candidateCount_;
    updateGeneratorProgress();

    // If the candidate is a valid word (is present in the dictionary), keep track of it.
    if (dictionary_.testWord(candidate.toLower())) {
        if (solutions_.insertRows(solutions_.rowCount(), 1)) {
            QModelIndex modelIndex = solutions_.index(solutions_.rowCount() - 1, 0);
            solutions_.setData(modelIndex, candidate);
        }
    }
}

void PuzzlerWindow::noMoreCandidates()
{
    // We're done generating candidates.
    int solutionCount = solutions_.rowCount();
    generator_ = nullptr;
    // Remove the progress bar.
    ui->statusBar->removeWidget(&generatorProgress_);
    // Show a message about the solutions.
    QString message("Found ");
    message.append(QString::number(solutionCount));
    message.append(" solution");
    if (solutionCount != 1) {
        message.append("s");
    }
    message.append(".");
    // The message will go away in 15 sec if not cleared sooner.
    ui->statusBar->showMessage(message, 15000);
    // Re-enable the UI (if appropriate).
    enableUi();
}

bool PuzzlerWindow::textIsAcceptable()
{
    return WordGenerator::textIsAcceptable(ui->lineEdit->text());
}

void PuzzlerWindow::enableUi()
{
    // The line editor is disabled if we're currently solving a puzzle, enabled otherwise.
    ui->lineEdit->setEnabled(generator_ == nullptr);
    // The "solve" button is disabled if we're solving a puzzle, if the dictionary is not
    // loaded, or if the line editor text is not acceptable.
    ui->solveButton->setEnabled(generator_ == nullptr && dictReady_ && textIsAcceptable());
}
