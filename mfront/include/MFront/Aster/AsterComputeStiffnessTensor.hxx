/*!
 * \file   AsterComputeStiffnessTensor.hxx
 * \brief  This file declares the AsterComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#ifndef _LIB_MFRONT_ASTERCOMPUTESTIFFNESSTENSOR_H_
#define _LIB_MFRONT_ASTERCOMPUTESTIFFNESSTENSOR_H_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterTraits.hxx"

namespace aster
{

  template<unsigned short N,
	   AsterBehaviourType>
  struct AsterComputeStiffnessTensor;

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessTensor<1u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessTensor<2u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessTensor<3u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessTensor<1u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessTensor<2u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeStiffnessTensor<3u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::StiffnessTensor&);
  }; // end of struct AsterComputeStiffnessTensor

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTERCOMPUTESTIFFNESSTENSOR_H */

