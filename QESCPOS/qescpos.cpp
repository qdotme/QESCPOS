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

void QESCPOS::setInitial() {
    m_underline = 0;
    m_characterHeight = 1;
    m_characterWidth  = 1;
    m_characterSizeChanged = false;

    connect(this, SIGNAL(characterSizeChanged()), this, SLOT(on_this_characterSizeChanged()), Qt::QueuedConnection);
}



void QESCPOS::write(const QByteArray &data) {
    qDebug() << data.toHex();
    QextSerialPort::write(data);
}

void QESCPOS::demoCharPage(int cpl, int base)
{
    for (int i=32; i<256; i+= cpl) {
        write(QString("%1 ").arg(i, 3, base).toAscii());
        for (unsigned char c = i; c < i+cpl; c++) {
            if (c == 0) break;
            write(QByteArray(1,c));
            if (c % (cpl/4) == (cpl/4) - 1)
                write(" ");
        }
        write("\n");
    }
}

QByteArray QESCPOS::cutPaperCommand(bool full, int pos) {
    QByteArray ret(GS"V");
    if (pos < 0) // function A
        return ret.append((unsigned char)full);
    return ret.append((unsigned char)((int)full+65)).append((unsigned char)pos);

    //! @todo: support for GS V function C, D
}
void QESCPOS::cutPaper(bool full, int pos) { write(cutPaperCommand(full, pos)); }

#define DEFINE_SINGLE_PARAM(name, Name, type, varname, body)  \
    QByteArray QESCPOS::set##Name##Command (type varname) {   \
        body                                                  \
    }                                                         \
                                                              \
    void QESCPOS::set##Name (type varname) {                  \
        if (m_##name == varname) return;                      \
        m_##name = varname;                                   \
        write(set##Name##Command(varname));                   \
    }                                                         \
                                                              \
    type QESCPOS::name () const { return m_##name; }

DEFINE_SINGLE_PARAM(underline,  Underline,  int,  thickness, \
                    return QByteArray(ESC"-").append((unsigned char)thickness); )
DEFINE_SINGLE_PARAM(emphasized, Emphasized, bool, emphasized, \
                    return QByteArray(ESC"E").append((unsigned char)emphasized); )

// *** CHARACTER SIZE ***//
QByteArray QESCPOS::setCharacterSizeCommand(int width, int height) {
    unsigned char n = ((width-1) << 4) + (height-1);
    return QByteArray(GS"!").append(n);
}

void QESCPOS::setCharacterSize(int width, int height) {
    if (!m_characterSizeChanged && (m_characterWidth == width) && (m_characterHeight == height))
        return;
    m_characterWidth = width; m_characterHeight = height;
    write(setCharacterSizeCommand(width, height));
}

void QESCPOS::setCharacterWidth(int n) {
    if (m_characterWidth == n) return;
    m_characterWidth = n;
    emit characterSizeChanged();
}

int QESCPOS::characterWidth() const { return m_characterWidth; }

void QESCPOS::setCharacterHeight(int n) {
    if (m_characterHeight == n) return;
    m_characterHeight = n;
    emit characterSizeChanged();
}

int QESCPOS::characterHeight() const { return m_characterHeight; }

void QESCPOS::on_this_characterSizeChanged() {
    if (m_characterSizeChanged) {
        setCharacterSize(m_characterWidth, m_characterHeight);
        m_characterSizeChanged = false;
    }
}
