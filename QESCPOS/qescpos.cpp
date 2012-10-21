#include "qescpos.h"

#define GS "\x1d"

QESCPOS::QESCPOS() : QextSerialPort()
{
}

QESCPOS::QESCPOS(const QString &name) : QextSerialPort(name)
{
}

QByteArray QESCPOS::cutPaperCommand(bool full, int pos) {
    QByteArray ret(GS"V");
    if (pos < 0) // function A
        return ret.append((unsigned char)full);
    return ret.append((unsigned char)((int)full+66)).append((unsigned char)pos)

    //! @todo: support for GS V function C, D
}

void QESCPOS::cutPaper(bool full, int pos) { write(cutPaperCommand(full, pos)); }
