#ifndef QESCPOS_H
#define QESCPOS_H

#include "QESCPOS_global.h"

#include "qextserialport.h"

#include <QBitmap>


#define DECLARE_STATIC(type, name, ...)                              \
    static QByteArray name##Command(__VA_ARGS__);                    \
    type name(__VA_ARGS__);

#define DECLARE_STATIC_VOID(name, ...) DECLARE_STATIC(void, name, __VA_ARGS__)

#define DECLARE_STATIC_VOID_SINGLESTATE(name, Name, type, def)     \
    private:                                                  \
    type m_##name;                                            \
    public:                                                   \
    DECLARE_STATIC_VOID(set##Name, type name = def)           \
    type name() const;                                        \



class QESCPOSSHARED_EXPORT QESCPOS : public QextSerialPort
{
    Q_OBJECT
    Q_PROPERTY(int      underline                 READ underline                 WRITE setUnderline                 )
    Q_PROPERTY(bool     emphasized                READ emphasized                WRITE setEmphasized                )
    Q_PROPERTY(bool     doubleStrike              READ doubleStrike              WRITE setDoubleStrike              )
    Q_PROPERTY(bool     upsideDown                READ upsideDown                WRITE setUpsideDown                )
    Q_PROPERTY(bool     reverse                   READ reverse                   WRITE setReverse                   )
    Q_PROPERTY(bool     smoothing                 READ smoothing                 WRITE setSmoothing                 )

    Q_PROPERTY(int      characterWidth            READ characterWidth            WRITE setCharacterWidth            )
    Q_PROPERTY(int      characterHeight           READ characterHeight           WRITE setCharacterHeight           )

    Q_PROPERTY(ICS      internationalCharacterSet READ internationalCharacterSet WRITE setInternationalCharacterSet )
    Q_PROPERTY(CCT      characterCodeTable        READ characterCodeTable        WRITE setCharacterCodeTable        )
    Q_PROPERTY(Color    characterColor            READ characterColor            WRITE setCharacterColor            )
    Q_PROPERTY(Color    backgroundColor           READ backgroundColor           WRITE setBackgroundColor           )
    Q_PROPERTY(Rotation clockwiseRotation         READ clockwiseRotation         WRITE setClockwiseRotation         )
    Q_PROPERTY(Font     font                      READ font                      WRITE setFont                      )

    Q_PROPERTY(Just     justification             READ justification             WRITE setJustification             )


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
    
    enum Rotation {
        ROTATE_NONE    = 0,
        ROTATE_SPACE1  = 1,
        ROTATE_SPACE15 = 2
    };

    enum Font {
        FONT_A   = 0,
        FONT_B   = 1,
        FONT_C   = 2,
        FONT_EXT = 97
    };

    void setType(Type t);

    // ** ACTIVES ** //
    DECLARE_STATIC_VOID(cutPaper,        bool full=false, int pos=-1)

    // ** STATE CHANGERS ** //
    DECLARE_STATIC_VOID_SINGLESTATE(underline,                 Underline,                 int,      1             )
    DECLARE_STATIC_VOID_SINGLESTATE(emphasized,                Emphasized,                bool,     true          )
    DECLARE_STATIC_VOID_SINGLESTATE(doubleStrike,              DoubleStrike,              bool,     true          )
    DECLARE_STATIC_VOID_SINGLESTATE(upsideDown,                UpsideDown,                bool,     true          )
    DECLARE_STATIC_VOID_SINGLESTATE(reverse,                   Reverse,                   bool,     true          )
    DECLARE_STATIC_VOID_SINGLESTATE(smoothing,                 Smoothing,                 bool,     true          )

    DECLARE_STATIC_VOID_SINGLESTATE(internationalCharacterSet, InternationalCharacterSet, ICS,      ICS_UK        )
    DECLARE_STATIC_VOID_SINGLESTATE(characterCodeTable,        CharacterCodeTable,        CCT,      CCT_PC437     )
    DECLARE_STATIC_VOID_SINGLESTATE(characterColor,            CharacterColor,            Color,    C_COLOR2      )
    DECLARE_STATIC_VOID_SINGLESTATE(backgroundColor,           BackgroundColor,           Color,    C_COLOR2      )
    DECLARE_STATIC_VOID_SINGLESTATE(clockwiseRotation,         ClockwiseRotation,         Rotation, ROTATE_SPACE1 )
    DECLARE_STATIC_VOID_SINGLESTATE(font,                      Font,                      Font,     FONT_B        )

    DECLARE_STATIC_VOID_SINGLESTATE(justification,             Justification,             Just,  JUST_CENTER      )

    DECLARE_STATIC_VOID(initialize, )

    DECLARE_STATIC_VOID(setShadingColor,    Color color = C_COLOR2, bool shadow = true)

    DECLARE_STATIC_VOID(setCharacterSize,   int width = 2, int height = 2)

    int characterWidth () const; void setCharacterWidth (int n);
    int characterHeight() const; void setCharacterHeight(int n);
    
    
    DECLARE_STATIC_VOID(printRaster,        QImage i,        int scaleX = 1, int scaleY = 1)
    DECLARE_STATIC_VOID(printNVRaster,      int n,           int scaleX = 1, int scaleY = 1)

    DECLARE_STATIC_VOID(defineNVRaster,     QList<QImage> l)
    
    Q_INVOKABLE qint64 write(const QByteArray &data);

    void demoCharPage(int charactersPerLine = 32, int base = 10);

    DECLARE_STATIC(unsigned char, modelId, )
    DECLARE_STATIC(unsigned char, typeId, )
    DECLARE_STATIC(unsigned char, versionId, )

    DECLARE_STATIC_VOID(generatePulse, int m, int t1, int t2)

    // int printerModelId

//    QMap<QString, QByteArray> getStaticCommandMap();
    QMap<QString, QByteArray> getCommandMap();
    void autoConfig();    
public slots:
    void on_this_characterSizeChanged();

signals:
    void characterSizeChanged();

private:
    void setInitial();
    static QByteArray _spitNVRasterData(QImage i);


    int  m_characterWidth;
    int  m_characterHeight;

    bool m_characterSizeChanged;
};

#endif // QESCPOS_H
