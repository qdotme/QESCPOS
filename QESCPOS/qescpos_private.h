#ifndef QESCPOS_PRIVATE_H
#define QESCPOS_PRIVATE_H

#define GS  "\x1d"
#define ESC "\x1b"
#define FS  "\x1c"

#define DEFINE_SINGLE_PARAM(rtype, name, Name, type, body)                 \
    QByteArray QESCPOS::set##Name##Command (type name) {                   \
        body                                                               \
    }                                                                      \
                                                                           \
    rtype QESCPOS::set##Name (type name) {                                 \
        if (m_##name == name) return;                                      \
        m_##name = name;                                                   \
        write(set##Name##Command(name));                                   \
    }                                                                      \
                                                                           \
    type QESCPOS::name () const { return m_##name; }

#define DEFINE_BYTE_READBACK(name, body)  \
    QByteArray QESCPOS::name##Command() { \
        body                              \
    }                                     \
                                          \
    unsigned char QESCPOS::name() {       \
        char rval;                        \
        write(name##Command());           \
        read(&rval, 1);                   \
        return rval;                      \
    }

#define DEFINE_SINGLE_BOOL_PARAM_STRING(Name)      \
    ret["setQESCPOS"#Name]   = set##Name##Command(true);  \
    ret["resetQESCPOS"#Name] = set##Name##Command(false);


#endif // QESCPOS_PRIVATE_H
