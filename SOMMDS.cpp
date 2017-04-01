///////////////////////////////////////////////////////////
//  SOMMDS.cpp
//  Implementation of the Class SOMMDS
//  Created on:      07-Lie-2013 20:07:32
//  Original author: Povilas
///////////////////////////////////////////////////////////
/*! \class SOMMDS
    \brief A class of methods and attributes for SOMMDS algorithm.
 */
#include "SOMMDS.h"
#include "SOM.h"

SOMMDS::SOMMDS()
{

}

SOMMDS::~SOMMDS()
{

}

//SOMMDS::SOMMDS(double eps, int max_iter, int d, int kx, int ky, int e_hat):SMACOF(eps, max_iter, d), SOM(kx, ky, e_hat){
SOMMDS::SOMMDS(double eps, int max_iter, int d, int kx, int ky, int e_hat): SOM(kx, ky, e_hat, true)
{
    this->eps = eps;
    this->maxIter = max_iter;
    this->d = d;

}

ObjectMatrix SOMMDS::getProjection()
{
    ObjectMatrix M_ws;
    M_ws = SOM::getProjection();
//    som_qe = SOM::getQuantizationError();
    // X = M_ws;
    smcf = new SMACOF (eps, maxIter, d, M_ws);
    // mds_error = SMACOF::getStress();
    return  smcf->getProjection();
}

double SOMMDS::getSOMQuantizationError()
{
    return SOM::getQuantizationError();
}

/*double SOMMDS::getMDSStressError()
{
    return MDS::getStress();
}*/

double SOMMDS::getStress()
{
    return smcf->getStress();
}

ObjectMatrix SOMMDS::getX()
{
    return SMACOF::X;
}
