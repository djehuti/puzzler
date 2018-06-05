// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#pragma once

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <utility>

///
/// The WordGenerator takes a base word (e.g., "sell") and generates all
/// possible candidate words from the base word. The base word may contain
/// zero or two carets ('^'). If two carets are present, they are used to
/// constrain the generated words so that the insertions are placed only
/// at the caret positions. Otherwise, all possible insertion points are
/// generated.
///
class WordGenerator : public QThread
{
    Q_OBJECT

public:
    /// Constructor. Accepts the base word (e.g. "sell" or "^se^ll").
    WordGenerator(QObject *parent, QString baseWord)
        : QThread(parent), baseWord_(std::move(baseWord)) {}

    /// Generates the words. Emits the signals listed below in the process.
    void run() override;

    /// Tests whether the given word is an acceptable base word (that is,
    /// it contains zero or two carets, and is itself non-empty).
    static bool textIsAcceptable(const QString &baseWord);

signals:
    /// Emitted at the start of the computation to give the expected number
    /// of words we will be generating.
    void expectedCandidateCount(int expectedCount);

    /// Called for each candidate word generated. It is up to the recipient
    /// of this signal to decide whether the word is a solution to the puzzle.
    void candidateGenerated(const QString &candidate);

    /// Informs the recipient that our calculation is finished.
    void finished();

private:
    /// Holds the generator word for the calculation.
    QString baseWord_;
};
