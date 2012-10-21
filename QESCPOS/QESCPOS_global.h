#ifndef QESCPOS_GLOBAL_H
#define QESCPOS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QESCPOS_LIBRARY)
#  define QESCPOSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QESCPOSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QESCPOS_GLOBAL_H
