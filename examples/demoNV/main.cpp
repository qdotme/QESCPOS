#include <QCoreApplication>

#include <qescpos.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QESCPOS printer("/dev/ttyUSB0");
    printer.setBaudRate(BAUD19200);
    printer.setFlowControl(FLOW_HARDWARE);
    printer.setParity(PAR_NONE);
    printer.open(QIODevice::ReadWrite);
    
    printer.initalize();
    printer.cutPaper(true, 0);

    QList<QImage> l;
    l.append(QImage("http://sample.sy2.com/site/sample.sy2.com/images/logo.png"));
    printer.defineNVRaster(l);

    printer.write("Operation complete\n");
    printer.cutPaper(true, 0);
    return a.exec();
}
