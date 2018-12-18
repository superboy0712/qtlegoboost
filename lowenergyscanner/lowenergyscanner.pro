TARGET = lowenergyscanner
INCLUDEPATH += .

QT += quick bluetooth

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    serviceinfo.cpp \
    characteristicinfo.cpp \
    legomotor.cpp \
    legocolordistancesensor.cpp \
    legoabmotor.cpp \
    legocdmotor.cpp \
    legocontrol.cpp

OTHER_FILES += assets/*.qml

HEADERS += \
    device.h \
    deviceinfo.h \
    serviceinfo.h \
    characteristicinfo.h \
    legomotor.h \
    legocolordistancesensor.h \
    legoabmotor.h \
    legocdmotor.h \
    constant.h \
    legocontrol.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/lowenergyscanner
INSTALLS += target
