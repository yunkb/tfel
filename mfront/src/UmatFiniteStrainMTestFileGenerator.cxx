/*! 
 * \file   mfront/src/UmatFiniteStrainMTestFileGenerator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<ostream>
#include<stdexcept>
#include<algorithm>

#include"MFront/UmatFiniteStrainMTestFileGenerator.hxx"

namespace mfront
{

  UmatFiniteStrainMTestFileGenerator::UmatFiniteStrainMTestFileGenerator(const std::string& i,
									 const std::string& l,
									 const std::string& b)
    : interface(i),
      library(l),
      behaviour(b)
  {
    std::fill(this->F0,this->F0+36,0.);
    std::fill(this->F1,this->F1+36,0.);
    std::fill(this->stress,this->stress+6,0.);
  } // end of UmatFiniteStrainMTestFileGenerator::UmatFiniteStrainMTestFileGenerator

  void
  UmatFiniteStrainMTestFileGenerator::writeBehaviourDeclaration(std::ostream& os) const
  {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".dll' '"
       << this->behaviour << "';" << std::endl;
#else
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".so' '"
       << this->behaviour << "';" << std::endl;
#endif
  } // end of UmatFiniteStrainMTestFileGenerator::writeBehaviourDeclaration

  void
  UmatFiniteStrainMTestFileGenerator::setDeformationGradientAtTheBeginningOfTheStimeStep(const double* const F)
  {
    this->F0[0] = F[0];
    this->F0[1] = F[4];
    this->F0[2] = F[8];
    if(this->getTensorSize()>=5u){
      this->F0[3] = F[3];
      this->F0[4] = F[1];
      if(this->getTensorSize()==9u){
	this->F0[5] = F[6];
	this->F0[6] = F[2];
	this->F0[7] = F[7];
	this->F0[8] = F[5];
      }
    }
  } // end of UmatFiniteStrainMTestFileGenerator::setStrainTensor

  void
  UmatFiniteStrainMTestFileGenerator::setDeformationGradientAtTheEndOfTheStimeStep(const double* const F)
  {
    this->F1[0] = F[0];
    this->F1[1] = F[4];
    this->F1[2] = F[8];
    if(this->getTensorSize()>=5u){
      this->F1[3] = F[3];
      this->F1[4] = F[1];
      if(this->getTensorSize()==9u){
	this->F1[5] = F[6];
	this->F1[6] = F[2];
	this->F1[7] = F[7];
	this->F1[8] = F[5];
      }
    }
  } // end of UmatFiniteStrainMTestFileGenerator::setStrainTensorIncrement

  void
  UmatFiniteStrainMTestFileGenerator::setStressTensor(const double*const s)
  {
    using namespace std;
    copy(s,s+this->getStensorSize(),this->stress);
  } // end of UmatFiniteStrainMTestFileGenerator::setStressTensor

  void
  UmatFiniteStrainMTestFileGenerator::writeDrivingVariables(std::ostream& os) const
  {
    using namespace std;
    const auto& n = this->getDeformationGradientComponentsNames();
    vector<string>::const_iterator p;
    unsigned short i;
    if(this->times.size()!=2){
      string msg("UmatFiniteStrainMTestFileGenerator::writeDrivingVariables : "
		 "invalid number of times");
      throw(runtime_error(msg));
    }
    const real t0 = *(this->times.begin());
    const real t1 = *(this->times.rbegin());
    os << "@Stress {";
    os.precision(14);
    for(i=0;i!=this->getStensorSize();){
      if(i<3){
	os << this->stress[i];
      } else {
	os << (this->stress[i])*sqrt(2.);
      }
      if(++i!=this->getStensorSize()){
	os << ",";
      }
    }
    os << "};\n\n";
    os << "@DeformationGradient {";
    for(p=n.begin(),i=0;p!=n.end();++i){
      os.precision(14);
      os << this->F0[i];
      if(++p!=n.end()){
	os << ",";
      }
    }
    os << "};\n";
    for(p=n.begin(),i=0;p!=n.end();++p,++i){
      os.precision(14);
      os << "@ImposedDeformationGradient<evolution> '" << *p << "' {" 
	 << t0 << ":" << this->F0[i]<< ","
	 << t1 << ":" << this->F1[i]<< "};\n";
    }
    os << endl;
  } // end of UmatFiniteStrainMTestFileGenerator::writeDrivingVariables
  
  UmatFiniteStrainMTestFileGenerator::~UmatFiniteStrainMTestFileGenerator()
  {} // end of UmatFiniteStrainMTestFileGenerator::UmatFiniteStrainMTestFileGenerator

} // end of namespace mfront

