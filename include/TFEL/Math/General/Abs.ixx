/*!
 * \file   include/TFEL/Math/General/Abs.ixx
 * \brief  This file declares the abs function
 * \author Thomas Helfer
 * \date   13 Feb 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_ABSIXX
#define LIB_TFEL_MATH_ABSIXX 

#include<cmath>
#include<functional>

#include"TFEL/Config/TFELConfig.hxx"
#include<type_traits>
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    AbsSum<T>::AbsSum(const T& v)
      : result(v)
    {}
    
    template<typename T>
    void
    AbsSum<T>::operator()(const T& v)
    {
      result += abs(v);
    } // end of AbsSum::operator

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_ABSIXX */

