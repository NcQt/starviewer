#ifndef FUZZYCOMPARETESTHELPER_H
#define FUZZYCOMPARETESTHELPER_H

#include <QVector>

class QVector3D;

namespace testing {

/// Classe que contindrÓ les funcions de comparaciˇ fuzzy que necessitem
class FuzzyCompareTestHelper {
public:
    /// Fa una comparaciˇ fuzzy de dos valors double. Tenim un tercer parÓmetre opcional epsilon que ens 
    /// permetrÓ fixar la precisiˇ que volem en la comparaciˇ.
    static bool fuzzyCompare(const double &v1, const double &v2, const double &epsilon = 0.000000001);

    /// Fa una comparaciˇ fuzzy de dos valors QVector<double>. Tots dos hauran de ser de la mateixa longitud.
    /// Tenim un tercer parÓmetre opcional epsilon que ens permetrÓ fixar la precisiˇ que volem en la comparaciˇ.
    static bool fuzzyCompare(const QVector<double> &v1, const QVector<double> &v2, const double &epsilon = 0.000000001);

    /// Fa una comparaciˇ fuzzy de dos valors QVector3D.
    /// Tenim un tercer parÓmetre opcional epsilon que ens permetrÓ fixar la precisiˇ que volem en la comparaciˇ.
    static bool fuzzyCompare(const QVector3D &v1, const QVector3D &v2, const double &epsilon = 0.000000001);
};

}

#endif // FUZZYCOMPARETESTHELPER_H
