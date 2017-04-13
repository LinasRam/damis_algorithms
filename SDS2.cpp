
#include "SDS2.h"
#include "ShufleObjects.h"
#include "Projection.h"
#include "PCA.h"
#include "SMACOF.h"
#include "alglib/optimization.h"
#include "DistanceMetrics.h"
#include "AdditionalMethods.h"
#include "ObjectMatrix.h"
#include <float.h>
//#include <fstream>
#include <cmath>
#include <iostream>

class StaticData2
{
public:
    static ObjectMatrix X_base;
    static ObjectMatrix Y_base;
    static ObjectMatrix X_new;

};

ObjectMatrix StaticData2::X_base;
ObjectMatrix StaticData2::Y_base;
ObjectMatrix StaticData2::X_new;

SDS2::SDS2()
{
    //ctor
}

SDS2::~SDS2()
{
    //dtor
}

SDS2::SDS2(double eps, int maxIter, int dim):MDS(eps, maxIter, dim)
{
    X = ObjectMatrix(AdditionalMethods::inputDataFile);
    X.loadDataMatrix();
    XX = ObjectMatrix(AdditionalMethods::inputDataFile2);
    XX.loadDataMatrix();
    XXX = ObjectMatrix(AdditionalMethods::inputDataFile3);
    XXX.loadDataMatrix();
    epsilon = eps;
    maxIteration = maxIter;
    XObjCount= X.getObjectCount();
    XXObjCount= XX.getObjectCount();
    //  d = dim;

}

ObjectMatrix SDS2::getProjection()
{
  //  std::vector<std::string> initClasses = X.getStringClassAttributes();

    int step = 0, rest = 0;
    StaticData2::X_base = XX;
    StaticData2::X_new = X;
    StaticData2::Y_base = XXX;
    Y_new = ObjectMatrix(XObjCount);
   // ObjectMatrix proj(m);
    std::vector<unsigned int> index;
    index.reserve(XObjCount);
/*
    switch (initMethod)
    {
    case 1:
        proj = Projection::projectMatrix(RAND, X);
        break;
    case 2:
        proj = Projection::projectMatrix(PCA, X);
        break;
    case 3:
        proj = Projection::projectMatrix(DISPERSION, X);
        break;
    default:
        proj = Projection::projectMatrix(DISPERSION, X);
        break;
    }

    index = ShufleObjects::shufleObjectMatrix(BUBLESORTDSC, proj); //works slow
    step = m / nb;
    for (int i = 0; i < nb; i++)
    {
        rest = index.at(i);
        index.at(i) = index.at(i * step);
        index.at(i * step) = rest;
    }

    for (int i = 0; i < m; i++)
    {
        if (i < nb)
            StaticData::X_base.addObject(X.getObjectAt(index.at(i)));
        else
            StaticData::X_new.addObject(X.getObjectAt(index.at(i)));
    } */

 //   StaticData::X_base.setIsClassPresent();

 /*  for (int i = 0; i < StaticData::X_base.getObjectCount(); i++)
           std::cout <<StaticData::X_base.getObjectAt(i).getClassLabel();*/



    /* PCA_ pca(StaticData::X_base, d);
    StaticData::Y_base = pca.getProjection();*/

  /* for (int i = 0; i < StaticData::Y_base.getObjectCount(); i++)
           std::cout <<StaticData::Y_base.getObjectAt(i).getClassLabel();*/

   /* SMACOF smcf(epsilon, maxIteration, d, StaticData::X_base, StaticData::Y_base);
    StaticData::Y_base = smcf.getProjection(); */

   Initialize(); // get the nearest

    Y.clearDataObjects();

    //initializeProjectionMatrix();

    for (int i = 0; i < XXObjCount; i++)
        Y.addObject(StaticData2::Y_base.getObjectAt(i), X.getObjectAt(index.at(i)).getClassLabel()); //neaisku ar nesumaiso klasių
   //     Y.addObject(StaticData::Y_base.getObjectAt(i), X.getObjectAt(i).getClassLabel());

//sudedam tai kas grazinama is QN
    ObjectMatrix tmpX;
//ObjectMatrix retMat;
    for (int i = 0; i < XObjCount; i++) //redo slow call and much more
    {
        //StaticData::X_new.clearDataObjects();
        tmpX.addObject(X.getObjectAt(index.at(i)));
        StaticData2::X_new = tmpX;

        // StaticData::X_new.addObject(X.getObjectAt(index.at(nb + i)));
        // tmpY.addObject(Y_new.getObjectAt(i));
        //std::cout << tmpY.getObjectAt(0).getFeatureAt(0) <<" " << tmpY.getObjectAt(0).getFeatureAt(1) << std::endl;
        //std::cout << i << std::endl;
        // retMat = getQN(tmpY);

        Y.addObject(getQN(Y_new.getObjectAt(i)), X.getObjectAt(index.at(i)).getClassLabel()) ;//retMat.getObjectAt(0));
        //Y.addObject(getQN(Y_new.getObjectAt(i)), X.getObjectAt(nb + i).getClassLabel()) ; //neaisku ar nesumaiso klasių

        //   tmpY.clearDataObjects();
        tmpX.clearDataObjects();
    }
    Y.setPrintClass(X.getStringClassAttributes());
    return  Y;
}

DataObject SDS2::getQN(DataObject Yqn)
{
    int m = 1; // number of objects that will be passed to Newton
    alglib::minlbfgsstate state;
    alglib::minlbfgsreport rep;
    double epsg = epsilon;
    double epsf = 0;
    double epsx = 0;
    alglib::ae_int_t maxits = maxIteration;
    alglib::real_1d_array Ynew;
    Ynew = AdditionalMethods::DataObjectTo1DArray(Yqn);

    alglib::minlbfgscreate(m, Ynew, state);
    alglib::minlbfgssetcond(state, epsg, epsf, epsx, maxits);
    alglib::minlbfgsoptimize(state,  E_SDS, NULL, NULL);
    alglib::minlbfgsresults(state, Ynew, rep);

    return AdditionalMethods::alglib1DArrayToDataObject(Ynew);
}

