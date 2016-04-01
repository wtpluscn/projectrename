#include "renamedialog.h"
#include <QApplication>	
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);


	QTranslator* translator = new QTranslator(0);
	translator->load(qApp->applicationDirPath() + "/rename_zh.qm");
	qApp->installTranslator(translator);

	RenameDialog dlg(0);
	return dlg.exec();
}
