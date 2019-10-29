#ifndef MANBERMYERS_GLOBAL_H
#define MANBERMYERS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MANBERMYERS_LIBRARY)
#  define MANBERMYERS_EXPORT Q_DECL_EXPORT
#else
#  define MANBERMYERS_EXPORT Q_DECL_IMPORT
#endif

#endif // MANBERMYERS_GLOBAL_H
