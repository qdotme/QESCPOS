#ifndef QESCPOS_H
#define QESCPOS_H

#include "QESCPOS_global.h"

#include "qextserialport.h"

#define DECLARE_STATIC(name, ...) static QByteArray name##Command(__VA_ARGS__); void name(__VA_ARGS__);

class QESCPOSSHARED_EXPORT QESCPOS : public QextSerialPort
{
public:
    QESCPOS();
    QESCPOS(const QString &name);

    enum Type {
        TM_T88IV
    };

    void setType(Type t);

    DECLARE_STATIC(cutPaper, bool full=false, int pos=-1)
    void write(const QByteArray &data);
};

#endif // QESCPOS_H
