#ifndef UDGQMODALITIESSELECTORGROUPBOX_H
#define UDGQMODALITIESSELECTORGROUPBOX_H

#include "ui_qmodalitiesselectorgroupboxbase.h"

class QButtonGroup;

namespace udg {

/**
    Combo box per seleccionar modalitats
 */
class QModalitiesSelectorGroupBox : public QGroupBox, private Ui::QModalitiesSelectorGroupBoxBase {
Q_OBJECT
public:
    QModalitiesSelectorGroupBox(QWidget *parent = 0);
    ~QModalitiesSelectorGroupBox();

    /// Permet escollir quines opcions adicionals estan disponibles. Per defecte totes ho estan.
    void enableAllModalitiesCheckBox(bool enable);
    void enableOtherModalitiesCheckBox(bool enable);
    
    /// Indica si podem seleccionar una sola modalitat o m�ltiples
    void setExclusive(bool exlusive);

    /// Ens retorna una llista amb les modalitats seleccionades
    QStringList getCheckedModalities();

    /// Selecciona les modalitats de la llista. Si la modalitat no �s v�lida no marca res.
    void checkModalities(const QStringList &modalities);

private:
    void initialize();

private:
    /// Grup per poder fer que els check box siguin exclusius o no
    QButtonGroup *m_buttonGroup;
};

} // End namespace udg

#endif
