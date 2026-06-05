QT += core gui widgets xml

TARGET = run
TEMPLATE = app

INCLUDEPATH += inc 

SOURCES += tests/main.cpp \
           src/hvd.cpp

HEADERS += inc/hvd_icp.h \
           inc/hvd.h \
           inc/shmem.h

# Output Directories
UI_DIR = build/ui         # ui_*.h files
MOC_DIR = build/moc       # moc_*.cpp files
OBJECTS_DIR = build/obj   # *.o files
DESTDIR = build/bin       # final executable