/*double SDS::getStress(){
    return DimReductionMethod::getStress();

   /* int m = X.getObjectCount();
    double dist1 = 0.0, dist2 = 0.0;
    double distX = 0.0, distY = 0.0;

    for (int i = 0; i < m - nb - 1; i++)
        for (int j = i + 1; j < m - nb; j++)
        {
            distX = DistanceMetrics::getDistance(StaticData::X_new.getObjectAt(i), StaticData::X_new.getObjectAt(j), EUCLIDEAN);
            distY = DistanceMetrics::getDistance(Y_new.getObjectAt(i), Y_new.getObjectAt(j), EUCLIDEAN);
            dist1 += pow(distX - distY, 2);
        }

    for (int i = 0; i < nb - 1; i++)
        for (int j = 0; j < m - nb; j++)
        {
            distX = DistanceMetrics::getDistance(StaticData::X_base.getObjectAt(i), StaticData::X_new.getObjectAt(j), EUCLIDEAN);
            distY = DistanceMetrics::getDistance(StaticData::Y_base.getObjectAt(i), Y_new.getObjectAt(j), EUCLIDEAN);
            dist2 += pow(distX - distY, 2);
        }

    return dist1 + dist2;
}*/

 void SDS2::E_SDS(const alglib::real_1d_array &Ynew, double &func, alglib::real_1d_array &grad, void *ptr)
{
    double f1 = 0.0, f2 = 0.0, distX = 0.0, distY = 0.0;
    int d = StaticData2::Y_base.getObjectAt(0).getFeatureCount();
    int sm = StaticData2::X_new.getObjectCount();
    int nb = StaticData2::X_base.getObjectCount();
    std::vector<double> items;
    items.reserve(d);
    DataObject dd[sm];

    for (int i = 0; i < sm; i++)
    {
        for (int j = 0; j < d; j++)
            items.push_back(Ynew[d * i + j]);
        dd[i] = DataObject(items);
        items.clear();
    }

    double tmp;

    for (int i = 0; i < d * sm; i++)
        grad[i] = 0.0;

    for (int i = 0; i < sm ; i++)
    {
        for (int j = 0; j < sm; j++)
        {
            distX = DistanceMetrics::getDistance(StaticData2::X_new.getObjectAt(i), StaticData2::X_new.getObjectAt(j), EUCLIDEAN);
            distY = DistanceMetrics::getDistance(dd[i], dd[j], EUCLIDEAN);
            tmp = distX - distY;
            f1 += tmp * tmp;
            if (distY != 0)
            {
                for (int  k = 0; k < d; k++)
                {
                    grad[j + k] += 2 * (distX - distY) / distY * (Ynew[d * i + k] - Ynew[d * j + k]);
                }
            }
        }
    }

    for (int i = 0; i < nb; i++)
    {
        for (int j = 0; j < sm; j++)
        {
            distX = DistanceMetrics::getDistance(StaticData2::X_base.getObjectAt(i), StaticData2::X_new.getObjectAt(j), EUCLIDEAN);
            distY = DistanceMetrics::getDistance(StaticData2::Y_base.getObjectAt(i), dd[j], EUCLIDEAN);
            tmp = distX - distY;
            f2 += tmp * tmp;
            if (distY != 0)
            {
                for (int  k = 0; k < d; k++)
                {
                    grad[j + k] += 2 * (distX - distY) / distY * (StaticData2::Y_base.getObjectAt(i).getFeatureAt(k) - Ynew[d * j + k]);
                }
            }
        }
    }

    func = f1 + f2;


    /*for (int i = 0; i < sm; i++)
    {
        for (int j = 0; j < sm; j++)
        {
            distX = DistanceMetrics::getDistance(StaticData::X_new.getObjectAt(i), StaticData::X_new.getObjectAt(j), EUCLIDEAN);
            distY = DistanceMetrics::getDistance(dd[i], dd[j], EUCLIDEAN);
            if (distY != 0)
                for (int  k = 0; k < d; k++)
                    {
                        grad[j + k] += 2 * (distX - distY) / distY * (Ynew[d * i + k] - Ynew[d * j + k]);
                    }
        }
    }
    for (int i = 0; i < nb; i++)
    {
        for (int j = 0; j < sm; j++)
        {
            distX = DistanceMetrics::getDistance(StaticData::X_base.getObjectAt(i), StaticData::X_new.getObjectAt(j), EUCLIDEAN);
            distY = DistanceMetrics::getDistance(StaticData::Y_base.getObjectAt(i), dd[j], EUCLIDEAN);

        }
    }*/

}

void SDS2::Initialize()
{

    int closest_base;
    double min_dist;
    double dist_ij;
    DataObject objXi;



    for (int i = 0; i <XObjCount ; i++)
    {
        min_dist = DBL_MAX;
        closest_base = 0;

        objXi = StaticData2::X_new.getObjectAt(i);
        for (int j = 0; j < XXObjCount; j++)
        {
            dist_ij = DistanceMetrics::getDistance(StaticData2::X_base.getObjectAt(j), objXi, EUCLIDEAN);
            if (dist_ij < min_dist)
            {
                min_dist = dist_ij;
                closest_base = j;
            }
        }
        Y_new.addObject(StaticData2::Y_base.getObjectAt(closest_base));
    }
}
