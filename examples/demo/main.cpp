#include <QCoreApplication>

#include <qescpos.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QESCPOS printer("/dev/ttyUSB0");
    printer.setBaudRate(BAUD19200);
    printer.open(QIODevice::ReadWrite);

    printer.write("Foo\n\n\n\n");
    printer.cutPaper();

    return a.exec();
}
