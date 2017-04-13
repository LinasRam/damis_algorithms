#if !defined(SDS2_H)
#define SDS2_H

#include "ObjectMatrix.h"
#include "PCA.h"
#include "SMACOF.h"
#include "MDS.h"
#include "DistanceMetricsEnum.h"
#include "ProjectionEnum.h"



class SDS2 : public MDS
{

public:
    /*! \fn SDS()
    * \brief A default constructor.
    */
    SDS2();
    /*! \fn virtual ~SDS();
    * \brief A destructor.
    */
    virtual ~SDS2();
    /*! \fn SDS(double eps, int maxIter, int d, ProjectionEnum baseVectInitt, int nofBaseVect, DistanceMetricsEnum distMetrics);
     * \brief An overloaded constructor that accepts: epsilon, the maximum number of iterations,
     * base vectors initialization method, the number of base vectors and distance metrics.
     * \param double eps - diff between two iterations
     * \param int maxIter - no of maximum iterations to perform
     * \param int d - projection dimmension
     * \param ProjectionEnum baseVectInitt - enumerated value that indicates the initialization of base vectors
     * \param int nofBaseVect - number of base vectors
     * \param DistanceMetricsEnum distMetrics - enumeration that indicates which distance metrics should be used
     */
    SDS2(double eps, int maxIter, int d);
    /*! \fn virtual ObjectMatrix getProjection();
     *  \brief Returns the projection matrix \a Y of matrix \a X.
     *  \return Y - the projection matrix.
     */
    virtual ObjectMatrix getProjection();
    /*! \fn virtual double getStress();
     *  \brief Returns the stress error value.
     *  \return error - Stress error value.
     */
private:
    /*!
     * Base vector initialization method
     */
     int XObjCount;
     int XXObjCount;

    ObjectMatrix Y_new;
    /*! \var DistanceMetricsEnum distMethod;
     *  \brief Distance measuring metric.
     */

    int nb;
    /*! \fn DataObject getQN();
     *  \brief Calculates one dataobject projection.
     *  \param Yqn - one dataObject for projection calculation.
     */
    DataObject getQN(DataObject Yqn);
    /*! \fn void Initialize();
     *  \brief Initialization of remainder vectors matrix \a Y_new.
     */
    void Initialize();
    /*! \fn static void E_SDS(const alglib::real_1d_array &x, double &func, alglib::real_1d_array &grad, void *ptr);
     *  \brief Function to minimize.
     *  \param x - the address of remainder vectors (initial values).
     *  \param func - The address of the minimized function.
     *  \param grad - The address of gradients.
     *  \param ptr - optional pointer which is passed to func/grad/hess/jac/rep can be NULL
     */
    static void E_SDS(const alglib::real_1d_array &x, double &func, alglib::real_1d_array &grad, void *ptr);

};
#endif // !defined(SDS_H)
