#TEMPLATE = lib
#TARGET = Pep10Common
#CONFIG += staticlib
QT += widgets printsupport concurrent
INCLUDEPATH += $$PWD\..\pep10common
VPATH += $$PWD\..\pep10common

FORMS += \
    asmobjectcodepane.ui \
    asmsourcecodepane.ui \
    executionstatisticswidget.ui \
    memorytracepane.ui \
    redefinemnemonicsdialog.ui \
    asmcpupane.ui \
    asmprogramtracepane.ui \
    asmprogramlistingpane.ui \
    assemblerpane.ui

HEADERS += \
    asmargument.h \
    asmcode.h \
    asmobjectcodepane.h \
    asmprogram.h \
    asmprogrammanager.h \
    asmsourcecodepane.h \
    cpphighlighter.h \
    executionstatisticswidget.h \
    interfaceisacpu.h \
    isaasm.h \
    macro.h \
    macromodules.h \
    macroregistry.h \
    macrotokenizer.h \
    memorycellgraphicsitem.h \
    memorytracepane.h \
    ngraph.h \
    ngraph_prune.h \
    pepasmhighlighter.h \
    setops.hpp \
    typetags.h \
    stacktrace.h \
    redefinemnemonicsdialog.h \
    asmcpupane.h \
    isacpu.h \
    isacpumemoizer.h \
    memoizerhelper.h \
    asmprogramtracepane.h \
    asmprogramlistingpane.h \
    assemblerpane.h

SOURCES += \
    asmargument.cpp \
    asmcode.cpp \
    asmobjectcodepane.cpp \
    asmprogram.cpp \
    asmprogrammanager.cpp \
    asmsourcecodepane.cpp \
    cpphighlighter.cpp \
    executionstatisticswidget.cpp \
    interfaceisacpu.cpp \
    isaasm.cpp \
    macro.cpp \
    macromodules.cpp \
    macroregistry.cpp \
    macrotokenizer.cpp \
    memorycellgraphicsitem.cpp \
    memorytracepane.cpp \
    pepasmhighlighter.cpp \
    typetags.cpp \
    stacktrace.cpp \
    redefinemnemonicsdialog.cpp \
    asmcpupane.cpp \
    isacpu.cpp \
    isacpumemoizer.cpp \
    memoizerhelper.cpp \
    asmprogramtracepane.cpp \
    asmprogramlistingpane.cpp \
    assemblerpane.cpp
