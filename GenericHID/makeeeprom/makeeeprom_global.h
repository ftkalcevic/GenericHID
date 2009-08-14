#ifndef MAKEEEPROM_GLOBAL_H
#define MAKEEEPROM_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef MAKEEEPROM_LIB
# define MAKEEEPROM_EXPORT Q_DECL_EXPORT
#else
# define MAKEEEPROM_EXPORT Q_DECL_IMPORT
#endif

#endif // MAKEEEPROM_GLOBAL_H
