#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
