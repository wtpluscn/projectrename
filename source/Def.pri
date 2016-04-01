PROJECT_HOME = ..

contains(CONFIG, debug) {
	BUILDDIR        = $${PROJECT_HOME}/Debug/
}
contains(CONFIG, release) {
	BUILDDIR        = $${PROJECT_HOME}/Release/
}
#message("The BUILDDIR is")
#message($$BUILDDIR)

win32:BUILDDIR  = $$join(BUILDDIR,,,win32)
unix:BUILDDIR   = $$join(BUILDDIR,,,unix)
macx:BUILDDIR   = $$join(BUILDDIR,,,macx)
BUILDDIR        = $$join(BUILDDIR,,,/$$TARGET)

UI_DIR          = .
contains(TEMPLATE, app) {
	UI_DIR          = $${BUILDDIR}/ui
}

UIC_DIR         = $${BUILDDIR}/uic
MOC_DIR         = $${BUILDDIR}/moc
RCC_DIR         = $${BUILDDIR}/rcc
OBJECTS_DIR     = $${BUILDDIR}/obj

DESTDIR    			 = $${PROJECT_HOME}/bin
contains(TEMPLATE, lib) {
    CONFIG  += dll
    unix:DESTDIR  = $${PROJECT_HOME}/bin 
    win32:DESTDIR  = $${PROJECT_HOME}/lib 
    win32:DLLDESTDIR = $${PROJECT_HOME}/bin    
}

INCLUDEPATH += include $${PROJECT_HOME}/include

win32 {
LIBPATH += $${PROJECT_HOME}/lib
DEFINES += WIN32 _WINDOWS _AFXDLL 
DEFINES -= _USRDLL
}

unix {
LIBPATH += $${PROJECT_HOME}/bin
}

