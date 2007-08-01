/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGEXTENSIONHANDLER_H
#define UDGEXTENSIONHANDLER_H

#include <QObject>
#include <QString>
#include "identifier.h"
#include "patient.h"
#include "studyvolum.h"

#include "patientfillerinput.h"

namespace udg {

// Fordward Declarations
class VolumeRepository;
class QApplicationMainWindow;
class Volume;
class PatientFillerInput;

// Espai reservat pels fwd decl de les mini-apps
class AppImportFile;

// Fi de l'Espai reservat pels fwd decl de les mini-apps
class QueryScreen;
/**
Gestor de mini-aplicacions i serveis de l'aplicació principal

@author Grup de Gràfics de Girona  ( GGG )

*/
class ExtensionHandler : public QObject{
Q_OBJECT
public:
    ExtensionHandler( QApplicationMainWindow *mainApp , QObject *parent = 0, QString name = 0);

    ~ExtensionHandler();

public slots:
    /// rep la petició d'un servei/mini-aplicació i fa el que calgui
    void request( int who );
    void request( const QString &who );

    /// aplicació que s'executa per defecte quan carreguem un volum al repositori
    void onVolumeLoaded( Identifier id );

    /// De moment, primera aproximació a arrencar el pacs de l'Starviewer.
    void viewPatient(PatientFillerInput patientFillerInput);

    /// \TODO [apanyo] es crida quan es demana un studi descarregat, es veu la pimera serie
    void viewStudy( StudyVolum study );

    /// \TODO [apanyo] Slot que afegeix al 2n visor una sèrie escollida per comparar-> és un mètode moooolt temporal.
    void viewStudyToCompare( StudyVolum study );

    /// \TODO [apanyo] Obre la imatge de perfusió del mètode de difusió-perfusió.
    void viewStudyForPerfusion( StudyVolum study );

    /// cridat quan l'aplicació mor
    void killBill();

    /// \TODO [temporal] Llança el diàleg per obrir la imatge de perfusió del mètode de difusió-perfusió.
    void openPerfusionImage();

    /// \TODO [temporal] Obre una sèrie per comparar en el visor per defecte
    void openSerieToCompare();

private:
    /// Punter a l'aplicació principal
    QApplicationMainWindow *m_mainApp;

    /// Entitat pacient que es controlarà des d'aquí
    Patient m_patient;

    /// S'encarrega de fer el registre de totes les extensions amb el factory
    void registerExtensions();

    /// Crea les connexions de signals i slots
    void createConnections();

    // :::::::::::::::::::::::::::::::::::::::::
    // Recursos
    // :::::::::::::::::::::::::::::::::::::::::

    /// L'id del volum amb el que estem treballant
    Identifier m_volumeID;

    /// \TODO aquest ha de ser temporal
    Identifier m_compareVolumeID;

    /// El repository de volums
    VolumeRepository *m_volumeRepository;

    // :::::::::::::::::::::::::::::::::::::::::
    // Mini Aplicacions
    // Aquí es declararan totes les mini-aplicacions que es faran servir
    // :::::::::::::::::::::::::::::::::::::::::

    /// Importar models del sistema de fitxers al repositori de volums
    AppImportFile *m_importFileApp;

    /// La pantalla d'accés al pacs
    QueryScreen *m_queryScreen;

    /// carrega una nova pestanya amb l'extensió de 2DViewer
    void load2DViewerExtension();

private slots:

    /// Slot que es crida quan hem canviat d'una extensió a una altre
    void extensionChanged( int index );

signals:
    /// Emet un senyal amb el segon volum per comparar
    void secondInput( Volume* );

    /// [temporal] Emet un senyal amb el volum de perfusió del mètode de difusió-perfusió.
    void perfusionImage( Volume * volume );

};

};  //  end  namespace udg

#endif
