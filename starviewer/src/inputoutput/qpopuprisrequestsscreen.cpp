/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "qpopuprisrequestsscreen.h"

#include <QTimer>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QMovie>

#include "qpopuprisrequestsscreen.h"
#include "logging.h"
#include "starviewerapplication.h"
#include "retrievedicomfilesfrompacsjob.h"
#include "study.h"
#include "pacsrequeststatus.h"
#include "patient.h"
#include "settings.h"
#include "applicationstylehelper.h"

namespace udg {

const int QPopUpRISRequestsScreen::msTimeOutToHidePopUp = 5000;
const int QPopUpRISRequestsScreen::msTimeOutToMovePopUpToBottomRight = 5000;

QPopUpRISRequestsScreen::QPopUpRISRequestsScreen(QWidget *parent): QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::SubWindow | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    m_qTimerToHidePopUp = new QTimer();
    m_qTimerToHidePopUp->setSingleShot(true);
    connect(m_qTimerToHidePopUp,SIGNAL(timeout()), SLOT(hidePopUpSmoothly()));

    QMovie *operationAnimation = new QMovie(this);
    operationAnimation->setFileName(":/images/loader.gif");
    m_operationAnimation->setMovie(operationAnimation);
    operationAnimation->start();

    connect(&m_hidePopUpAnimation, SIGNAL(finished()), this, SLOT(hidePopUp()));

