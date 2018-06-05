// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#pragma once

#include "worddictionary.h"
#include "wordgenerator.h"

#include <QtCore/QStringListModel>
#include <QtCore/QThread>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>

namespace Ui {

/// Forward declaration.
class PuzzlerWindow;

}

///
/// Implements the main UI for the application.
///
class PuzzlerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PuzzlerWindow(QWidget *parent = nullptr);
    ~PuzzlerWindow() override;

private slots:
    /// Display an About box.
    void showAboutBox();

    /// Invokes the "solve" operation.
    void solvePuzzle();

    /// Triggered when the edit box is edited. Used to enable/disable the solve button.
    void on_lineEdit_textChanged(const QString &text);

    /// Listens for the dictionary loading progress.
    void dictProgress(int numWords);

    /// Listens for the dictionary finished-loading message.
    void dictLoaded(bool success);

    /// Listens for the solution count estimate message.
    void gotCandidateCount(int expectedCount);

    /// Listens for generated candidates.
    void candidateGenerated(const QString &candidate);

    /// Signaled when the solution process is finished.
    void noMoreCandidates();

signals:
    /// Emitted to trigger dictionary load.
    void loadDictionary();

private:
    /// Tests whether the text box's current text is valid.
    bool textIsAcceptable();

    /// Enables or disables the Solve button based on text box contents validity.
    void enableUi();

    /// Updates the solution progress bar.
    void updateGeneratorProgress() {
        generatorProgress_.setRange(0, expectedCandidateCount_);
        generatorProgress_.setValue(candidateCount_);
    }

    /// Holds the UI loaded from the .ui file.
    Ui::PuzzlerWindow *ui;
    /// The worker thread for dictionary loading.
    QThread workerThread_;
    /// The dictionary.
    WordDictionary dictionary_;
    /// How many words are in the dictionary.
    int dictCount_;
    /// Is the dictionary ready?
    bool dictReady_;

    /// The word generator.
    WordGenerator *generator_;
    /// Expected number of candidates (for the progress bar).
    int expectedCandidateCount_;
    /// How many candidates we've seen so far (for the progress bar).
    int candidateCount_;
    /// The solutions we've found so far (for the table).
    QStringListModel solutions_;
    /// The progress bar.
    QProgressBar generatorProgress_;
};
