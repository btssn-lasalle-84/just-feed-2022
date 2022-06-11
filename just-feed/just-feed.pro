QT       += core gui widgets sql
QT       += webenginewidgets
QT       += mqtt

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    basededonnees.cpp \
    communication.cpp \
    distributeur.cpp \
    main.cpp \
    ihmjustfeed.cpp

HEADERS += \
    basededonnees.h \
    communication.h \
    distributeur.h \
    ihmjustfeed.h

FORMS += \
    ihmjustfeed.ui

RESOURCES += \
        ressources.qrc

COPIES += bd
bd.files = just-feed.sqlite
bd.path = $$OUT_PWD/
bd.base = $$PWD/

COPIES += configuration
configuration.files = justfeed.ini
configuration.path = $$OUT_PWD/
configuration.base = $$PWD/

CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT
