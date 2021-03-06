/*! 
 * \file  include/TFEL/Math/ST2toST2/ST2toST2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX
#define LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX 1

#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Math/power.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/General/ConceptRebind.hxx"
#include"TFEL/Math/Forward/TensorConcept.hxx"
#include"TFEL/Math/ExpressionTemplates/Expr.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math{

    //! a simple alias
    template<typename A>
    struct ST2toST2TransposeExpr;
    
    template<typename T>
    struct ST2toST2Traits{
      typedef tfel::meta::InvalidType NumType;
      static constexpr unsigned short dime = 0u;
    };

    //! a simple alias
    template<typename T>
    using ST2toST2NumType = typename ST2toST2Traits<T>::NumType;
    
    /*!
     * \class ST2toST2Tag
     * \brief Helper class to characterise st2tost2.
     */ 
    struct ST2toST2Tag
    {};

    template<class T>
    struct ST2toST2Concept 
    {
      typedef ST2toST2Tag ConceptTag;

      typename ST2toST2Traits<T>::NumType
      operator()(const unsigned short,
		 const unsigned short) const;

    protected:
      ST2toST2Concept() = default;
      ST2toST2Concept(ST2toST2Concept&&) = default;
      ST2toST2Concept(const ST2toST2Concept&) = default;
      ST2toST2Concept&
      operator=(const ST2toST2Concept&) = default;
      ~ST2toST2Concept() = default;
    };

    template<typename T>
    struct ST2toST2Type{
      typedef T type;
    };

    template<typename T>
    struct ConceptRebind<ST2toST2Tag,T>
    {
      typedef ST2toST2Concept<T> type;
    };

    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond,
      typename tfel::typetraits::AbsType<typename ST2toST2Traits<ST2toST2Type>::NumType>::type
    >::type
    abs(const ST2toST2Type&);

    /*!
     * \brief performs the push_forward of a st2tost2:
     * \[
     * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
     * \]
     * \param[out] Ct: result
     * \param[in] C: input
     * \param[in] F: deformation gradient
     */
    template<typename ST2toST2Type,
	     typename ST2toST2Type2,
	     typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond&&
      tfel::meta::Implements<ST2toST2Type2,ST2toST2Concept>::cond&&
      tfel::meta::Implements<TensorType,TensorConcept>::cond&&
      ST2toST2Traits<ST2toST2Type>::dime==1u&&
      ST2toST2Traits<ST2toST2Type2>::dime==1u&
      TensorTraits<TensorType>::dime==1u,
      void>::type
    push_forward(ST2toST2Type&,
		 const ST2toST2Type2&,
		 const TensorType&);
    /*!
     * \brief performs the push_forward of a st2tost2:
     * \[
     * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
     * \]
     * \param[out] Ct: result
     * \param[in] C: input
     * \param[in] F: deformation gradient
     */
    template<typename ST2toST2Type,
	     typename ST2toST2Type2,
	     typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond&&
      tfel::meta::Implements<ST2toST2Type2,ST2toST2Concept>::cond&&
      tfel::meta::Implements<TensorType,TensorConcept>::cond&&
      ST2toST2Traits<ST2toST2Type>::dime==2u&&
      ST2toST2Traits<ST2toST2Type2>::dime==2u&
      TensorTraits<TensorType>::dime==2u,
      void>::type
    push_forward(ST2toST2Type&,
		 const ST2toST2Type2&,
		 const TensorType&);
    /*!
     * \brief performs the push_forward of a st2tost2:
     * \[
     * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
     * \]
     * \param[out] Ct: result
     * \param[in] C: input
     * \param[in] F: deformation gradient
     */
    template<typename ST2toST2Type,
	     typename ST2toST2Type2,
	     typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond&&
      tfel::meta::Implements<ST2toST2Type2,ST2toST2Concept>::cond&&
      tfel::meta::Implements<TensorType,TensorConcept>::cond&&
      ST2toST2Traits<ST2toST2Type>::dime==3u&&
      ST2toST2Traits<ST2toST2Type2>::dime==3u&
      TensorTraits<TensorType>::dime==3u,
      void>::type
    push_forward(ST2toST2Type&,
		 const ST2toST2Type2&,
		 const TensorType&);
    /*!
     * \return a transposed view of  the tensor
     */
    template<typename T>
    TFEL_MATH_INLINE auto
    transpose(T&& t)
    -> typename std::enable_if<
       tfel::meta::Implements<typename std::decay<T>::type,ST2toST2Concept>::cond,
       Expr<typename ST2toST2Type<typename std::decay<T>::type>::type,
            ST2toST2TransposeExpr<decltype(t)>>>::type;
    /*!
     * \return the determinant of a `st2tost2`
     * \param[in] s: fourth order tensor
     */
    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond&&
      (ST2toST2Traits<ST2toST2Type>::dime==1u)&&
      tfel::typetraits::IsScalar<ST2toST2NumType<ST2toST2Type>>::cond,
      typename ComputeUnaryResult<ST2toST2NumType<ST2toST2Type>,
				  Power<3>>::Result
      >::type
    det(const ST2toST2Type&);
    /*!
     * \return the determinant of a `st2tost2`
     * \param[in] s: fourth order tensor
     */
    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond&&
      ((ST2toST2Traits<ST2toST2Type>::dime==2u)||
       (ST2toST2Traits<ST2toST2Type>::dime==3u))&&
      tfel::typetraits::IsScalar<ST2toST2NumType<ST2toST2Type>>::cond,
      typename ComputeUnaryResult<ST2toST2NumType<ST2toST2Type>,
				  Power<ST2toST2Traits<ST2toST2Type>::dime>>::Result
      >::type
    det(const ST2toST2Type&);
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/ST2toST2/ST2toST2Concept.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2CONCEPT_HXX */
