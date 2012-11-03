#include "qescpos.h"
#include <QDebug>
#include <QColor>

#include <arpa/inet.h>

#include "qescpos_private.h"

// Here be hungry dragons,
//#include <boost/preprocessor/list.hpp>
//#include <boost/preprocessor/seq.hpp>

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
    qDebug() << data.length() << data.toHex();
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


DEFINE_SINGLE_PARAM(void, underline,    Underline,    int,                         \
    return QByteArray(ESC"-").append((unsigned char)underline); )

DEFINE_SINGLE_PARAM(void, emphasized,   Emphasized,   bool,                        \
    return QByteArray(ESC"E").append((unsigned char)emphasized); )

DEFINE_SINGLE_PARAM(void, doubleStrike, DoubleStrike, bool,                        \
    return QByteArray(ESC"G").append((unsigned char)doubleStrike); )

DEFINE_SINGLE_PARAM(void, upsideDown,   UpsideDown,   bool,                        \
    return QByteArray(ESC"{").append((unsigned char)upsideDown); )

DEFINE_SINGLE_PARAM(void, reverse,      Reverse,      bool,                        \
    return QByteArray(GS"B").append((unsigned char)reverse); )

DEFINE_SINGLE_PARAM(void, smoothing,    Smoothing,    bool,                        \
    return QByteArray(GS"b").append((unsigned char)smoothing); )

DEFINE_SINGLE_PARAM(void, font,         Font,         QESCPOS::Font,               \
    return QByteArray(ESC"M").append((unsigned char)font); )

DEFINE_SINGLE_PARAM(void, internationalCharacterSet, InternationalCharacterSet, QESCPOS::ICS, \
    return QByteArray(ESC"R").append((unsigned char)internationalCharacterSet); )

// ESC r - print color - not featured on TM88-IV.

DEFINE_SINGLE_PARAM(void, characterCodeTable, CharacterCodeTable, QESCPOS::CCT, \
    return QByteArray(ESC"t").append((unsigned char)characterCodeTable); )

DEFINE_SINGLE_PARAM(void, clockwiseRotation, ClockwiseRotation, QESCPOS::Rotation, \
    return QByteArray(ESC"V").append((unsigned char)clockwiseRotation); )

DEFINE_SINGLE_PARAM(void, characterColor, CharacterColor, QESCPOS::Color,       \
    return QByteArray(GS"(N\x02\x00\x30").append((unsigned char)characterColor); )

DEFINE_SINGLE_PARAM(void, backgroundColor, BackgroundColor, QESCPOS::Color,     \
    return QByteArray(GS"(N\x02\x00\x31").append((unsigned char)backgroundColor); )

// GS ( N 03 00 32 shading color.

DEFINE_SINGLE_PARAM(void, justification, Justification, QESCPOS::Just, \
    return QByteArray(ESC"a").append((unsigned char)justification); )

QByteArray QESCPOS::generatePulseCommand(int m, int t1, int t2) {
    return QByteArray(ESC"p").append((unsigned char)m).append((unsigned char)(t1/2)).append((unsigned char)(t2/2));
}

void QESCPOS::generatePulse(int m, int t1, int t2) {
    write(generatePulseCommand(m, t1, t2));
}

QByteArray QESCPOS::initializeCommand() {
    return QByteArray(ESC"\x40");
}

void QESCPOS::initialize() {
    write(initializeCommand());
}

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

QByteArray QESCPOS::printNVRasterCommand(int n, int scaleX, int scaleY) {
    QByteArray ret = QByteArray(FS"p").append((char)n).append((char)0);
    return ret;
}

void QESCPOS::printNVRaster(int n, int scaleX, int scaleY) {
    write(printNVRasterCommand(n, scaleX, scaleY));
}

QByteArray QESCPOS::printRasterCommand(QImage i, int scaleX, int scaleY) {
    QImage i_mono = i.convertToFormat(QImage::Format_Mono);
    if (i.hasAlphaChannel()) {
        qDebug() << "Printing with alpha";
        for (int x = 0; x<i.width(); x++)
            for (int y = 0; y<i.height(); y++)
                i_mono.setPixel(x, y, (255-qAlpha(i.pixel(x, y)))*(255-qGray(i.pixel(x, y))) < 128*255);
    }
    else
        for (int x = 0; x<i.width(); x++)
            for (int y = 0; y<i.height(); y++)
                i_mono.setPixel(x, y, qGray(i.pixel(x, y)) < 128);


    QByteArray ret = QByteArray(GS"v0").append((char)0); //! @todo scaleXY

    qDebug() << "Image Width:"  << i.width()  << htons(i.width());  // 013E or 3E01
    qDebug() << "Image Height:" << i.height() << htons(i.height());

    uint16_t w = i.width() / 8; // Portability warning - convert to BE
    uint16_t h = i.height();

    ret = ret.append((char*)&w, 2).append((char*)&h, 2);
/*
    for (int y=0; y<i.height(); y++)
        for (int x=0; x<i.width(); x+=8) {
            char c=0, o=128, idx=0;
            while (idx<8) {
                c |= o*(i.pixel(x+idx, y))
                idx++;
            }

            for (int cx = x; (cx<i.width())
        }
*/
    for (int y=0; y<i.height(); y++)
        ret = ret.append((const char*) i_mono.scanLine(y), i.width() / 8);

//    ret = ret.append((const char*) i_mono.bits(), i.width() * i.height() / 8);
    return ret;
}

