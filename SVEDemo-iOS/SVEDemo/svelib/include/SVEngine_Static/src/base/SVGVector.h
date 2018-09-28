#ifndef SV_GVECTOR_H
#define SV_GVECTOR_H
#include "SVObject.h"



template <class Real>
class  GVector 
{
public:
    // construction
    GVector (s32 iSize = 0);
    GVector (s32 iSize, const Real* afTuple);
    GVector (const GVector& rkV);
    ~GVector ();

    // coordinate access
    void SetSize (int iSize);
    s32 GetSize () const;
    operator const Real* () const;
    operator Real* ();
    Real operator[] (s32 i) const;
    Real& operator[] (s32 i);

    // assignment
    GVector& operator= (const GVector& rkV);

    // comparison
    bool operator== (const GVector& rkV) const;
    bool operator!= (const GVector& rkV) const;
    bool operator<  (const GVector& rkV) const;
    bool operator<= (const GVector& rkV) const;
    bool operator>  (const GVector& rkV) const;
    bool operator>= (const GVector& rkV) const;

    // arithmetic operations
    GVector operator+ (const GVector& rkV) const;
    GVector operator- (const GVector& rkV) const;
    GVector operator* (Real fScalar) const;
    GVector operator/ (Real fScalar) const;
    GVector operator- () const;

    // arithmetic updates
    GVector& operator+= (const GVector& rkV);
    GVector& operator-= (const GVector& rkV);
    GVector& operator*= (Real fScalar);
    GVector& operator/= (Real fScalar);

    // vector operations
    Real Length () const;
    Real SquaredLength () const;
    Real Dot (const GVector& rkV) const;
    Real Normalize ();

protected:
    // support for comparisons
    int CompareArrays (const GVector& rkV) const;

    int m_iSize;
    Real* m_afTuple;
};

template <class Real>
 GVector<Real> operator* (Real fScalar, const GVector<Real>& rkV);

typedef GVector<f32> GVectorf;
typedef GVector<f64> GVectord;



#endif
