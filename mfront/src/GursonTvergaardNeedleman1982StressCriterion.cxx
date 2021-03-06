/*!
 * \file   mfront/src/GursonTvergaardNeedleman1982StressCriterion.cxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   20/07/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/GursonTvergaardNeedleman1982StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    GursonTvergaardNeedleman1982StressCriterion::GursonTvergaardNeedleman1982StressCriterion()
        : StandardPorousStressCriterionBase("GursonTvergaardNeedleman1982") {}  // end of
    // GursonTvergaardNeedleman1982StressCriterion::GursonTvergaardNeedleman1982StressCriterion

    std::vector<mfront::BehaviourSymmetryType>
    GursonTvergaardNeedleman1982StressCriterion::getSupportedBehaviourSymmetries() const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of
    // GursonTvergaardNeedleman1982StressCriterion::getSupporxtedBehaviourSymmetries()

    std::vector<OptionDescription> GursonTvergaardNeedleman1982StressCriterion::getOptions()
        const {
      auto opts = StressCriterionBase::getOptions();
      opts.emplace_back("f_c", "coalescence porosity",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("f_r", "fracture porosity",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("q_1", "first Tvergaard parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("q_2", "second Tvergaard parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("q_3", "third Tvergaard parameter",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of GursonTvergaardNeedleman1982StressCriterion::getOptions()

    StressCriterion::PorosityEffectOnFlowRule
    GursonTvergaardNeedleman1982StressCriterion::getPorosityEffectOnFlowRule() const {
      return StressCriterion::STANDARD_POROSITY_CORRECTION_ON_FLOW_RULE;
    }  // end of GursonTvergaardNeedleman1982StressCriterion::getPorosityEffectOnFlowRule()

    GursonTvergaardNeedleman1982StressCriterion::
        ~GursonTvergaardNeedleman1982StressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
