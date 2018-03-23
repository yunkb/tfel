/*!
 * \file  DDIF2Brick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20 oct. 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include "MFront/DDIF2Brick.hxx"

namespace mfront {

  DDIF2Brick::DDIF2Brick(AbstractBehaviourDSL& dsl_,
                         BehaviourDescription& mb_,
                         const Parameters&,
                         const DataMap& d)
      : BehaviourBrickBase(dsl_, mb_) {
    auto& spf = mfront::bbrick::StressPotentialFactory::getFactory();
    this->ddif2= spf.generate("DDIF2", this->dsl, this->bd, d);
  }  // end of DDIF2Brick::DDIF2Brick

  std::string DDIF2Brick::getName() const {
    return "DDIF2";
  }  // end of DDIF2Brick::getName()

  void DDIF2Brick::completeVariableDeclaration() const {
    this->ddif2->completeVariableDeclaration();
  }  // end of DDIF2Brick::completeVariableDeclaration()

  void DDIF2Brick::endTreatment() const {
    this->ddif2->endTreatment();
  }  // end of DDIF2Brick::endTreatment

  std::vector<tfel::material::ModellingHypothesis::Hypothesis>
  DDIF2Brick::getSupportedModellingHypotheses() const {
    return this->ddif2->getSupportedModellingHypotheses();
  }  // end of DDIF2Brick::getSupportedModellingHypothesis

  DDIF2Brick::~DDIF2Brick() = default;

}  // end of namespace mfront
