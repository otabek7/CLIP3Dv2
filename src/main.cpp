#include <QApplication>
#include <QDir>
#include <QFile>
#include "src/mainwindow/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("MyOrganization");
    a.setOrganizationDomain("MyDomain");
    a.setApplicationName("CLIP3DGUI");

    QFile styleSheetFile(":/light/stylesheet.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet{QLatin1String(styleSheetFile.readAll())};
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.setWindowTitle("CLIP3DGUI");
    w.setWindowIcon(QIcon(":/new/pics/testicon.ico"));
    w.show();
    return a.exec();
}
