/*!
 * \file   StensorSymmetricProductDerivative.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   17 août 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_STENSORSYMMETRICPRODUCTDERIVATIVE_HXX
#define LIB_STENSORSYMMETRICPRODUCTDERIVATIVE_HXX

#ifndef  LIB_TFEL_MATH_ST2TOST2_HXX
#error "This file is not meant to be included directly"
#endif

namespace tfel{

  namespace math{

    template<unsigned short N,typename T>
    struct StensorSymmetricProductDerivative;

    template<typename T>
    struct StensorSymmetricProductDerivative<1u,T>{
      template<typename StensorType>
      static TFEL_MATH_INLINE
      typename std::enable_if<
	tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond &&
        StensorTraits<StensorType>::dime==1u&&
	tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,T>::cond,
	tfel::math::st2tost2<1u,T>>::type
      exe(const StensorType& s){
	using real = typename tfel::typetraits::BaseType<T>::type;
	constexpr const real zero = real(0);
	return {2*s[0],zero,zero,
		zero,2*s[1],zero,
		zero,zero,2*s[2]};
      }
    }; // end of struct StensorSymmetricProductDerivative<1u,T>

    template<typename T>
    struct StensorSymmetricProductDerivative<2u,T>{
      template<typename StensorType>
      static TFEL_MATH_INLINE typename std::enable_if<
	tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond &&
      StensorTraits<StensorType>::dime==2u&&
      tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,T>::cond,
      tfel::math::st2tost2<2u,T>>::type
      exe(const StensorType& s){
	using real = typename tfel::typetraits::BaseType<T>::type;
	constexpr const real zero = real(0);
	return {2*s[0],zero,zero,s[3],
	    zero,2*s[1],zero,s[3],
	    zero,zero,2*s[2],zero,
	    s[3],s[3],zero,s[1]+s[0]};
      }
    }; // end of struct StensorSymmetricProductDerivative<2u,T>

    template<typename T>
    struct StensorSymmetricProductDerivative<3u,T>{
      template<typename StensorType>
      static TFEL_MATH_INLINE typename std::enable_if<
	tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond &&
        StensorTraits<StensorType>::dime==3u&&
	tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,T>::cond,
	tfel::math::st2tost2<3u,T>>::type
      exe(const StensorType& s){
	using real = tfel::typetraits::base_type<T>;
	constexpr const auto icste = Cste<T>::isqrt2;
	constexpr const auto zero = real(0);
	return {2*s[0],zero,zero,s[3],s[4],zero,
	    zero,2*s[1],zero,s[3],zero,s[5],
	    zero,zero,2*s[2],zero,s[4],s[5],
	    s[3],s[3],zero,s[1]+s[0],s[5]*icste,s[4]*icste,
	    s[4],zero,s[4],s[5]*icste,s[2]+s[0],s[3]*icste,
	    zero,s[5],s[5],s[4]*icste,s[3]*icste,s[2]+s[1]};
      }
    }; // end of struct StensorSymmetricProductDerivative<3u,T>
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* LIB_STENSORSYMMETRICPRODUCTDERIVATIVE_HXX */
