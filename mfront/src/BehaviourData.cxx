/*!
 * \file   mfront/src/BehaviourData.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>
#include<iostream>
#include<iterator>

#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/PerformanceProfiling.hxx"
#include"MFront/BehaviourData.hxx"

namespace mfront{

  const std::string
  BehaviourData::FlowRule("FlowRule");
  const std::string
  BehaviourData::BeforeInitializeLocalVariables("BeforeInitializeLocalVariables");
  const std::string
  BehaviourData::InitializeLocalVariables("InitializeLocalVariables");
  const std::string
  BehaviourData::AfterInitializeLocalVariables("AfterInitializeLocalVariables");
  const std::string
  BehaviourData::ComputePredictor("ComputePredictor");
  const std::string
  BehaviourData::ComputeStress("ComputeStress");
  const std::string
  BehaviourData::ComputeFinalStress("ComputeFinalStress");
  const std::string
  BehaviourData::ComputeFinalStressCandidate("ComputeFinalStressCandidate");
  const std::string
  BehaviourData::Integrator("Integrator");
  const std::string
  BehaviourData::ComputeDerivative("ComputeDerivative");
  const std::string
  BehaviourData::UpdateAuxiliaryStateVariables("UpdateAuxiliaryStateVariables");
  const std::string
  BehaviourData::ComputePredictionOperator("ComputePredictionOperator");
  const std::string
  BehaviourData::ComputeTangentOperator("ComputeTangentOperator");
  const std::string
  BehaviourData::InitializeJacobian("InitializeJacobian");
  const std::string
  BehaviourData::InitializeJacobianInvert("InitializeJacobianInvert");

  const std::string
  BehaviourData::profiling("profiling");
  const std::string
  BehaviourData::hasTimeStepScalingFactor("hasTimeStepScalingFactor");
  const std::string
  BehaviourData::hasConsistentTangentOperator("hasConsistentTangentOperator");
  const std::string
  BehaviourData::isConsistentTangentOperatorSymmetric("isConsistentTangentOperatorSymmetric");
  const std::string
  BehaviourData::hasPredictionOperator("hasPredictionOperator");
  const std::string
  BehaviourData::compareToNumericalJacobian("compareToNumericalJacobian");
  const std::string
  BehaviourData::allowsNewUserDefinedVariables("allowsNewUserDefinedVariables");
  const std::string
  BehaviourData::algorithm("algorithm");
  const std::string
  BehaviourData::numberOfEvaluations("numberOfEvaluations");

  /*!
   * check if the given name has already been used as value in the
   * glossary map or in the entry map
   * \param[in] gm : glossray names
   * \param[in] em : entry names
   * \param[in] n  : glossary name
   */
  static void
  BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& gm,
						       const std::map<std::string,std::string>& em,
						       const std::string& n)
  {
    auto check = [&n](const std::map<std::string,std::string>& m,
		      const char* const t){
      for(const auto& v : m){
	if(v.second==n){
	  throw(std::runtime_error("BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName : "
				   "name '"+n+"' is already used as a "+std::string(t)));
	}
      }
    };
    check(gm,"glossary name");
    check(em,"entry name");
  } // end of BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName

  /*!
   * \brief associate a glossary name or an entry name to a variable
   * \param[in] m  : map to be updated
   * \param[in] gn : glossary names
   * \param[in] en : entry names
   * \param[in] vn  : variable name
   * \param[in] n  : variable name
   * \param[in] g  : glossray name or entry name
   */
  static void
  BehaviourDataAddToGlossaryOrEntryNames(std::map<std::string,std::string>& m,
					 const std::map<std::string,std::string>& gn,
					 const std::map<std::string,std::string>& en,
					 const std::set<std::string>& vn,
					 const std::string& n,
					 const std::string& g)
  {
    BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName(gn,en,g);
    if(en.find(n)!=en.end()){
      throw(std::runtime_error("BehaviourDataAddToGlossaryOrEntryNames : "
			       "an entry name has already been specified for "
			       "variable '"+n+"'"));
    }
    if(gn.find(n)!=gn.end()){
      throw(std::runtime_error("BehaviourDataAddToGlossaryOrEntryNames : "
			       "an glossary name has already been specified for "
			       "variable '"+n+"'"));
    }
    if(n!=g){
      if(vn.find(g)!=vn.end()){
	throw(std::runtime_error("BehaviourDataAddToGlossaryOrEntryNames : "
				 "a member with the '"+g+"' name has already "
				 "been declared"));
      }
    }
    if(!m.insert({n,g}).second){
      throw(std::runtime_error("BehaviourDataAddToGlossaryOrEntryNames : "
			       "glossary name for variable '"+n+"' "
			       "already specified"));
    }
  }
  
  void
  BehaviourData::throwUndefinedAttribute(const std::string& n)
  {
    throw(std::runtime_error("BehaviourData::getAttribute : "
			     "no attribute named '"+n+"'"));
  } // end of BehaviourData::throwUndefinedAttribute
  
  BehaviourData::CodeBlocksAggregator::CodeBlocksAggregator()
    : is_mutable(true)
  {} // end of BehaviourData::CodeBlocksAggregator::CodeBlocksAggregator

  bool
  BehaviourData::CodeBlocksAggregator::isMutable(void) const
  {
    return this->is_mutable;
  } // end of BehaviourData::CodeBlocksAggregator::isMutable

  void
  BehaviourData::CodeBlocksAggregator::update(void)
  {
    // updating code
    this->cblock.code  = cblock_begin;
    if(!this->cblock_body.empty()){
      if(!this->cblock.code.empty()){
	if(*(this->cblock.code.rbegin())!='\n'){
	  this->cblock.code.push_back('\n');
	}
      }
    }
    this->cblock.code += cblock_body;
    if(!this->cblock_end.empty()){
      if(!this->cblock.code.empty()){
	if(*(this->cblock.code.rbegin())!='\n'){
	  this->cblock.code.push_back('\n');
	}
      }
    }
    this->cblock.code += cblock_end;
    // updating description
    this->cblock.description  = cdoc_begin;
    if(!this->cdoc_body.empty()){
      if(!this->cblock.description.empty()){
	if(*(this->cblock.description.rbegin())!='\n'){
	  this->cblock.description.push_back('\n');
	}
      }
    }
    this->cblock.description += cdoc_body;
    if(!this->cdoc_end.empty()){
      if(!this->cblock.description.empty()){
	if(*(this->cblock.description.rbegin())!='\n'){
	  this->cblock.description.push_back('\n');
	}
      }
    }
    this->cblock.description += cdoc_end;
  } // end of BehaviourData::update

  void
  BehaviourData::CodeBlocksAggregator::set(const CodeBlock& c,
					   const Position p,
					   const bool b)
  {
    this->check();
    this->cblock.staticMembers.insert(c.staticMembers.begin(),
				      c.staticMembers.end());
    this->cblock.members.insert(c.members.begin(),
				c.members.end());
    switch(p){
    case AT_BEGINNING:
      if(!this->cblock_begin.empty()){
	this->cblock_begin += '\n';
      }
      this->cblock_begin += c.code;
      if(!c.description.empty()){
	if(!this->cdoc_begin.empty()){
	  this->cdoc_begin += '\n';
	}
	this->cdoc_begin += c.description;
      }
      break;
    case BODY:
      if(!this->cblock_body.empty()){
	this->cblock_body += '\n';
      }
      this->cblock_body  += c.code;
      if(!c.description.empty()){
	if(!this->cdoc_body.empty()){
	  this->cdoc_body += '\n';
	}
	this->cdoc_body += c.description;
      }
      break;
    case AT_END:
      if(!this->cblock_end.empty()){
	this->cblock_end += '\n';
      }
      this->cblock_end   += c.code;
      if(!c.description.empty()){
	if(!this->cdoc_end.empty()){
	  this->cdoc_end += '\n';
	}
	this->cdoc_end += c.description;
      }
      break; 
    }
    this->update();
    this->is_mutable = b;
  } // end of BehaviourData::CodeBlocksAggregator::set

  void
  BehaviourData::CodeBlocksAggregator::replace(const CodeBlock& c,
					       const Position p,
					       const bool b)
  {
    this->check();
    this->cblock_begin.clear();
    this->cblock_body.clear();
    this->cblock_end.clear();
    this->cblock.code.clear();
    this->cblock.members.clear();
    this->cblock.staticMembers.clear();
    this->set(c,p,b);
  } // end of BehaviourData::CodeBlocksAggregator::set

  void
  BehaviourData::CodeBlocksAggregator::check(void) const
  {
    if(!this->is_mutable){
      throw(std::runtime_error("BehaviourData::CodeBlocksAggregator::set : "
			       "can't modifiy a code block"));
    }
  } // end of BehaviourData::CodeBlocksAggregator::check

  const CodeBlock&
  BehaviourData::CodeBlocksAggregator::get(void) const
  {
    this->is_mutable = false;
    return this->cblock;
  } // end of BehaviourData::CodeBlocksAggregator::get

  BehaviourData::CodeBlocksAggregator::~CodeBlocksAggregator()
  {} // end of BehaviourData::CodeBlocksAggregator::~CodeBlocksAggregator()

  BehaviourData::BehaviourData()
    : usableInPurelyImplicitResolution(false)
  {
    this->registerMemberName("T");
    this->registerMemberName("dT");
    this->registerMemberName("dt");
    this->glossaryNames.insert({"T","Temperature"});
  } // end of BehaviourData::BehaviourData()

  BehaviourData::BehaviourData(const BehaviourData&) = default;

  static void
  checkAlreadyRegistred(const std::set<std::string>& r,
			const std::string& n){
    if(r.find(n)==r.end()){
      throw(std::runtime_error("checkAlreadyRegistred: "
			       "variable '"+n+"' was declared "
			       "a 'already registred' but is not"));
    }
  }
  
  void
  BehaviourData::addStaticVariable(const StaticVariableDescription& v,
				   const RegistrationStatus s)
  {
    if(s==UNREGISTRED){
      this->registerStaticMemberName(v.name);
    } else {
      checkAlreadyRegistred(this->reservedNames,v.name);
    }
    this->staticVariables.push_back(v);
  } // end of BehaviourData::addStaticVariable

  const StaticVariableDescriptionContainer&
  BehaviourData::getStaticVariables(void) const
  {
    return this->staticVariables;
  } // end of BehaviourData::getStaticVariables

  const std::vector<BoundsDescription>&
  BehaviourData::getBounds(void) const
  {
    return this->bounds;
  } // end of BehaviourData::getBoundsDescriptions

  const VariableDescription&
  BehaviourData::getPersistentVariableDescription(const std::string& v) const
  {
    return this->getPersistentVariables().getVariable(v);
  } // end of BehaviourData::getPersistentVariableDescription

  const VariableDescription&
  BehaviourData::getIntegrationVariableDescription(const std::string& v) const
  {
    return this->getIntegrationVariables().getVariable(v);
  } // end of BehaviourData::getIntegrationVariableDescription

  const VariableDescription&
  BehaviourData::getStateVariableDescription(const std::string& v) const
  {
    return this->getStateVariables().getVariable(v);
  } // end of BehaviourData::getStateVariableDescription

  void
  BehaviourData::addMaterialProperty(const VariableDescription& v,
				   const RegistrationStatus s)
  {
    this->addVariable(this->materialProperties,v,s,false);
  } // end of BehaviourData::addMaterialProperty

  void
  BehaviourData::addIntegrationVariable(const VariableDescription& v,
					const RegistrationStatus s)
  {
    this->addVariable(this->integrationVariables,v,s,true);
  } // end of BehaviourData::addIntegrationVariable

  void
  BehaviourData::addStateVariable(const VariableDescription& v,
				  const RegistrationStatus s)
  {
    this->addVariable(this->stateVariables,v,s,true);
    this->addVariable(this->integrationVariables,v,
		      ALREADYREGISTRED,true);
    /*!
     * for compatibility reasons with previous mfront versions
     * (<2.0), auxiliary state variables shall be put after
     * state variables.
     */
    auto found = false;
    auto p=this->persistentVariables.begin();
    while((p!=this->persistentVariables.end())&&(!found)){
      if(this->isAuxiliaryStateVariableName(p->name)){
	this->persistentVariables.insert(p,v);
	found = true;
      } else {
	++p;
      }
    }
    if(!found){
      this->persistentVariables.push_back(v);
    }
  } // end of BehaviourData::addStateVariable

  void
  BehaviourData::addAuxiliaryStateVariable(const VariableDescription& v,
					   const RegistrationStatus s)
  {
    this->addVariable(this->auxiliaryStateVariables,v,s,false);
    this->addVariable(this->persistentVariables,v,
		      ALREADYREGISTRED,true);
  } // end of BehaviourData::addAuxiliaryStateVariable

  void
  BehaviourData::addExternalStateVariable(const VariableDescription& v,
					   const RegistrationStatus s)
  {
    this->addVariable(this->externalStateVariables,v,s,true);
  } // end of BehaviourData::addExternalStateVariable

  void
  BehaviourData::addLocalVariable(const VariableDescription& v,
				  const RegistrationStatus s)
  {
    this->addVariable(this->localVariables,v,s,false);
  } // end of BehaviourData::addLocalVariable

  void
  BehaviourData::addParameter(const VariableDescription& v,
			      const RegistrationStatus s)
  {
    this->addVariable(this->parameters,v,s,false);
  } // end of BehaviourData::addParameter

  bool
  BehaviourData::hasParameter(const std::string& n) const
  {
    return this->parameters.contains(n);
  }

  bool
  BehaviourData::hasParameters(void) const
  {
    return !this->parameters.empty();
  }

  bool
  BehaviourData::isMemberUsedInCodeBlocks(const std::string& v) const
  {
    for(const auto& c : this->cblocks){
      const auto& members = c.second.get().members;
      if(members.find(v)!=members.end()){
	return true;
      }
    }
    return false;
  } // end of BehaviourData::isMemberUsedInCodeBlocks

  bool
  BehaviourData::isMaterialPropertyName(const std::string& n) const
  {
    return this->getMaterialProperties().contains(n);
  } // end of BehaviourData::isMaterialPropertyName

  bool
  BehaviourData::isStaticVariableName(const std::string& n) const
  {
    return this->getStaticVariables().contains(n);
  } // end of BehaviourData::isStaticVariableName

  bool
  BehaviourData::isLocalVariableName(const std::string& n) const
  {
    return this->getLocalVariables().contains(n);
  } // end of BehaviourData::isLocalVariableName

  bool
  BehaviourData::isPersistentVariableName(const std::string& n) const
  {
    return this->getPersistentVariables().contains(n);
  } // end of BehaviourData::isPersistentVariableName

  bool
  BehaviourData::isIntegrationVariableName(const std::string& n) const
  {
    return this->getIntegrationVariables().contains(n);
  } // end of BehaviourData::isIntegrationVariableName

  bool
  BehaviourData::isIntegrationVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getIntegrationVariables().contains(n.substr(1));
  } // end of BehaviourData::isIntegrationVariableName

  bool
  BehaviourData::isStateVariableName(const std::string& n) const
  {
    return this->getStateVariables().contains(n);
  } // end of BehaviourData::isStateVariableName

  bool
  BehaviourData::isStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getStateVariables().contains(n.substr(1));
  } // end of BehaviourData::isStateVariableName

  bool
  BehaviourData::isAuxiliaryStateVariableName(const std::string& n) const
  {
    return this->getAuxiliaryStateVariables().contains(n);
  } // end of BehaviourData::isStateVariableName
  
  bool
  BehaviourData::isExternalStateVariableName(const std::string& n) const
  {
    return this->getExternalStateVariables().contains(n);
  } // end of BehaviourData::isExternalStateVariableName

  bool
  BehaviourData::isExternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getExternalStateVariables().contains(n.substr(1));
  } // end of BehaviourData::isExternalStateVariableName

  bool
  BehaviourData::isParameterName(const std::string& n) const
  {
    return this->getParameters().contains(n);
  } // end of BehaviourData::isParameterName

  const VariableDescriptionContainer&
  BehaviourData::getMaterialProperties(void) const
  {
    return this->materialProperties;
  } // end of BehaviourData::getMaterialProperties

  const VariableDescriptionContainer&
  BehaviourData::getPersistentVariables(void) const
  {
    return this->persistentVariables;
  } // end of BehaviourData::getPersistentVariables

  std::set<std::string>
  BehaviourData::getVariablesNames(void) const{
    auto getNames = [](std::set<std::string>& r,
		       const VariableDescriptionContainer& c){
      for(const auto& v : c){
	if(!r.insert(v.name).second){
	  throw(std::runtime_error("BehaviourData::getVariablesNames: "
				   "internal error, variable name '"+
				   v.name+"' multiply defined"));
	}
      }
    };
    auto n = std::set<std::string>{};
    getNames(n,this->getMaterialProperties());
    getNames(n,this->getStateVariables());
    getNames(n,this->getAuxiliaryStateVariables());
    getNames(n,this->getExternalStateVariables());
    getNames(n,this->getParameters());
    return n;
  } // end of BehaviourData::getVariablesNames

  
  const VariableDescriptionContainer&
  BehaviourData::getVariables(const std::string& t) const
  {
    using namespace std;
    const VariableDescriptionContainer& (BehaviourData::* m)(void) const;
    if(t=="MaterialProperty"){
      m = &BehaviourData::getMaterialProperties;
    } else if(t=="PersistentVariable"){
      m = &BehaviourData::getPersistentVariables;
    } else if(t=="IntegrationVariable"){
      m = &BehaviourData::getIntegrationVariables;
    } else if(t=="StateVariable"){
      m = &BehaviourData::getStateVariables;
    } else if(t=="AuxiliaryStateVariable"){
      m = &BehaviourData::getAuxiliaryStateVariables;
    } else if(t=="ExternalStateVariable"){
      m = &BehaviourData::getExternalStateVariables;
    } else if(t=="Parameter"){
      m = &BehaviourData::getParameters;
    } else {
      string msg("BehaviourData::getVariables : "
		 "invalid variables type '"+t+"'");
      throw(runtime_error(msg));
    }
    return (this->*m)();
  } // end of BehaviourData::getIntegrationVariables

  const VariableDescriptionContainer&
  BehaviourData::getIntegrationVariables(void) const
  {
    return this->integrationVariables;
  } // end of BehaviourData::getIntegrationVariables

  const VariableDescriptionContainer&
  BehaviourData::getStateVariables(void) const
  {
    return this->stateVariables;
  } // end of BehaviourData::getStateVariables

  const VariableDescriptionContainer&
  BehaviourData::getAuxiliaryStateVariables(void) const
  {
    return this->auxiliaryStateVariables;
  } // end of BehaviourData::getAuxiliaryStateVariables

  const VariableDescriptionContainer&
  BehaviourData::getExternalStateVariables(void) const
  {
    return this->externalStateVariables;
  } // end of BehaviourData::getExternalStateVariables

  const VariableDescriptionContainer&
  BehaviourData::getLocalVariables(void) const
  {
    return this->localVariables;
  } // end of BehaviourData::getLocalVariables
  
  bool
  BehaviourData::isUsableInPurelyImplicitResolution(void) const
  {
    return this->usableInPurelyImplicitResolution;
  } // end of BehaviourData::isUsableInPurelyImplicitResolution

  void
  BehaviourData::setUsableInPurelyImplicitResolution(const bool b)
  {
    this->usableInPurelyImplicitResolution = b;
  } // end of BehaviourData::setUsableInPurelyImplicitResolution

  void
  BehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    this->pupirv.insert(n);
  } // end of BehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  const std::set<std::string>&
  BehaviourData::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const
  {
    return this->pupirv;
  } // end of BehaviourData::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution

  void
  BehaviourData::requiresTVectorOrVectorIncludes(bool& b1,
							   bool& b2) const
  {
    VariableDescriptionContainer::const_iterator ps;
    b1 = false;
    b2 = false;
    for(ps=this->getMaterialProperties().begin();
	(ps!=this->getMaterialProperties().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getIntegrationVariables().begin();
	(ps!=this->getIntegrationVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getStateVariables().begin();
	(ps!=this->getStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getAuxiliaryStateVariables().begin();
	(ps!=this->getAuxiliaryStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getLocalVariables().begin();
	(ps!=this->getLocalVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->getExternalStateVariables().begin();
	(ps!=this->getExternalStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
  } // end of BehaviourData::requiresTVectorOrVectorIncludes

  void
  BehaviourData::addVariable(VariableDescriptionContainer& c,
			     const VariableDescription& v,
			     const RegistrationStatus s,
			     const bool bi)
  {
    if(s==ALREADYREGISTRED){
      checkAlreadyRegistred(this->reservedNames,v.name);
    } else {
      this->registerMemberName(v.name);
      if(bi){
	this->registerMemberName("d"+v.name);
      }
    }
    c.push_back(v);
  } // end of BehaviourData::addVariable

  void
  BehaviourData::reserveName(const std::string& n)
  {
    if(!this->reservedNames.insert(n).second){
      throw(std::runtime_error("BehaviourData::reserveName: "
			       "name '"+n+"' already registred"));
    }
  } // end of BehaviourData::reserveName
  
  void
  BehaviourData::registerMemberName(const std::string& n)
  {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    for(auto& e : this->entryNames){
      if(e.second==n){
	throw(std::runtime_error("BehaviourData::registerMemberName : "
				 "the name '"+n+"' is already been used "
				 "for an entry name"));
      }
    }
    if(glossary.contains(n)){
      std::ostringstream msg;
      msg << "BehaviourData::registerMemberName : "
	  << "the name '" << n << "' is a registred as a glossary name.\n";
      displayGlossaryEntryCompleteDescription(msg,glossary.getGlossaryEntry(n));
      throw(std::runtime_error(msg.str()));
    }
    this->reserveName(n);
    if(!this->membersNames.insert(n).second){
      throw(std::runtime_error("BehaviourData::registerMemberName : "
			       "a variable named '"+n+"' has already been registred"));
    }
  } // end of BehaviourData::registerMemberName

  void
  BehaviourData::registerStaticMemberName(const std::string& n)
  {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    for(auto& e : this->entryNames){
      if(e.second==n){
	throw(std::runtime_error("BehaviourData::registerStaticMemberName : "
				 "the name '"+n+"' is already been used "
				 "for an entry name"));
      }
    }
    if(glossary.contains(n)){
      std::ostringstream msg;
      msg << "BehaviourData::registerStaticMemberName : "
	  << "the name '" << n << "' is a registred as a glossary name.\n";
      displayGlossaryEntryCompleteDescription(msg,glossary.getGlossaryEntry(n));
      throw(std::runtime_error(msg.str()));
    }
    this->reserveName(n);
    if(!this->staticMembersNames.insert(n).second){
      throw(std::runtime_error("BehaviourData::registerStaticMemberName : "
			  "a variable named '"+n+"' has already been registred"));
    }
  } // end of BehaviourData::registerStaticMemberName
  
  const std::set<std::string>&
  BehaviourData::getRegistredMembersNames(void) const
  {
    return this->membersNames;
  } // end of BehaviourData::getRegistredMemberNames

  const std::set<std::string>&
  BehaviourData::getRegistredStaticMembersNames(void) const
  {
    return this->staticMembersNames;
  } // end of BehaviourData::getRegistredStaticMemberNames
  
  void
  BehaviourData::checkVariableName(const std::string& n) const
  {
    if((this->materialProperties.contains(n))||
       (this->persistentVariables.contains(n))||
       (this->integrationVariables.contains(n))||
       (this->stateVariables.contains(n))||
       (this->auxiliaryStateVariables.contains(n))||
       (this->externalStateVariables.contains(n))||
       (this->localVariables.contains(n))||
       (this->parameters.contains(n))||
       (this->staticVariables.contains(n))){
      return;
    }
    throw(std::runtime_error("BehaviourData::checkVariableName : "
			     "no variable named '"+n+"'"));
  } // end of BehaviourData::checkVariableName

  void
  BehaviourData::setCode(const std::string& n,
			 const CodeBlock& c,
			 const Mode m,
			 const Position p,
			 const bool b)
  {
    using namespace std;
    auto pc = this->cblocks.find(n);
    if(pc==this->cblocks.end()){
      pc = this->cblocks.insert({n,CodeBlocksAggregator{}}).first;
    } else {
      if(m==CREATE){
	string msg("BehaviourData::setCode : "
		   "a code block named '"+n+"' already exists.\n"
		   "If you wanted to append this new code to the "
		   "existing one, you shall use the 'Append' option.\n"
		   "You can also replace it with 'Replace' option "
		   "(assuming you know what you are doing).\n");
	throw(runtime_error(msg));
      } else if(m==CREATEORREPLACE){
	if(!pc->second.isMutable()){
	  string msg("BehaviourData::setCode : "
		     "the code block named '"+n+"' is not modifiable");
	  throw(runtime_error(msg));
	}
	this->cblocks.erase(pc);
	pc = this->cblocks.insert({n,CodeBlocksAggregator{}}).first;
      } else if(m==CREATEBUTDONTREPLACE){
	return;
      }
    }
    pc->second.set(c,p,b);
  } // end of BehaviourData::setCode


  const CodeBlock&
  BehaviourData::getCodeBlock(const std::string& n) const
  {
    using namespace std;
    auto p = this->cblocks.find(n);
    if(p==this->cblocks.end()){
      string msg("BehaviourData::getCode : "
		 "no code block associated with '"+n+"'");
      throw(runtime_error(msg));
    }
    return p->second.get();
  } // end of BehaviourData::getCodeBlock

  std::string
  BehaviourData::getCode(const std::string& n,
				   const std::string& cn,
				   const bool b) const
  {
    if(!b){
      return this->getCodeBlock(n).code;
    }
    std::ostringstream out;
    writeStandardPerformanceProfilingBegin(out,cn,n);
    out << this->getCodeBlock(n).code;
    writeStandardPerformanceProfilingEnd(out);
    return out.str();
  } // end of BehaviourData::getCode

  bool
  BehaviourData::hasCode(const std::string& n) const
  {
    return this->cblocks.find(n)!=this->cblocks.end();
  }

  void
  BehaviourData::setBounds(const BoundsDescription& d)
  {
    this->bounds.push_back(d);
  } // end of BehaviourData::setBound

  void
  BehaviourData::setParameterDefaultValue(const std::string& n,
						    const double v)
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    const auto& p = this->parameters.getVariable(n);
    if(p.type!="real"){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "parameter '"+n+"' is not a floatting point");
      throw(runtime_error(msg));
    }
    if(!this->parametersDefaultValues.insert({n,v}).second){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "default value for parameter '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  }

  void
  BehaviourData::setParameterDefaultValue(const std::string& n,
						    const int v)
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    const auto& p = this->parameters.getVariable(n);
    if(p.type!="int"){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "parameter '"+n+"' is not an integer");
      throw(runtime_error(msg));
    }
    if(!this->iParametersDefaultValues.insert({n,v}).second){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "default value for parameter '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  }

  void
  BehaviourData::setParameterDefaultValue(const std::string& n,
					  const unsigned short v)
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    const auto& p = this->parameters.getVariable(n);
    if(p.type!="ushort"){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "parameter '"+n+"' is not an unsigned short integer");
      throw(runtime_error(msg));
    }
    if(!this->uParametersDefaultValues.insert({n,v}).second){
      string msg("BehaviourData::setParameterDefaultValue : "
		 "default value for parameter '"+n+"' already defined");
      throw(runtime_error(msg));
    }
  }

  double
  BehaviourData::getFloattingPointParameterDefaultValue(const std::string& n) const
  {
    using namespace std;
    if(!this->parameters.contains(n)){
      string msg("BehaviourData::getFloattingPointParameterDefaultValue : "
		 "no parameter '"+n+"' defined");
      throw(runtime_error(msg));
    }
    map<string,double>::const_iterator p = this->parametersDefaultValues.find(n);
    if(p==this->parametersDefaultValues.end()){
      string msg("BehaviourData::getFloattingPointParameterDefaultValue : "
		 "no default value defined for parameter '"+n+"'");
      throw(runtime_error(msg));
    }
    return p->second;
  } // end of BehaviourData::getFloattingPointParameterDefaultValue

  int
  BehaviourData::getIntegerParameterDefaultValue(const std::string& n) const
  {
    if(!this->parameters.contains(n)){
      throw(std::runtime_error("BehaviourData::getIntegerParameterDefaultValue : "
			       "no parameter '"+n+"' defined"));
    }
    auto p = this->iParametersDefaultValues.find(n);
    if(p==this->iParametersDefaultValues.end()){
      throw(std::runtime_error("BehaviourData::getIntegerParameterDefaultValue : "
			       "no default value defined for parameter '"+n+"'"));
    }
    return p->second;
  } // end of BehaviourData::getIntegerParameterDefaultValue

  unsigned short
  BehaviourData::getUnsignedShortParameterDefaultValue(const std::string& n) const
  {
    if(!this->parameters.contains(n)){
      throw(std::runtime_error("BehaviourData::getUnsignedShortParameterDefaultValue : "
			       "no parameter '"+n+"' defined"));
    }
    auto p = this->uParametersDefaultValues.find(n);
    if(p==this->uParametersDefaultValues.end()){
      throw(std::runtime_error("BehaviourData::getUnsignedShortParameterDefaultValue : "
			       "no default value defined for parameter '"+n+"'"));
    }
    return p->second;
  } // end of BehaviourData::getUnsignedShortParameterDefaultValue

  void
  BehaviourData::setAttribute(const std::string& n,
					const BehaviourAttribute& a,
					const bool b)
  {
    using namespace std;
    if(b){
      auto p=this->attributes.find(n);
      if(p!=this->attributes.end()){
	if(a.getTypeIndex()!=p->second.getTypeIndex()){
	  string msg("BehaviourData::setAttribute : ",
		     "attribute already exists with a different type");
	  throw(runtime_error(msg));
	}
	return;
      }
    }
    if(!this->attributes.insert({n,a}).second){
      throw(runtime_error("BehaviourData::setAttribute : "
			  "attribute '"+n+"' already declared"));
    }
  } // end of BehaviourData::setAttribute

  bool
  BehaviourData::hasAttribute(const std::string& n) const
  {
    return this->attributes.count(n)!=0u;
  } // end of BehaviourData::hasAttribute
  
  const std::map<std::string,BehaviourAttribute>&
  BehaviourData::getAttributes() const
  {
    return this->attributes;
  } // end of BehaviourData::getAttributes

  std::vector<std::string>
  BehaviourData::getCodeBlockNames() const
  {
    auto names = std::vector<std::string>{};
    for(const auto c : this->cblocks){
      names.push_back(c.first);
    }
    return names;
  } // end of BehaviourData::getCodeBlockNames

  bool
  BehaviourData::hasGlossaryName(const std::string& n) const
  {
    this->checkVariableName(n);
    return this->glossaryNames.find(n)!=this->glossaryNames.end();
  } // end of BehaviourData::hasGlossaryName

  bool
  BehaviourData::hasEntryName(const std::string& n) const
  {
    this->checkVariableName(n);
    return this->entryNames.find(n)!=this->entryNames.end();
  } // end of BehaviourData::hasEntryName

  std::string
  BehaviourData::getExternalName(const std::string& n) const
  {
    this->checkVariableName(n);
    auto p=this->glossaryNames.find(n);
    if(p!=this->glossaryNames.end()){
      return p->second;
    }
    p=this->entryNames.find(n);
    if(p!=this->entryNames.end()){
      return p->second;
    }
    return n;
  } // end of BehaviourData::getExternalNames

  std::vector<std::string>
  BehaviourData::getExternalNames(const VarContainer& v) const
  {
    return v.getExternalNames(this->glossaryNames,this->entryNames);
  } // end of BehaviourData::getExternalNames
  
  void
  BehaviourData::getExternalNames(std::vector<std::string>& names,
				  const VarContainer& v) const
  {
    v.getExternalNames(names,this->glossaryNames,this->entryNames);
  } // end of BehaviourData::getExternalNames
  
  void
  BehaviourData::appendExternalNames(std::vector<std::string>& names,
				     const VarContainer& v) const
  {
    v.appendExternalNames(names,this->glossaryNames,
			  this->entryNames);
  } // end of BehaviourData::appendExternalNames
  
  void
  BehaviourData::setGlossaryName(const std::string& n,
				 const std::string& g)
  {
    using tfel::glossary::Glossary;
    const auto& glossary = Glossary::getGlossary();
    if(!glossary.contains(g)){
      throw(std::runtime_error("BehaviourData::setGlossaryName : "
			       "'"+g+"' is not a glossary name"));
    }
    this->checkVariableName(n);
    BehaviourDataAddToGlossaryOrEntryNames(this->glossaryNames,
					   this->glossaryNames,
					   this->entryNames,
					   this->reservedNames,n,
					   glossary.getGlossaryEntry(g).getKey());
  } // end of BehaviourData::addGlossaryName

  bool
  BehaviourData::isGlossaryNameUsed(const std::string& n) const
  {
    using namespace tfel::glossary;
    const auto& g = Glossary::getGlossary();
    if(!g.contains(n)){
      throw(std::runtime_error("BehaviourData::isGlossaryNameUsed: "
			       "'"+n+"' is not a glossary name"));
    }
    for(const auto& gn : this->glossaryNames){
      if(gn.second==n){
	return true;
      }
    }
    return false;
  } // end of BehaviourData::isGlossaryName
  
  void
  BehaviourData::setEntryName(const std::string& n,
			      const std::string& e)
  {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    this->checkVariableName(n);
    if(glossary.contains(e)){
      std::ostringstream msg;
      msg << "BehaviourData::setEntryName : "
	  << "'" << e << "' is a glossary name. " << std::endl
	  << "Please use 'setGlossaryName' method instead or choose another entry name.";
      displayGlossaryEntryCompleteDescription(msg,glossary.getGlossaryEntry(e));
      throw(std::runtime_error(msg.str()));
    }
    BehaviourDataAddToGlossaryOrEntryNames(this->entryNames,
					   this->glossaryNames,
					   this->entryNames,
					   this->reservedNames,n,e);
  } // end of BehaviourData::addEntryName

  bool
  BehaviourData::isUsedAsEntryName(const std::string& n) const
  {
    for(const auto& en : this->entryNames){
      if(en.second==n){
	return true;
      }
    }
    return false;
  } // end of BehaviourData::isEntryName

  std::string
  BehaviourData::getVariableNameFromGlossaryNameOrEntryName(const std::string& n) const
  {
    for(const auto& e : this->glossaryNames){
      if(e.second==n){
	return e.first;
      }
    }
    for(const auto& e : this->entryNames){
      if(e.second==n){
	return e.first;
      }
    }
    throw(std::runtime_error("BehaviourData::getVariableNameFromGlossaryNameOrEntryName : "
			     "no variable with glossary or entry name '"+n+"'"));
  } // end of BehaviourData::getVariableNameFromGlossaryNameOrEntryName

  const VariableDescriptionContainer&
  BehaviourData::getParameters(void) const
  {
    return this->parameters;
  } // end of BehaviourData::getParameters

  BehaviourData::~BehaviourData()
  {} // end of BehaviourData::~BehaviourData

} // end of namespace mfront
