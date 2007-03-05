/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGTOOL_H
#define UDGTOOL_H

#include <QObject>

// FWD declarations
class QAction;

namespace udg {

class QViewer;

/**
Classe base per a totes les classes de tools

@author Grup de Gràfics de Girona  ( GGG )
*/

class Tool : public QObject{
Q_OBJECT

public slots:
    /// Decideix què s'ha de fer per cada event rebut. Mètode virtual pur que es re-implementa obligatòriament en cada classe filla.
    virtual void handleEvent( unsigned long eventID ) = 0;
    
    /// Premet assignar un determinat comportament a la Tool. Aquest comportament no té sentit si la tool no està preparada per a rebre'l.
    virtual void setBehavior( int behavior ) {m_behavior = behavior;};
    
    /// Ens retorna el comportament actual de la Tool com a enter. 
    virtual int getBehavior() const { return m_behavior; };

protected:
    /// Per controlar l'estat de la tool
    int m_state;
    
    /// Atribut per a determinar el comportament de la Tool
    int m_behavior;

};

};  //  end  namespace udg

#endif

