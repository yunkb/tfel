/*! 
 * \file  MFrontPowellDogLegAlgorithmBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 */

#include<iostream>
#include<sstream>

#include"MFront/MechanicalBehaviourDescription.hxx"
#include"MFront/MFrontNonLinearSystemSolverBase.hxx"
#include"MFront/MFrontPowellDogLegAlgorithmBase.hxx"

namespace mfront{

  std::vector<std::string>
  MFrontPowellDogLegAlgorithmBase::getReservedNames(void)
  {
    using namespace std;
    vector<string> n;
    n.push_back("pdl_g");
    n.push_back("pdl_g2");
    n.push_back("pdl_0");
    n.push_back("pdl_1");
    n.push_back("pdl_2");
    n.push_back("pdl_3");
    n.push_back("pdl_alpha");
    n.push_back("pdl_cste");
    n.push_back("powell_dogleg_trust_region_size_inv");
    return n;
  } // end of MFrontPowellDogLegAlgorithmBase::getReservedNames

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontPowellDogLegAlgorithmBase::treatSpecificKeywords(MechanicalBehaviourDescription& mb,
							 const std::string& key,
							 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
							 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator pe)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    const ModellingHypothesis::Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(key=="@PowellDogLegTrustRegionSize"){
      CxxTokenizer::TokensContainer::const_iterator current = p;
      CxxTokenizer::checkNotEndOfLine("MFrontPowellDogLegAlgorithmBase::treatSpecificKeywords",current,pe);
      double pdl_trs;
      CxxTokenizer::checkNotEndOfLine("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize"
				      "Cannot read pdl_trs value.",current,pe);
      istringstream flux(current->value);
      flux >> pdl_trs;
      if((flux.fail())||(!flux.eof())){
	string msg("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",
		   "Failed to read region size.");
	throw(runtime_error(msg));
      }
      if(pdl_trs<0){
	string msg("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",
		   "Region size must be positive.");
	throw(runtime_error(msg));
      }
      ++current;
      CxxTokenizer::readSpecifiedToken("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",";",current,pe);
      mb.addParameter(h,VariableDescription("real","powell_dogleg_trust_region_size",1u,0u));
      mb.setParameterDefaultValue(h,"powell_dogleg_trust_region_size",pdl_trs);
      return make_pair(true,current);
    }
    return make_pair(false,p);
  } // end of MFrontPowellDogLegAlgorithmBase::treatSpecificKeywords


  void
  MFrontPowellDogLegAlgorithmBase::endsInputFileProcessing(MechanicalBehaviourDescription& mb)
  {
    using namespace tfel::material;
    const ModellingHypothesis::Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!mb.hasParameter(h,"powell_dogleg_trust_region_size")){
      mb.addParameter(h,VariableDescription("real","powell_dogleg_trust_region_size",1u,0u));
      mb.setParameterDefaultValue(h,"powell_dogleg_trust_region_size",1.e-4);
    }
  } // end of MFrontPowellDogLegAlgorithmBase::endsInputFileProcessing
  
  void
  MFrontPowellDogLegAlgorithmBase::writePowellDogLegStep(std::ostream& out,
							 const MechanicalBehaviourDescription& mb,
							 const tfel::material::ModellingHypothesis::Hypothesis h,
							 const std::string& B,
							 const std::string& f,
							 const std::string& pn)
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n += mb.getTypeSize(p->type,p->arraySize);
    }
    out << "if(abs(" << pn<< ")<(" << n << ")*(this->powell_dogleg_trust_region_size)){" << endl;
    out << "// using the newton method only" << endl;
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValues(out,mb,h,pn);
    out << "this->zeros -= " << pn<< ";\n";
    out << "} else { " << endl;
    out << "// computing the steepest descent step\n";
    out << "tvector<" << n << ",real> pdl_g;\n";
    out << "tvector<" << n << ",real> pdl_g2;\n";
    out << "for(unsigned short idx=0;idx!=" << n << ";++idx){" << endl;
    out << "pdl_g[idx]=real(0);\n";
    out << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){" << endl;
    out << "pdl_g[idx] += (" << B << "(idx2,idx)) * (" << f << "(idx2));\n";
    out << "}" << endl;
    out << "}" << endl;
    out << "for(unsigned short idx=0;idx!=" << n << ";++idx){" << endl;
    out << "pdl_g2[idx]=real(0);\n";
    out << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){" << endl;
    out << "pdl_g2[idx] += (" << B << "(idx,idx2)) * pdl_g(idx2);\n";
    out << "}" << endl;
    out << "}" << endl;
    out << "const real pdl_cste = (pdl_g|pdl_g)/(pdl_g2|pdl_g2);" << endl;
    out << "pdl_g *= pdl_cste;" << endl;
    out << "if(abs(pdl_g)<(" << n << ")*(this->powell_dogleg_trust_region_size)){" << endl;
    out << "const real pdl_0 = (this->powell_dogleg_trust_region_size)*(this->powell_dogleg_trust_region_size);" << endl;
    out << "const real pdl_1 = (pdl_g|pdl_g);" << endl;
    out << "const real pdl_2 = ((" << pn << ")|pdl_g);" << endl;
    out << "const real pdl_3 = ((" << pn << ")|(" << pn << "));" << endl;
    out << "const real pdl_alpha = "
			<< "(pdl_0-pdl_1)/((pdl_2-pdl_1)+sqrt(max((pdl_2-pdl_0)*(pdl_2-pdl_0)+(pdl_3-pdl_0)*(pdl_0-pdl_1),real(0))));" << endl;
    out << "pdl_g = pdl_alpha*(" << pn<< ") + (1-pdl_alpha)*pdl_g;" << endl;
    out << "} else {" << endl;
    out << "const real pdl_alpha = (this->powell_dogleg_trust_region_size)/(norm(pdl_g));" << endl;
    out << "pdl_g *= pdl_alpha;" << endl;
    out << "}" << endl;
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValues(out,mb,h,"pdl_g");
    out << "this->zeros -= pdl_g;\n";
    out << "}" << endl;
  } // end of MFrontImplicitParserBase::writePowellDogLegStep

  MFrontPowellDogLegAlgorithmBase::~MFrontPowellDogLegAlgorithmBase()
  {} // end o fMFrontPowellDogLegAlgorithmBase::~MFrontPowellDogLegAlgorithmBase

}
