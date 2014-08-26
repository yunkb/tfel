/*!
 * \file   st2tot2.ixx
 * \brief  This file implements the methods of the class st2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_ST2TOT2_IXX_
#define _LIB_TFEL_MATH_ST2TOT2_IXX_ 

#include <cmath>
#include <iterator>
#include <algorithm>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/StorageTraits.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"

namespace tfel{

  namespace math {

    // Assignement operator
    template<typename Child>
    template<typename ST2toT2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toT2Type,ST2toT2Concept>::cond &&
      ST2toT2Traits<Child>::dime==ST2toT2Traits<ST2toT2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename ST2toT2Traits<ST2toT2Type>::NumType,
				       typename ST2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tot2_base<Child>::operator=(const ST2toT2Type& src)
    {
      Child& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::copy(src,child);
      return child;
    }

    template<typename Child>
    template<typename ST2toT2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toT2Type,ST2toT2Concept>::cond &&
      ST2toT2Traits<Child>::dime==ST2toT2Traits<ST2toT2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename ST2toT2Traits<ST2toT2Type>::NumType,
				       typename ST2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tot2_base<Child>::operator+=(const ST2toT2Type& src)
    {
      Child& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::plusEqual(child,src);
      return child;
    }

    // Assignement operator
    template<typename Child>
    template<typename ST2toT2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toT2Type,ST2toT2Concept>::cond &&
      ST2toT2Traits<Child>::dime==ST2toT2Traits<ST2toT2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename ST2toT2Traits<ST2toT2Type>::NumType,
				       typename ST2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tot2_base<Child>::operator-=(const ST2toT2Type& src)
    {
      Child& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::minusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<typename ST2toT2Traits<Child>::NumType,
						 T2,OpMult>::type,
			     typename ST2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tot2_base<Child>::operator*=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::multByScalar(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<typename ST2toT2Traits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename ST2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tot2_base<Child>::operator/=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::divByScalar(child,s);
      return child;
    }

    template<unsigned short N, typename T>
    st2tot2<N,T>::st2tot2(const T init)
    {
      tfel::fsalgo::fill<TensorDimeToSize<N>::value*
			 StensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    st2tot2<N,T>::st2tot2(const ST2toT2Expr<st2tot2<N,T2>,Expr>& src){
      matrix_utilities<TensorDimeToSize<N>::value,
		       StensorDimeToSize<N>::value,
		       StensorDimeToSize<N>::value>::copy(src,*this);
    }

    template<unsigned short N, typename T>
    T& 
    st2tot2<N,T>::operator()(const unsigned short i,const unsigned short j){
      assert(i<TensorDimeToSize<N>::value);
      assert(j<StensorDimeToSize<N>::value);
      return this->v[StensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    const T& 
    st2tot2<N,T>::operator()(const unsigned short i,const unsigned short j) const{
      assert(i<TensorDimeToSize<N>::value);
      assert(j<StensorDimeToSize<N>::value);
      return this->v[StensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void st2tot2<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value*StensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T>
    typename st2tot2<N,T>::RunTimeProperties
    st2tot2<N,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    } // end of st2tot2<N,T>::getRunTimeProperties

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ST2TOT2_IXX */
