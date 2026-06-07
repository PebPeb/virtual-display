QT += core gui widgets xml

TARGET = run
TEMPLATE = app

INCLUDEPATH += inc 

SOURCES += tests/main.cpp \
           src/vd_gui.cpp \
           src/vd_icp_driver.cpp

HEADERS += inc/vd_gui.h \
           inc/vd_icp_driver.h
           

# Output Directories
UI_DIR = build/ui         # ui_*.h files
MOC_DIR = build/moc       # moc_*.cpp files
OBJECTS_DIR = build/obj   # *.o files
DESTDIR = build/bin       # final executable