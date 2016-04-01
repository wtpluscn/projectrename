include (./Def.pri)

QT       += core gui widgets 

TARGET = ProjectRename
TEMPLATE = app


SOURCES += 	source/main.cpp \
			source/renamedialog.cpp

HEADERS  += include/renamedialog.h

FORMS    += ui/renamedialog.ui

win32:RC_FILE=rc/main.rc

RESOURCES += rc/main.qrc
	
TRANSLATIONS = lang/rename_zh.ts

