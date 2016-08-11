/*! 
 * \file  mfront/mtest/Evolution.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include<stdexcept>

#include"MTest/Evolution.hxx"

namespace mtest
{
  
  Evolution::~Evolution()
  {} // end of Evolution::~Evolution()

  ConstantEvolution::ConstantEvolution(const real v)
    : value(v)
  {}

  real
  ConstantEvolution::operator()(const real) const
  {
    return this->value;
  } // end of ConstantEvolution::operator()

  bool
  ConstantEvolution::isConstant() const
  {
    return true;
  } // end of ConstantEvolution::isConstant

  void ConstantEvolution::setValue(const real v)
  {
    this->value = v;
  }
  
  void ConstantEvolution::setValue(const real,const real)
  {
    throw(std::runtime_error("ConstantEvolution::setValue : "
			     "this method does not makes sense "
			     "for constant evolution"));
  }

  ConstantEvolution::~ConstantEvolution()
  {} // end of ConstantEvolution::~ConstantEvolution

  LPIEvolution::LPIEvolution(const std::vector<real>& t,
			     const std::vector<real>& v)
  {
    using namespace std;
    if(t.size()!=v.size()){
      string msg("LPIEvolution::LPIEvolution : "
		 "the number of values of the times don't match "
		 "the number of values of the evolution");
      throw(runtime_error(msg));
    }
    auto pt = t.begin();
    auto pv = v.begin();
    while(pt!=t.end()){
      this->values.insert({*pt,*pv});
      ++pt;
      ++pv;
    }
  }

  void LPIEvolution::setValue(const real)
  {
    throw(std::runtime_error("LPIEvolution::setValue : "
			     "this method does not makes sense "
			     "for LPI evolution"));
  }
  
  void LPIEvolution::setValue(const real t,
			      const real v)
  {
    this->values[t] = v;
  }

  real
  LPIEvolution::operator()(const real t) const
  {
    if(this->values.empty()){
      throw(std::runtime_error("LPILoadingEvolution::getValue: "
			       "no values specified"));
    }
    auto p = this->values.lower_bound(t);
    real x0;
    real x1;
    real y0;
    real y1;
    if(this->values.size()==1u){
      return this->values.begin()->second;
    }
    if(p==this->values.begin()){
      return p->second;
    } else if(p==this->values.end()){
      --p;
      return p->second;
    } else {
      x1 = p->first;
      y1 = p->second;
      --p;
      x0 = p->first;
      y0 = p->second;
    }
    return (y1-y0)/(x1-x0)*(t-x0)+y0;
  }

  bool
  LPIEvolution::isConstant() const
  {
    return (this->values.size()==1);
  } // end of LPIEvolution::isLPI

  LPIEvolution::~LPIEvolution()
  {}

  std::shared_ptr<Evolution>
  make_evolution(const real v){
    return std::shared_ptr<Evolution>{new ConstantEvolution(v)};
  }

  std::shared_ptr<Evolution>
  make_evolution(const std::map<real,real>& v){
    std::vector<real> tv(v.size());
    std::vector<real> ev(v.size());
    std::vector<real>::size_type i = 0u;
    for(const auto& mv : v){
      tv[i] = mv.first;
      ev[i] = mv.second;
      ++i;
    }
    return std::shared_ptr<Evolution>{new LPIEvolution(tv,ev)};
  }
  
} // end of namespace mtest
