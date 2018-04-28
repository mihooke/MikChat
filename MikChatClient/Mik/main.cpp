#include "mainwindow.h"
#include <QApplication>
#include "utils/logging.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LOG_INFO << "DD";

    return a.exec();
}
