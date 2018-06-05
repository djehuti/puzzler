# Copyright (c) 2018 Ben Cox. See the file LICENSE for license information.

load("//tools:qt.bzl", "qt_cc_library")

cc_binary(
    name = "puzzler",
    srcs = [
        "main.cpp",
    ],
    copts = ["-fPIC"],
    deps = [
        ":puzzlerwindow",
        ":worddictionary",
        ":wordgenerator",
        "@qt5//:qt_widgets",
    ],
)

qt_cc_library(
    name = "puzzlerwindow",
    src = "puzzlerwindow.cpp",
    copts = ["-fPIC"],
    hdr = "puzzlerwindow.h",
    includes = ["."],
    ui = "puzzlerwindow.ui",
    ui_deps = [
        "@qt5//:qt_widgets",
    ],
    deps = [
        ":worddictionary",
        ":wordgenerator",
        "@qt5//:qt_core",
        "@qt5//:qt_gui",
        "@qt5//:qt_widgets",
    ],
)

qt_cc_library(
    name = "worddictionary",
    src = "worddictionary.cpp",
    copts = ["-fPIC"],
    hdr = "worddictionary.h",
    includes = ["."],
    deps = [
        "@qt5//:qt_core",
        "@qt5//:qt_gui",
        "@qt5//:qt_widgets",
    ],
)

qt_cc_library(
    name = "wordgenerator",
    src = "wordgenerator.cpp",
    copts = ["-fPIC"],
    hdr = "wordgenerator.h",
    includes = ["."],
    deps = [
        "@qt5//:qt_core",
        "@qt5//:qt_gui",
        "@qt5//:qt_widgets",
    ],
)
