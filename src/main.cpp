#include "mainUI/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	// Specify the plugin folders
	// (language codecs and image loaders)
	app.addLibraryPath("codecs");
	app.addLibraryPath("iconengines");
	app.addLibraryPath("imageformats");

	// Set application information for
	// easier use of QSettings classes
	QCoreApplication::setOrganizationName("");
	QCoreApplication::setOrganizationDomain("");
	QCoreApplication::setApplicationName("Pisces Reader");
	QCoreApplication::setApplicationVersion("0.1.0");

	app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    MainWindow w;
    w.show();

    return app.exec();
}
