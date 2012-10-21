#ifndef QESCPOS_H
#define QESCPOS_H

#include "QESCPOS_global.h"

#include "qextserialport.h"

#include <QBitmap>

#define DECLARE_STATIC(name, ...)                             \
    static QByteArray name##Command(__VA_ARGS__);             \
    void name(__VA_ARGS__);

#define DECLARE_STATIC_SINGLESTATE(name, Name, type, def)     \
    private:                                                  \
    type m_##name;                                            \
    public:                                                   \
    DECLARE_STATIC(set##Name, type name = def)                \
    type name() const;                                        \



class QESCPOSSHARED_EXPORT QESCPOS : public QextSerialPort
{
    Q_OBJECT
    Q_PROPERTY(int  underline       READ underline       WRITE setUnderline      )
    Q_PROPERTY(bool emphasized      READ emphasized      WRITE setEmphasized     )

    Q_PROPERTY(int  characterWidth  READ characterWidth  WRITE setCharacterWidth )
    Q_PROPERTY(int  characterHeight READ characterHeight WRITE setCharacterHeight)


public:
    QESCPOS();
    QESCPOS(const QString &name);

    enum Type {
        TM_T88IV
    };
    
    enum ICS {
        ICS_USA = 0,
        ICS_FRANCE = 1,
        ICS_GERMANY = 2,
        ICS_UK = 3,
        ICS_DENMARK1 = 4,
        ICS_SWEDEN = 5,
        ICS_ITALY = 6,
        ICS_SPAIN = 7,
        ICS_JAPAN = 8,
        ICS_NORWAY = 9,
        ICS_DENMARK2 = 10,
        ICS_SPAIN2 = 11,
        ICS_LATINAMERICA = 12,
        ICS_KOREAN = 13,
        ICS_SLOVENIA = 14,
        ICS_CHINESE = 15,
    };
    
    enum CCT {
        CCT_PC437 = 0,
        CCT_KATAKANA = 1,
        CCT_PC850 = 2,
        CCT_PC860 = 3,
        CCT_PC863 = 4,
        CCT_PC865 = 5,
        CCT_KANJI1 = 6,
        CCT_KANJI2 = 7,
        CCT_KANJI3 = 8,
        CCT_WPC1252 = 16,
        CCT_PC866 = 17,
        CCT_PC852 = 18,
        CCT_PC858 = 19,
        CCT_PAGE254 = 254,
        CCT_PAGE255 = 255,
    };
    
    enum Color {
        C_NONE   = 48,
        C_COLOR1 = 49,
        C_COLOR2 = 50,
        C_COLOR3 = 51,
    };
    
    enum Just {
        JUST_LEFT = 0,
        JUST_CENTER = 1,
        JUST_RIGHT = 2,
    };
    

    void setType(Type t);

    // ** ACTIVES ** //
    DECLARE_STATIC(cutPaper,        bool full=false, int pos=-1)

    // ** STATE CHANGERS ** //
    DECLARE_STATIC_SINGLESTATE(underline,    Underline,    int,  1)
    DECLARE_STATIC_SINGLESTATE(emphasized,   Emphasized,   bool, true)
    DECLARE_STATIC_SINGLESTATE(doubleStrike, DoubleStrike, bool, true)

    DECLARE_STATIC(setFont,         int  font=0)
    DECLARE_STATIC(setInternationalCharacterSet, ICS ics=ICS_UK)
    DECLARE_STATIC(setClockwiseRotation, int rotation=1)
    DECLARE_STATIC(setCharacterCodeTable, CCT cct=CCT_PC437)
    DECLARE_STATIC(setUpsideDown,   bool upsideDown=true)
    DECLARE_STATIC(setCharacterColor,  Color color = C_COLOR2)
    DECLARE_STATIC(setBackgroundColor, Color color = C_COLOR2)
    DECLARE_STATIC(setShadingColor,    Color color = C_COLOR2, bool shadow = true)

    DECLARE_STATIC(setCharacterSize,   int width = 2, int height = 2)
    int characterWidth () const; void setCharacterWidth (int n);
    int characterHeight() const; void setCharacterHeight(int n);

    DECLARE_STATIC(setReverse,         bool reverse   = true)
    DECLARE_STATIC(setSmoothing,       bool smoothing = true)
    
    
    DECLARE_STATIC(setJustification,   Just just = JUST_CENTER)
    DECLARE_STATIC(printRaster,        QBitmap b, int scaleX = 0, int scaleY = 1)
    
    void write(const QByteArray &data);

    void demoCharPage(int charactersPerLine = 32, int base = 10);
public slots:
    void on_this_characterSizeChanged();

signals:
    void characterSizeChanged();

private:
    void setInitial();

    int m_characterWidth;
    int m_characterHeight;

    bool m_characterSizeChanged;
};

#endif // QESCPOS_H
