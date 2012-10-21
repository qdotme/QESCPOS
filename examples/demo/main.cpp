#include <QCoreApplication>

#include <qescpos.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QESCPOS printer("/dev/ttyUSB0");
    printer.setBaudRate(BAUD19200);
    printer.open(QIODevice::ReadWrite);

    printer.write("Foo\n\n\n\nBar\n");
    printer.setUnderline(2);
    printer.write("QESCPOS 2012 (c) Blattaria, Ltd.\n");
    printer.setUnderline(0);
    printer.setCharacterSize();
    printer.setEmphasized(false);
    printer.write("QESCPOS System\n");
    printer.setSmoothing();
    printer.write("QESCPOS System\n");
    printer.setCharacterSize(1, 1);

    for (int i=1; i<=8; i++) {
        printer.demoCharPage();
        printer.demoCharPage(32, 16);
        printer.setCharacterCodeTable((QESCPOS::CCT)i);
    }
    for (int i=16; i<=19; i++) {
        printer.demoCharPage();
        printer.demoCharPage(32, 16);
        printer.setCharacterCodeTable((QESCPOS::CCT)i);
    }
    printer.cutPaper(true, 0);

    return a.exec();
}
