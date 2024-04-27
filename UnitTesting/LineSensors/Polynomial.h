//=======================================================================
// Copyright (C) 2003-2013 William Hallahan
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//=======================================================================

//**********************************************************************
//  File: Polynomial.h
//  Author: Bill Hallahan
//  Date: January 30, 2003
//
//  Abstract:
//
//    This file contains the definition for class Polynomial.
//
//**********************************************************************

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include "PolynomialRootFinder.h"

//======================================================================
//  Class definition.
//======================================================================

class Polynomial
{
protected:

    std::vector<float> m_coefficient_vector;
    int m_degree;
    float * m_coefficient_vector_ptr;

public:

    Polynomial();

    Polynomial(float scalar);

    Polynomial(float x_coefficient, float scalar);

    Polynomial(float x_squared_coefficient,
               float x_coefficient,
               float scalar);

    Polynomial(float * coefficient_vector, int degree);

    Polynomial(const Polynomial & polynomial);

    virtual ~Polynomial();

    void SetCoefficients(float * coefficient_vector_ptr,
                         int degree);

    void SetToScalar(float scalar);

    void SetToFirstOrderPolynomial(float x_coefficient, float scalar);

    void SetToQuadraticPolynomial(float x_squared_coefficient,
                                  float x_coefficient,
                                  float scalar);

    float EvaluateReal(float xr) const;

    float EvaluateReal(float xr, float & dr) const;

    void EvaluateImaginary(float xi,
                           float & pr,
                           float & pi) const;

    void EvaluateComplex(float xr,
                         float xi,
                         float & pr,
                         float & pi) const;

    void EvaluateComplex(float xr,
                         float xi,
                         float & pr,
                         float & pi,
                         float & dr,
                         float & di) const;

    Polynomial Derivative() const;

    Polynomial Integral() const;

    int Degree() const;

    PolynomialRootFinder::RootStatus_T FindRoots(float * real_zero_vector_ptr,
                                                 float * imaginary_zero_vector_ptr,
                                                 int * roots_found_ptr = 0) const;

    void IncludeRealRoot(float real_value);

    void IncludeComplexConjugateRootPair(float real_value, float imag_value);

    bool Divide(const Polynomial & divisor_polynomial,
                Polynomial & quotient_polynomial,
                Polynomial & remainder_polynomial) const;

    float operator [](int power_index) const;

    float & operator [](int power_index);

    Polynomial operator +=(const Polynomial & polynomial);

    Polynomial operator +=(float scalar);

    Polynomial operator -=(const Polynomial & polynomial);

    Polynomial operator -=(float scalar);

    Polynomial operator *=(const Polynomial & polynomial);

    Polynomial operator *=(float scalar);

    Polynomial operator /=(float scalar);

    Polynomial operator +();

    Polynomial operator -();

    Polynomial operator =(float scalar);

    Polynomial operator =(const Polynomial & polynomial);

private:

    void AdjustPolynomialDegree();

    void Copy(const Polynomial & polynomial);

    void SetLength(unsigned int number_of_coefficients,
                   bool copy_data_flag = true);
};

//======================================================================
//  Global operators.
//======================================================================

//======================================================================
//  Addition of two instances of this class.
//======================================================================

Polynomial operator +(const Polynomial & polynomial_0,
                      const Polynomial & polynomial_1);

//======================================================================
//  Addition of an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator +(const Polynomial & polynomial,
                      float scalar);

Polynomial operator +(float scalar,
                      const Polynomial & polynomial);

//======================================================================
//  Subtraction of two instances of this class.
//======================================================================

Polynomial operator -(const Polynomial & minuend_polynomial,
                      const Polynomial & subtrahend_polynomial);

//======================================================================
//  Subtraction with an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator -(const Polynomial & minuend_polynomial,
                      float scalar);

Polynomial operator -(float scalar,
                      const Polynomial & polynomial);

//======================================================================
//  Multiplication of two instances of this class.
//======================================================================

Polynomial operator *(const Polynomial & polynomial_0,
                      const Polynomial & polynomial_1);

//======================================================================
//  Multiplication of an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator *(const Polynomial & polynomial,
                      float scalar);

Polynomial operator *(float scalar,
                      const Polynomial & polynomial);

//======================================================================
//  Division with an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator /(const Polynomial & polynomial,
                      float scalar);
#endif
