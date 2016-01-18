/*************************************************************************************
  Copyright (C) 2014 Laboratori de Gràfics i Imatge, Universitat de Girona &
  Institut de Diagnòstic per la Imatge.
  Girona 2014. All rights reserved.
  http://starviewer.udg.edu

  This file is part of the Starviewer (Medical Imaging Software) open source project.
  It is subject to the license terms in the LICENSE file found in the top-level
  directory of this distribution and at http://starviewer.udg.edu/license. No part of
  the Starviewer (Medical Imaging Software) open source project, including this file,
  may be copied, modified, propagated, or distributed except according to the
  terms contained in the LICENSE file.
 *************************************************************************************/

#ifndef _LOGGING_
#define _LOGGING_

#include <QString>


/// Macro per a inicialitzar els loggers
/// Assegurar que només es crida una sola vegada, preferiblement crideu-la
/// just després d'incloure el fitxer logging.h al main.cpp.

namespace udg {
    void beginLogging();
    void endLogging(int returnValue);

    void debugLog(const QString &msg);
    void infoLog(const QString &msg);
    void warnLog(const QString &msg);
    void errorLog(const QString &msg);
    void fatalLog(const QString &msg);
    void verboseLog(int vLevel, const QString &msg);
    void traceLog(const QString &msg);
}


/// Macro per a missatges de debug. \TODO de moment fem servir aquesta variable de qmake i funciona bé, però podria ser més adequat troba la forma d'afegir
/// una variable pròpia, com per exemple DEBUG
#ifdef QT_NO_DEBUG
#define DEBUG_LOG(msg) while (false)
#else
#define DEBUG_LOG(msg) udg::debugLog(msg)
#endif

#define INFO_LOG(msg) udg::infoLog(msg)
#define WARN_LOG(msg) udg::warnLog(msg)
#define ERROR_LOG(msg) udg::errorLog(msg)
#define FATAL_LOG(msg) udg::fatalLog(msg)
#define VERBOSE_LOG(vLevel, msg) udg::verboseLog(vLevel, msg)
#define TRACE_LOG(msg) udg::traceLog(msg)

#endif //_LOGGING_
