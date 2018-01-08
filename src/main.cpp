#include "EQCluster.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EQCluster w;
    w.show();

    return a.exec();
}
