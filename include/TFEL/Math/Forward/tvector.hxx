/*! 
 * \file  include/TFEL/Math/Forward/tvector.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_FORWARD_TVECTOR_HXX
#define LIB_TFEL_MATH_FORWARD_TVECTOR_HXX 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace math
  {

    /*
     * a base for tvector or classes acting like tvector.
     */
    template<typename Child,
	     unsigned short N,
	     typename T>
    struct tvector_base;

    /*
     * a class representing tiny vectors of fixed size.
     */
    template<unsigned short N, typename T>
    struct tvector;

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_TVECTOR_HXX */

