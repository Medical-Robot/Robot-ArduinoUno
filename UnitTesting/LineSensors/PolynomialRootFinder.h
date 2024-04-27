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
//  File: PolynomialRootFinder.h
//  Author: Bill Hallahan
//  Date: January 30, 2003
//
//  Abstract:
//
//    This file contains the definition for class PolynomialRootFinder.
//
//**********************************************************************

#ifndef POLYNOMIALROOTFINDER_H
#define POLYNOMIALROOTFINDER_H

#include <vector>

//======================================================================
//  Class definition.
//======================================================================

class PolynomialRootFinder
{
protected:

    typedef float PRF_Float_T;

    std::vector<float> m_p_vector;
    std::vector<float> m_qp_vector;
    std::vector<float> m_k_vector;
    std::vector<float> m_qk_vector;
    std::vector<float> m_svk_vector;

    float * m_p_vector_ptr;
    float * m_qp_vector_ptr;
    float * m_k_vector_ptr;
    float * m_qk_vector_ptr;
    float * m_svk_vector_ptr;

    int m_degree;
    int m_n;
    int m_n_plus_one;
    float m_real_s;
    float m_imag_s;
    float m_u;
    float m_v;
    float m_a;
    float m_b;
    float m_c;
    float m_d;
    float m_a1;
    float m_a2;
    float m_a3;
    float m_a6;
    float m_a7;
    float m_e;
    float m_f;
    float m_g;
    float m_h;
    float m_real_sz;
    float m_imag_sz;
    float m_real_lz;
    float m_imag_lz;
    PRF_Float_T m_are;
    PRF_Float_T m_mre;

public:

    enum RootStatus_T
    {
        SUCCESS,
        LEADING_COEFFICIENT_IS_ZERO,
        SCALAR_VALUE_HAS_NO_ROOTS,
        FAILED_TO_CONVERGE
    };

    PolynomialRootFinder();

    virtual ~PolynomialRootFinder();

    PolynomialRootFinder::RootStatus_T FindRoots(float * coefficient_ptr,
                                                 int degree,
                                                 float * real_zero_vector_ptr,
                                                 float * imaginary_zero_vector_ptr,
                                                 int * number_of_roots_found_ptr = 0);

private:

    int Fxshfr(int l2var);

    int QuadraticIteration(float uu, float vv);

    int RealIteration(float & sss, int & flag);

    int CalcSc();

    void NextK(int itype);

    void Newest(int itype, float & uu, float & vv);

    void QuadraticSyntheticDivision(int n_plus_one,
                                    float u,
                                    float v,
                                    float * p_ptr,
                                    float * q_ptr,
                                    float & a,
                                    float & b);

    void SolveQuadraticEquation(float a,
                                float b,
                                float c,
                                float & sr,
                                float & si,
                                float & lr,
                                float & li);

    //==================================================================
    //  Declare the copy constructor and operator equals to be private
    //  and do not implement them to prevent copying instances of this
    //  class.
    //==================================================================

    PolynomialRootFinder(const PolynomialRootFinder & that);

    PolynomialRootFinder operator =(const PolynomialRootFinder & that);
};

#endif
