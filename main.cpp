#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/*    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("PlugIns");
    QApplication::setLibraryPaths(QStringList(dir.absolutePath()));

#if defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
    }
#endif*/

    MainWindow w;
    w.show();

    return a.exec();
}
