// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#include "worddictionary.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

namespace {

/// How often we'll inform listeners of our loading progress.
constexpr int WORDS_PER_MESSAGE = 50;

}  // namespace

void WordDictionary::load()
{
    QFile dictFile(dictPath_);
    if (!dictFile.open(QIODevice::ReadOnly)) {
        emit finished(false);
        return;
    }
    QTextStream in(&dictFile);
    QString line;
    int wordCount = 0;
    while (in.readLineInto(&line)) {
        ++wordCount;
        words_.insert(line.toLower());
        if ((wordCount % WORDS_PER_MESSAGE) == 0) {
            emit loadProgress(wordCount);
        }
    }
    emit finished(true);
}
