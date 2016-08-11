/*!
 * \file   LogarithmicStrain1DBehaviourWrapper.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   28 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include"TFEL/FSAlgorithm/copy.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/LogarithmicStrain1DBehaviourWrapper.hxx"

namespace mtest{

  static void
  convertStiffness(tfel::math::matrix<real>& k,
		   const tfel::math::vector<real>& e,
		   const tfel::math::vector<real>& s){
    k(0,0) = (-s(0)+k(0,0)/(1+e(0)))/(1+e(0));
    k(1,1) = (-s(1)+k(1,1)/(1+e(1)))/(1+e(1));
    k(2,2) = (-s(2)+k(2,2)/(1+e(2)))/(1+e(2));
    k(0,1) = k(0,1)/((1+e(1))*(1+e(0)));
    k(1,0) = k(1,0)/((1+e(0))*(1+e(1)));
    k(2,0) = k(2,0)/((1+e(2))*(1+e(0)));
    k(0,2) = k(0,2)/((1+e(0))*(1+e(2)));
    k(1,2) = k(1,2)/((1+e(2))*(1+e(1)));
    k(2,1) = k(2,1)/((1+e(1))*(1+e(2)));
  }

  LogarithmicStrain1DBehaviourWrapper::LogarithmicStrain1DBehaviourWrapper(const std::shared_ptr<Behaviour>& wb)
    : b(wb)
  {
    const auto h = this->b->getHypothesis();
    if(h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      throw(std::runtime_error("LogarithmicStrain1DBehaviourWrapper::check: "
			       "unsupported hypothesis '"+ModellingHypothesis::toString(h)+"'"));
    }
  }

  LogarithmicStrain1DBehaviourWrapper::Hypothesis
  LogarithmicStrain1DBehaviourWrapper::getHypothesis(void) const{
    return this->b->getHypothesis();
  } // end of LogarithmicStrain1DBehaviourWrapper::getHypothesis
  
  tfel::material::MechanicalBehaviourBase::BehaviourType
  LogarithmicStrain1DBehaviourWrapper::getBehaviourType(void) const
  {
    return this->b->getBehaviourType();
  } // end of LogarithmicStrain1DBehaviourWrapper::getBehaviourType

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getDrivingVariablesSize() const
  {
    return this->b->getDrivingVariablesSize();
  } // end of LogarithmicStrain1DBehaviourWrapper::getDrivingVariablesSize

  void
  LogarithmicStrain1DBehaviourWrapper::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    return this->b->getDrivingVariablesDefaultInitialValues(v);
  } // end of LogarithmicStrain1DBehaviourWrapper::getDrivingVariablesDefaultInitialValues

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getThermodynamicForcesSize() const
  {
    return this->b->getThermodynamicForcesSize();
  } // end of LogarithmicStrain1DBehaviourWrapper::getThermodynamicForcesSize

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getStensorComponentsSuffixes() const
  {
    return this->b->getStensorComponentsSuffixes();
  } // end of LogarithmicStrain1DBehaviourWrapper::getStensorComponentsSuffixes

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getTensorComponentsSuffixes() const
  {
    return this->b->getTensorComponentsSuffixes();
  } // end of LogarithmicStrain1DBehaviourWrapper::getTensorComponentsSuffixes

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getDrivingVariablesComponents() const
  {
    return this->b->getDrivingVariablesComponents();
  } // end of LogarithmicStrain1DBehaviourWrapper::getDrivingVariablesComponents

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getThermodynamicForcesComponents() const
  {
    return this->b->getThermodynamicForcesComponents();
  } // end of LogarithmicStrain1DBehaviourWrapper::getThermodynamicForcesComponents

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getDrivingVariableComponentPosition(const std::string& c) const
  {
    return this->b->getDrivingVariableComponentPosition(c);
  } // end of LogarithmicStrain1DBehaviourWrapper::getDrivingVariableComponentPosition

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getThermodynamicForceComponentPosition(const std::string& c) const
  {
    return this->b->getThermodynamicForceComponentPosition(c);
  } // end of LogarithmicStrain1DBehaviourWrapper::getThermodynamicForceComponentPosition

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getSymmetryType(void) const
  {
    return this->b->getSymmetryType();
  } // end of LogarithmicStrain1DBehaviourWrapper::getSymmetryType

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getMaterialPropertiesNames(void) const
  {
    return this->b->getMaterialPropertiesNames();
  } // end of LogarithmicStrain1DBehaviourWrapper::getMaterialPropertiesNames

  void
  LogarithmicStrain1DBehaviourWrapper::setOptionalMaterialPropertiesDefaultValues(EvolutionManager& mp,
										  const EvolutionManager& evm) const
  {
    return this->b->setOptionalMaterialPropertiesDefaultValues(mp,evm);
  } // end of LogarithmicStrain1DBehaviourWrapper::setOptionalMaterialPropertiesDefaultValues

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesNames(void) const 
  {
    return this->b->getInternalStateVariablesNames();
  } // end of LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesNames

  size_t
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesSize() const
  {
    return this->b->getInternalStateVariablesSize();
  } // end of LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesSize

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesDescriptions() const
  {
    return this->b->getInternalStateVariablesDescriptions();
  } // end of LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesDescriptions

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariableType(const std::string& n) const
  {
    return this->b->getInternalStateVariableType(n);
  } // end of LogarithmicStrain1DBehaviourWrapper::getInternalStateVariableType

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablePosition(const std::string& n) const
  {
    return this->b->getInternalStateVariablePosition(n);
  } // end of LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablePosition

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getExternalStateVariablesNames(void) const 
  {
    return this->b->getExternalStateVariablesNames();
  } // end of LogarithmicStrain1DBehaviourWrapper::getExternalStateVariablesNames

  void
  LogarithmicStrain1DBehaviourWrapper::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p) const 
  {
    return this->b->setOutOfBoundsPolicy(p);
  } // end of LogarithmicStrain1DBehaviourWrapper::setOutOfBoundsPolicy

  void
  LogarithmicStrain1DBehaviourWrapper::setParameter(const std::string& n,
						    const real v) const 
  {
    this->b->setParameter(n,v);
  } // end of LogarithmicStrain1DBehaviourWrapper::setParameter

  void
  LogarithmicStrain1DBehaviourWrapper::setIntegerParameter(const std::string& n,
							   const int v) const 
  {
    this->b->setIntegerParameter(n,v);
  } // end of LogarithmicStrain1DBehaviourWrapper::setIntegerParameter

  void
  LogarithmicStrain1DBehaviourWrapper::setUnsignedIntegerParameter(const std::string& n,
								   const unsigned short v) const 
  {
    this->b->setUnsignedIntegerParameter(n,v);
  } // end of LogarithmicStrain1DBehaviourWrapper::setUnsignedIntegerParameter

  void
  LogarithmicStrain1DBehaviourWrapper::allocate(BehaviourWorkSpace& wk) const
  {
    this->b->allocate(wk);
  } // end of LogarithmicStrain1DBehaviourWrapper::allocate

  StiffnessMatrixType
  LogarithmicStrain1DBehaviourWrapper::getDefaultStiffnessMatrixType(void) const
  {
    return this->b->getDefaultStiffnessMatrixType();
  } // end of LogarithmicStrain1DBehaviourWrapper::getDefaultStiffnessMatrixType

  tfel::math::tmatrix<3u,3u,real>
  LogarithmicStrain1DBehaviourWrapper::getRotationMatrix(const tfel::math::vector<real>& mp,
							 const tfel::math::tmatrix<3u,3u,real>& r) const
  {
    return this->b->getRotationMatrix(mp,r);
  } // end of LogarithmicStrain1DBehaviourWrapper::getRotationMatrix

  bool
  LogarithmicStrain1DBehaviourWrapper::doPackagingStep(CurrentState& s,
						       BehaviourWorkSpace& wk) const{
    return this->b->doPackagingStep(s,wk);
  } // end of LogarithmicStrain1DBehaviourWrapper::doPackagingStep
  
  std::pair<bool,real>
  LogarithmicStrain1DBehaviourWrapper::computePredictionOperator(BehaviourWorkSpace& wk,
								 const CurrentState& s,
								 const StiffnessMatrixType ktype) const
  {
    auto cs=s;
    // logarithmic strains
    cs.e0[0] = std::log(1+s.e0[0]);
    cs.e0[1] = std::log(1+s.e0[1]);
    cs.e0[2] = std::log(1+s.e0[2]);
    cs.e1[0] = cs.e0[0];
    cs.e1[1] = cs.e0[1];
    cs.e1[2] = cs.e0[2];
    cs.e_th0[0] = std::log(1+s.e_th0[0]);
    cs.e_th0[1] = std::log(1+s.e_th0[1]);
    cs.e_th0[2] = std::log(1+s.e_th0[2]);
    cs.e_th1[0] = std::log(1+s.e_th1[0]);
    cs.e_th1[1] = std::log(1+s.e_th1[1]);
    cs.e_th1[2] = std::log(1+s.e_th1[2]);
    // stresses
    cs.s0[0] = s.s0[0]*(1+s.e0[0]);
    cs.s0[1] = s.s0[1]*(1+s.e0[1]);
    cs.s0[2] = s.s0[2]*(1+s.e0[2]);
    cs.s1[0] = cs.s0[0];
    cs.s1[1] = cs.s0[1];
    cs.s1[2] = cs.s0[2];
    const auto r = this->b->computePredictionOperator(wk,cs,ktype);
    if(!r.first){
      return r;
    }
    // modify wk.kp
    if((ktype!=StiffnessMatrixType::NOSTIFFNESS)&&
       (ktype!=StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE)&&
       (ktype!=StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)){
      convertStiffness(wk.kt,s.e0,s.s0);
    }
    return r;
  } // end of LogarithmicStrain1DBehaviourWrapper::computePredictionOperator

  std::pair<bool,real>
  LogarithmicStrain1DBehaviourWrapper::integrate(CurrentState& s,
						 BehaviourWorkSpace& wk,
						 const real dt,
						 const StiffnessMatrixType ktype) const
  {
    real e0[3];
    real e1[3];
    real e_th0[3];
    real e_th1[3];
    real s0[3];
    // logarithmic strains
    tfel::fsalgo::copy<3u>::exe(s.e0.begin(),e0);
    tfel::fsalgo::copy<3u>::exe(s.e1.begin(),e1);
    tfel::fsalgo::copy<3u>::exe(s.s0.begin(),s0);
    s.e0[0] = std::log(1+e0[0]);
    s.e0[1] = std::log(1+e0[1]);
    s.e0[2] = std::log(1+e0[2]);
    s.e1[0] = std::log(1+e1[0]);
    s.e1[1] = std::log(1+e1[1]);
    s.e1[2] = std::log(1+e1[2]);
    // stresses
    s.s0[0] = s0[0]*(1+e0[0]);
    s.s0[1] = s0[1]*(1+e0[1]);
    s.s0[2] = s0[2]*(1+e0[2]);
    // thermal strain
    tfel::fsalgo::copy<3u>::exe(s.e_th0.begin(),e_th0);
    tfel::fsalgo::copy<3u>::exe(s.e_th1.begin(),e_th1);
    s.e_th0[0] = std::log(1+e_th0[0]);
    s.e_th0[1] = std::log(1+e_th0[1]);
    s.e_th0[2] = std::log(1+e_th0[2]);
    s.e_th1[0] = std::log(1+e_th1[0]);
    s.e_th1[1] = std::log(1+e_th1[1]);
    s.e_th1[2] = std::log(1+e_th1[2]);
    tfel::fsalgo::copy<3u>::exe(s.s0.begin(),s.s1.begin());
    const auto r = this->b->integrate(s,wk,dt,ktype);
    tfel::fsalgo::copy<3u>::exe(e0,s.e0.begin());
    tfel::fsalgo::copy<3u>::exe(e1,s.e1.begin());
    tfel::fsalgo::copy<3u>::exe(s0,s.s0.begin());
    tfel::fsalgo::copy<3u>::exe(e_th0,s.e_th0.begin());
    tfel::fsalgo::copy<3u>::exe(e_th1,s.e_th1.begin());
    if(!r.first){
      tfel::fsalgo::copy<3u>::exe(s0,s.s1.begin());
      return r;
    }
    s.s1[0] /= 1+e1[0];
    s.s1[1] /= 1+e1[1];
    s.s1[2] /= 1+e1[2];
    // modify wk.k
    if((ktype!=StiffnessMatrixType::NOSTIFFNESS)&&
       (ktype!=StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE)&&
       (ktype!=StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)){
      convertStiffness(wk.k,s.e1,s.s1);
    }
    return r;
  } // end of LogarithmicStrain1DBehaviourWrapper::integrate
  
  LogarithmicStrain1DBehaviourWrapper::~LogarithmicStrain1DBehaviourWrapper()
  {} // end of LogarithmicStrain1DBehaviourWrapper::~LogarithmicStrain1DBehaviourWrapper()
  
} // end of namespace mtest