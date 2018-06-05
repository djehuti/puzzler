// Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

#include "wordgenerator.h"

#include <mutex>
#include <thread>

#include <QtCore/QString>
#include <QtCore/QVector>

namespace {

/// Builds a list of all possible two-character combinations of letters:
/// aa, ab, ac, ad, ... az, ba, bb, bc, ... zy, zz. This list never changes,
/// so it's built just once.
const QVector<QString> *getPairs() {
    constexpr int numLetters = 'z' - 'a' + 1;
    constexpr int numPairs = numLetters * numLetters;
    static const QVector<QString> *pairs = nullptr;
    static std::once_flag once;
    std::call_once(once, []() {
        QVector<QString> *the_pairs = new QVector<QString>();
        the_pairs->reserve(numPairs);
        QChar letters[2];
        for (char first = 'a'; first <= 'z'; ++first) {
            letters[0] = first;
            for (char second = 'a'; second <= 'z'; ++second) {
                letters[1] = second;
                the_pairs->append(QString(&letters[0], 2));
            }
        }
        pairs = the_pairs;
    });
    return pairs;
}

}  // namespace

void WordGenerator::run() {
    if (!WordGenerator::textIsAcceptable(baseWord_)) {
        emit expectedCandidateCount(0);
        emit finished();
        return;
    }

    QString word = baseWord_.toLower();
    int wordLength = word.length();
    int carets = word.count('^');
    const QVector<QString> *pairs = getPairs();
    QVector<QString> positionals;
    if (carets == 0) {
        int positionalCount = (wordLength + 1) * (wordLength + 2) / 2;
        emit expectedCandidateCount(pairs->size() * positionalCount);
        positionals.reserve(positionalCount);
        for (int firstInsertion = 0; firstInsertion <= wordLength; ++firstInsertion) {
            for (int secondInsertion = firstInsertion; secondInsertion <= wordLength;
                 ++secondInsertion) {
                QString positional(word);
                positional.insert(firstInsertion, '^');
                positional.insert(secondInsertion + 1, '^');
                positionals.push_back(positional);
            }
        }
        Q_ASSERT(positionals.size() == positionalCount);
    } else {
        emit expectedCandidateCount(pairs->size());
        positionals.append(word);
    }

    for (const QString &positional : positionals) {
        for (const QString &pair : *pairs) {
            QString candidate(positional);
            candidate.replace("^", pair);
            emit candidateGenerated(candidate);
        }
    }

    emit finished();
}

bool WordGenerator::textIsAcceptable(const QString &baseWord) {
    if (baseWord.length() == 0 || baseWord == "^^") {
        return false;
    }
    int carets = baseWord.count('^');
    return carets == 0 || carets == 2;
}