    // TODO: Aquesta és la única manera que s'ha trobat de que el text, al canviar-lo a un tamany major, no quedi tallat
    // caldria refer el diàleg i vigilar el tema de com es fa per situar-lo, etc. perquè ara mateix és una mica "hack".
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void QPopUpRISRequestsScreen::queryStudiesByAccessionNumberStarted()
{
    m_labelRisRequestDescription->setText(tr("%1 has received a request from RIS to retrieve studies.").arg(ApplicationNameString));
    m_operationDescription->setText(tr("Querying PACS..."));
    m_operationAnimation->show();

    m_studiesRetrievingCounter->setText("");

    m_pacsJobIDOfStudiesToRetrieve.clear();
    m_numberOfStudiesRetrieved = 0;
}

void QPopUpRISRequestsScreen::addStudyToRetrieveByAccessionNumber(RetrieveDICOMFilesFromPACSJob *retrieveDICOMFilesFromPACSJob)
{
    if (m_pacsJobIDOfStudiesToRetrieve.count() == 0)
    {
        //Si és el primer estudi indiquem que comencem a descarregar i indiquem el nom del pacient
        m_operationDescription->setText(tr("Retrieving study"));
        showPatientNameOfRetrievingStudies(retrieveDICOMFilesFromPACSJob->getStudyToRetrieveDICOMFiles()->getParentPatient());
    }

    m_pacsJobIDOfStudiesToRetrieve.append(retrieveDICOMFilesFromPACSJob->getPACSJobID());
    refreshScreenRetrieveStatus();

    connect(retrieveDICOMFilesFromPACSJob, SIGNAL(PACSJobFinished(PACSJob*)), SLOT(retrieveDICOMFilesFromPACSJobFinished(PACSJob *)));
    connect(retrieveDICOMFilesFromPACSJob, SIGNAL(PACSJobCancelled(PACSJob*)), SLOT(retrieveDICOMFilesFromPACSJobCancelledOrFailed(PACSJob *)));
}

void QPopUpRISRequestsScreen::retrieveDICOMFilesFromPACSJobFinished(PACSJob *pacsJob)
{
    RetrieveDICOMFilesFromPACSJob *retrieveDICOMFilesFromPACSJob = qobject_cast<RetrieveDICOMFilesFromPACSJob*>(pacsJob);

    if (retrieveDICOMFilesFromPACSJob == NULL)
    {
        ERROR_LOG("El PACSJob que ha finalitzat no és un RetrieveDICOMFilesFromPACSJob");
    }
    else
    {
        if (m_pacsJobIDOfStudiesToRetrieve.contains(retrieveDICOMFilesFromPACSJob->getPACSJobID()))
        {
            //Si no està a la llista de PACSJob per descarregar vol dir que és d'una altra petició de RIS que ha estat matxacada per l'actual
            //Com que el QPopUpRisRequestScreen només segueix l'última petició del RIS les ignorem
            if (retrieveDICOMFilesFromPACSJob->getStatus() == PACSRequestStatus::RetrieveOk || 
                retrieveDICOMFilesFromPACSJob->getStatus() == PACSRequestStatus::RetrieveSomeDICOMFilesFailed)
            {
                m_numberOfStudiesRetrieved++;
                
                refreshScreenRetrieveStatus();
            }
            else
            {
                retrieveDICOMFilesFromPACSJobCancelledOrFailed(pacsJob);
            }
        }
    }
}

void QPopUpRISRequestsScreen::retrieveDICOMFilesFromPACSJobCancelledOrFailed(PACSJob *pacsJob)
{
    RetrieveDICOMFilesFromPACSJob *retrieveDICOMFilesFromPACSJob = qobject_cast<RetrieveDICOMFilesFromPACSJob*>(pacsJob);

    if (retrieveDICOMFilesFromPACSJob == NULL)
    {
        ERROR_LOG("El PACSJob que ha finalitzat no és un RetrieveDICOMFilesFromPACSJob");
    }
    else
    {
        //Si ha fallat o s'ha cancel·lat la descarrega l'estudi el treiem de la llista d'estudis a descarregar
        if (m_pacsJobIDOfStudiesToRetrieve.removeOne(retrieveDICOMFilesFromPACSJob->getPACSJobID()))
        {
            //Si no està a la llista de PACSJob per descarregar vol dir que és d'una altra petició de RIS que ha estat matxacada per l'actual
            //Com que el QPopUpRisRequestScreen només segueix l'última petició del RIS les ignorem
            refreshScreenRetrieveStatus();
        }
    }
}

void QPopUpRISRequestsScreen::refreshScreenRetrieveStatus()
{
    if (m_numberOfStudiesRetrieved < m_pacsJobIDOfStudiesToRetrieve.count())
    {
        m_studiesRetrievingCounter->setText(QString(tr("%1 of %2.")).arg(m_numberOfStudiesRetrieved + 1).arg(m_pacsJobIDOfStudiesToRetrieve.count()));
    }
    else
    {
        showRetrieveFinished();
        m_qTimerToHidePopUp->start(msTimeOutToHidePopUp);
    }
}

void QPopUpRISRequestsScreen::showNotStudiesFoundMessage()
{
    m_operationDescription->setText(tr("No studies found."));
    m_operationAnimation->hide();
    m_qTimerToHidePopUp->start(msTimeOutToHidePopUp);
}

void QPopUpRISRequestsScreen::showRetrieveFinished()
{
    m_operationAnimation->hide();

    if (m_numberOfStudiesRetrieved == 0)
    {
        m_operationDescription->setText(tr("No studies has been retrieved.").arg(m_numberOfStudiesRetrieved));
    }
    else if (m_numberOfStudiesRetrieved  == 1)
    {
        m_operationDescription->setText(tr("%1 study has been retrieved.").arg(m_numberOfStudiesRetrieved));
    }
    else
    {
        m_operationDescription->setText(tr("%1 studies have been retrieved.").arg(m_numberOfStudiesRetrieved));
    }
    m_studiesRetrievingCounter->setText("");
}

void QPopUpRISRequestsScreen::showPatientNameOfRetrievingStudies(Patient *patient)
{
    QString popUpText = tr("%1 has received a request from RIS to retrieve studies of patient %2.").arg(ApplicationNameString).arg(patient->getFullName());
    m_labelRisRequestDescription->setText(popUpText);
}

void QPopUpRISRequestsScreen::showEvent(QShowEvent *)
{
    //Es situa el PopUp al centre de la pantalla on està la mainWindow del Starviewer
    //TODO Ara s'està agafant la geometria de manera "xapussilla" dels settings d'interface, caldria solucionar-ho i fer-ho diferent
    Settings settings;
    QWidget fakeMainWindow;

    settings.restoreGeometry(QString("geometry"), &fakeMainWindow);

    this->move(QApplication::desktop()->screenGeometry(&fakeMainWindow).center() - this->rect().center());

    // TODO Ho hem de fer aquí ja que, tal i com està ara, si es fa al constructor aquest es fa abans que es cridi
    // ApplicationStyleHelper::recomputeStyleToScreenOfWidget
    ApplicationStyleHelper style;
    style.setScaledFontSizeTo(this);
    style.setScaledSizeTo(m_operationAnimation->movie());

    QTimer::singleShot(msTimeOutToMovePopUpToBottomRight, this, SLOT(moveToBottomRight()));
}

void QPopUpRISRequestsScreen::hidePopUpSmoothly()
{
    if(m_operationAnimation->isHidden() && m_hidePopUpAnimation.state() != QAbstractAnimation::Running)
    {
        m_hidePopUpAnimation.setTargetObject(this);
        m_hidePopUpAnimation.setPropertyName("windowOpacity");
        m_hidePopUpAnimation.setDuration(1000);
        m_hidePopUpAnimation.setEndValue(0.0);
        m_hidePopUpAnimation.start();
    }
}

void QPopUpRISRequestsScreen::hidePopUp()
{
    if (m_operationAnimation->isHidden())
    {
        //Si es mostra el gif d'operació vol dir que ha arribat una petició de RIS mentre estava corrent el QTimer, per això hem de fer no s'amagui
        this->hide();
    }
    this->setWindowOpacity(1.0);
}

void QPopUpRISRequestsScreen::moveToBottomRight()
{
    if(m_moveToBottomAnimation.state() != QAbstractAnimation::Running)
    {
        m_moveToBottomAnimation.setTargetObject(this);
        m_moveToBottomAnimation.setPropertyName("pos");
        m_moveToBottomAnimation.setDuration(2000);
        m_moveToBottomAnimation.setEndValue(QApplication::desktop()->availableGeometry(this).bottomRight() - this->rect().bottomRight());
        m_moveToBottomAnimation.setEasingCurve(QEasingCurve::OutQuint);
        m_moveToBottomAnimation.start();
    }
}

QPopUpRISRequestsScreen::~QPopUpRISRequestsScreen()
{
    m_qTimerToHidePopUp->stop();
    delete m_qTimerToHidePopUp;
}

};
