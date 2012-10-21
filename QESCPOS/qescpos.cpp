#include "qescpos.h"
#include <QDebug>

#define GS  "\x1d"
#define ESC "\x1b"

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
        return ret.append((unsigned char)full);
    return ret.append((unsigned char)((int)full+66)).append((unsigned char)pos)

    //! @todo: support for GS V function C, D
}
void QESCPOS::cutPaper(bool full, int pos) { write(cutPaperCommand(full, pos)); }

QByteArray QESCPOS::setUnderlineCommand(int thickness) {
    return QByteArray(ESC"-").append((unsigned char)thickness);
}
void QESCPOS::setUnderline(int thickness) { write(setUnderlineCommand(thickness)); }
