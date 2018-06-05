# Twice Removed Puzzle Solver

The "Puzzler" application is a simple Qt application for solving the
[Twice Removed][nyt] puzzle, by Allen R. Morgan, which appeared in
the New York Times in August 2012.

It uses a brute-force approach to solve the puzzle, which still completes
in a couple of seconds on reasonably current machines.
I wrote a command-line solver for this back in 2012 after the puzzle
appeared, and wrote this Qt version to teach myself Qt in 2018.
I provide it here with source, for others to use as an example.

This code is released under an MIT license. See the file [LICENSE](LICENSE)
for details.

The file [qt.bzl](tools/qt.bzl) is copied from [the Qt-Bazel Example][qtbz]
which is subject to an Apache 2.0 license, as per [this GitHub issue][ghi3].

[nyt]: https://wordplay.blogs.nytimes.com/2012/08/04/variety-twice-removed/
[qtbz]: https://github.com/bbreslauer/qt-bazel-example
[ghi3]: https://github.com/bbreslauer/qt-bazel-example/issues/3
