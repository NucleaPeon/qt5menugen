#ifndef QT5MENUGEN_GLOBAL_H
#define QT5MENUGEN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT5MENUGEN_LIBRARY)
#  define QT5MENUGENSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QT5MENUGENSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QT5MENUGEN_GLOBAL_H
