#include "qescpos.h"
#include <QDebug>

#define GS "\x1d"

QESCPOS::QESCPOS() : QextSerialPort()
{
}

QESCPOS::QESCPOS(const QString &name) : QextSerialPort(name)
{
}

void QESCPOS::write(const QByteArray &data) {
    qDebug() << data.toHex();
    QextSerialPort::write(data);
}

QByteArray QESCPOS::cutPaperCommand(bool full, int pos) {
    QByteArray ret(GS"V");
    if (pos < 0) // function A
        return ret.append((char)full);


    //! @todo: support for GS V function C, D
}

void QESCPOS::cutPaper(bool full, int pos) { write(cutPaperCommand(full, pos)); }
