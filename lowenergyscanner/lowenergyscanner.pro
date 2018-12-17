TARGET = lowenergyscanner
INCLUDEPATH += .

QT += quick bluetooth

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    serviceinfo.cpp \
    characteristicinfo.cpp \
    legomotor.cpp

OTHER_FILES += assets/*.qml

HEADERS += \
    device.h \
    deviceinfo.h \
    serviceinfo.h \
    characteristicinfo.h \
    legomotor.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/lowenergyscanner
INSTALLS += target
