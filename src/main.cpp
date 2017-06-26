#include "processmonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ProcessMonitor w;

    w.show();

    return app.exec();
}
