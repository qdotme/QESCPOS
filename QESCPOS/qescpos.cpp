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

#define DEFINE_SINGLE_PARAM(name, Name, type, body)           \
    QByteArray QESCPOS::set##Name##Command (type name) {      \
        body                                                  \
    }                                                         \
                                                              \
    void QESCPOS::set##Name (type name) {                     \
        if (m_##name == name) return;                         \
        m_##name = name;                                      \
        write(set##Name##Command(name));                      \
    }                                                         \
                                                              \
    type QESCPOS::name () const { return m_##name; }

DEFINE_SINGLE_PARAM(underline,    Underline,    int,                         \
    return QByteArray(ESC"-").append((unsigned char)underline); )

DEFINE_SINGLE_PARAM(emphasized,   Emphasized,   bool,                        \
    return QByteArray(ESC"E").append((unsigned char)emphasized); )

DEFINE_SINGLE_PARAM(doubleStrike, DoubleStrike, bool,                        \
    return QByteArray(ESC"G").append((unsigned char)doubleStrike); )

DEFINE_SINGLE_PARAM(upsideDown,   UpsideDown,   bool,                        \
    return QByteArray(ESC"{").append((unsigned char)upsideDown); )

DEFINE_SINGLE_PARAM(reverse,      Reverse,      bool,                        \
    return QByteArray(GS"B").append((unsigned char)reverse); )

DEFINE_SINGLE_PARAM(smoothing,    Smoothing,    bool,                        \
    return QByteArray(GS"b").append((unsigned char)smoothing); )

DEFINE_SINGLE_PARAM(font,         Font,         QESCPOS::Font,               \
    return QByteArray(ESC"M").append((unsigned char)font); )

DEFINE_SINGLE_PARAM(internationalCharacterSet, InternationalCharacterSet, QESCPOS::ICS, \
    return QByteArray(ESC"R").append((unsigned char)internationalCharacterSet); )

// ESC r - print color - not featured on TM88-IV.

DEFINE_SINGLE_PARAM(characterCodeTable, CharacterCodeTable, QESCPOS::CCT, \
    return QByteArray(ESC"t").append((unsigned char)characterCodeTable); )

DEFINE_SINGLE_PARAM(clockwiseRotation, ClockwiseRotation, QESCPOS::Rotation, \
    return QByteArray(ESC"V").append((unsigned char)clockwiseRotation); )

DEFINE_SINGLE_PARAM(characterColor, CharacterColor, QESCPOS::Color,       \
    return QByteArray(GS"(N\x02\x00\x30").append((unsigned char)characterColor); )

DEFINE_SINGLE_PARAM(backgroundColor, BackgroundColor, QESCPOS::Color,     \
    return QByteArray(GS"(N\x02\x00\x31").append((unsigned char)backgroundColor); )

// GS ( N 03 00 32 shading color.

DEFINE_SINGLE_PARAM(justification, Justification, QESCPOS::Just, \
    return QByteArray(ESC"a").append((unsigned char)justification); )

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
