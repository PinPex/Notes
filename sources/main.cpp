#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen splash(QPixmap(QDir::currentPath() + "/debug/pngwingcom-uMou7FaoN-transformed.png"));
    splash.show();
    a.processEvents();
    Widget w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
