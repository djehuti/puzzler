// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#pragma once

#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtWidgets/QStatusBar>
#include <utility>

///
/// The WordDictionary holds a set of words, loaded from a text file (such
/// as /usr/share/dict/words). You can test a word for presence in the dictionary.
///
class WordDictionary : public QObject
{
    Q_OBJECT

public:
    /// Constructor.
    explicit WordDictionary(QString dictPath) : dictPath_(std::move(dictPath)) {}

    /// Checks a word for presence in the dictionary.
    bool testWord(const QString &word) const {
        return words_.contains(word);
    }

signals:
    /// Informs the listener of how many words have been loaded so far,
    void loadProgress(int wordCount);

    /// Informs the listener that we are finished loading the dictionary.
    void finished(bool success);

public slots:
    /// Causes the dictionary to be loaded. Progress will be reported via signals.
    void load();

private:
    QString dictPath_;
    QSet<QString> words_;
};