void QESCPOS::printRaster(QImage i, int scaleX, int scaleY) {
    write(printRasterCommand(i, scaleX, scaleY));
}

QByteArray QESCPOS::_spitNVRasterData(QImage i_orig) {
    QTransform t;
    t.rotate(-90);
    t.scale(-1, 1);
    QImage i = i_orig.transformed(t).convertToFormat(QImage::Format_Mono); // , Qt::MonoOnly | Qt::ThresholdDither | Qt::ThresholdAlphaDither);


    if (i_orig.hasAlphaChannel())
        for (int x = 0; x<i.width(); x++)
            for (int y = 0; y<i.height(); y++)
                i.setPixel(x, y, (255-qAlpha(i_orig.pixel(y, x)))*(255-qGray(i_orig.pixel(y, x))) < 128*255);
    else
        for (int x = 0; x<i.width(); x++)
            for (int y = 0; y<i.height(); y++)
                i.setPixel(x, y, qGray(i_orig.pixel(y, x)) < 128);


    QByteArray ret;

    qDebug() << "Image Width:"  << i.width()  << htons(i.width());  // 013E or 3E01
    qDebug() << "Image Height:" << i.height() << htons(i.height());

    uint16_t h = i.width() / 8; // Portability warning - convert to BE
    uint16_t w = i.height() / 8;

    ret = ret.append((char*)&w, 2).append((char*)&h, 2);

    for (int y=0; y<w * 8; y++)
        ret = ret.append((const char*) i.scanLine(y), i.width() / 8);

    return ret;
}

QByteArray QESCPOS::defineNVRasterCommand(QList<QImage> l) {
    QByteArray ret = QByteArray(FS"q").append((char)l.count());
    Q_FOREACH(QImage i, l)
        ret.append(_spitNVRasterData(i));

    return ret;
}

void QESCPOS::defineNVRaster(QList<QImage> l) {
    write(defineNVRasterCommand(l));
}


DEFINE_BYTE_READBACK(modelId, \
    return QByteArray(GS"I").append((char)1); )
DEFINE_BYTE_READBACK(typeId, \
    return QByteArray(GS"I").append((char)2); )
DEFINE_BYTE_READBACK(versionId, \
    return QByteArray(GS"I").append((char)3); )


void QESCPOS::autoConfig() {
    QList<BaudRateType> l_baudrates;
    l_baudrates << BAUD19200 << BAUD38400 << BAUD9600 << BAUD4800 << BAUD2400;

    setParity(PAR_NONE);
    setFlowControl(FLOW_HARDWARE);

    BaudRateType rate;
    Q_FOREACH(rate, l_baudrates) {
        setBaudRate(rate);
        initialize();
        if (modelId()==32)
            break;
    }

    initialize();

    //write("QESCPOS Sequence");
    cutPaper(true, 0);

    if (rate == BAUD19200)
        write("AutoConfig: BAUD19200\n");
    if (rate == BAUD38400)
        write("AutoConfig: BAUD38400\n");
    if (rate == BAUD9600)
        write("AutoConfig: BAUD9600\n");
    if (rate == BAUD4800)
        write("AutoConfig: BAUD4800\n");
    if (rate == BAUD2400)
        write("AutoConfig: BAUD2400\n");

    write("AutoConfig: Assume: PAR_NONE\n");
    write("AutoConfig: Assume: FLOW_HARDWARE\n");

    cutPaper(true, 0);
}

/* *** Command Maps and API adapters *** */

QMap<QString, QByteArray> QESCPOS::getCommandMap() {
    QMap<QString, QByteArray> ret;

    for(int x = 1; x <= 8; x++)
        for(int y = 1; y <= 8; y++)
            ret[QString("setQESCPOSCharacterSize_%1x%2").arg(x).arg(y)] = setCharacterSizeCommand(x, y);

    for (int i=1; i<=255; i++)
        for(int x = 1; x <= 2; x++)
            for(int y = 1; y <= 2; y++)
                ret[QString("printNVRaster_%1_%2x%3").arg(i).arg(x).arg(y)] = printNVRasterCommand(i, x, y);

    ret["setQESCPOSFontA"] = setFontCommand(FONT_A);
    ret["setQESCPOSFontB"] = setFontCommand(FONT_B);

    DEFINE_SINGLE_BOOL_PARAM_STRING(Emphasized);
    DEFINE_SINGLE_BOOL_PARAM_STRING(Smoothing);


    return ret;
}
