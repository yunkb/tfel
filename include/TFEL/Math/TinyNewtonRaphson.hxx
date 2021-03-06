/*!
 * \file   include/TFEL/Math/TinyNewtonRaphson.hxx
 * 
 * \brief    
 * \author Thomas Helfer
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_TINYNEWTONRAPHSON_HXX
#define LIB_TFEL_TINYNEWTONRAPHSON_HXX 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/Math/MathException.hxx"
#include"TFEL/Math/TinyMatrixSolve.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel{

  namespace math{

    template<unsigned short N,
	     typename T,
	     typename Func>
    class TinyNewtonRaphson;

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Newton-Raphson/TinyNewtonRaphson.ixx"

#endif /* LIB_TFEL_TINYNEWTONRAPHSON_HXX */

