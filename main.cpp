#include "encoder.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Encoder w;
    w.show();
    return a.exec();
}
