#ifndef SV_GMATRIX_H
#define SV_GMATRIX_H

#include "SVGVector.h"
#include "SVObject.h"


template <class Real>
class  GMatrix:public SVObject
{
public:
    // Construction.  In the second constructor, if bZero is 'true', the
    // matrix is set to the zero matrix.  If bZero is 'false', the matrix
    // is set to the identity matrix.
    GMatrix (s32 iRows = 0, s32 iCols = 0);
    GMatrix (s32 iRows, s32 iCols, const Real* afData);
    GMatrix (s32 iRows, s32 iCols, const Real** aafEntry);
    GMatrix (const GMatrix& rkM);
    ~GMatrix ();

    // member access
    void SetSize (s32 iRows, s32 iCols);
    void GetSize (s32& riRows, s32& riCols) const;
    int GetRows () const;
    int GetColumns () const;
    int GetQuantity () const;
    operator const Real* () const;
    operator Real* ();
    const Real* operator[] (s32 iRow) const;
    Real* operator[] (s32 iRow);
    void SwapRows (s32 iRow0, s32 iRow1);
    Real operator() (s32 iRow, s32 iCol) const;
    Real& operator() (s32 iRow, s32 iCol);
    void SetRow (s32 iRow, const GVector<Real>& rkV);
    GVector<Real> GetRow (s32 iRow) const;
    void SetColumn (s32 iCol, const GVector<Real>& rkV);
    GVector<Real> GetColumn (s32 iCol) const;
    void SetMatrix (s32 iRows, s32 iCols, const Real* afEntry);
    void SetMatrix (s32 iRows, s32 iCols, const Real** aafMatrix);
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    GMatrix& operator= (const GMatrix& rkM);

    // comparison
    bool operator== (const GMatrix& rkM) const;
    bool operator!= (const GMatrix& rkM) const;
    bool operator<  (const GMatrix& rkM) const;
    bool operator<= (const GMatrix& rkM) const;
    bool operator>  (const GMatrix& rkM) const;
    bool operator>= (const GMatrix& rkM) const;

    // arithmetic operations
    GMatrix operator+ (const GMatrix& rkM) const;
    GMatrix operator- (const GMatrix& rkM) const;
    GMatrix operator* (const GMatrix& rkM) const;
    GMatrix operator* (Real fScalar) const;
    GMatrix operator/ (Real fScalar) const;
    GMatrix operator- () const;

    // arithmetic updates
    GMatrix& operator+= (const GMatrix& rkM);
    GMatrix& operator-= (const GMatrix& rkM);
    GMatrix& operator*= (Real fScalar);
    GMatrix& operator/= (Real fScalar);

    // matrix products
    GMatrix Transpose () const;  // M^T
    GMatrix TransposeTimes (const GMatrix& rkM) const;  // this^T * M
    GMatrix TimesTranspose (const GMatrix& rkM) const;  // this * M^T

    // matrix-vector operations
    GVector<Real> operator* (const GVector<Real>& rkV) const;  // M * v
    Real QForm (const GVector<Real>& rkU, const GVector<Real>& rkV)
        const;  // u^T*M*v

protected:
    // Support for allocation and deallocation.  The allocation call requires
    // m_iRows, m_iCols, and m_iQuantity to have already been correctly
    // initialized.
    void Allocate (bool bSetToZero);
    void Deallocate ();

    // support for comparisons
    s32 CompareArrays (const GMatrix& rkM) const;

    s32 m_iRows, m_iCols, m_iQuantity;

    // the matrix is stored in row-major form as a 1-dimensional array
    Real* m_afData;

    // An array of pointers to the rows of the matrix.  The separation of
    // row pointers and actual data supports swapping of rows in linear
    // algebraic algorithms such as solving linear systems of equations.
    Real** m_aafEntry;
};

// c * M
template <class Real>
GMatrix<Real> operator* (Real fScalar, const GMatrix<Real>& rkM);

// v^T * M
template <class Real>
GVector<Real> operator* (const GVector<Real>& rkV,
    const GMatrix<Real>& rkM);

typedef GMatrix<f32> GMatrixf;
typedef GMatrix<f64> GMatrixd;


#endif
