/*!
 * \file  DDIF2StressPotential.cxx
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

#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/DDIF2StressPotential.hxx"

namespace mfront {

  namespace bbrick {

    DDIF2StressPotential::DDIF2StressPotential(AbstractBehaviourDSL& dsl_,
                                               BehaviourDescription& mb_,
                                               const DataMap& d)
        : HookeStressPotential(dsl_, mb_, DataMap()) {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "DDIF2StressPotential::DDIF2StressPotential: " + m);
      };
      // undefined hypothesis
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      throw_if(this->bd.getElasticSymmetryType() != mfront::ISOTROPIC,
               "the DDIF2 brick is only usable for isotropic behaviours");
      // reserve some specific variables
      this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                           "ddif2bdata");
      // reserve some specific variables
      this->bd.appendToIncludes("#include\"TFEL/Material/DDIF2Base.hxx\"");
      VariableDescription ef("strain", "ef", 3u, 0u);
      ef.description = "fracture strain";
      this->bd.addStateVariable(uh, ef);
      this->bd.setEntryName(uh, "ef", "FractureStrain");
      VariableDescription efm("strain", "efm", 3u, 0u);
      efm.description = "fracture strain";
      this->bd.addAuxiliaryStateVariable(uh, efm);
      this->bd.setEntryName(uh, "efm", "MaximumFractureStrain");
      addLocalVariable(this->bd,"StrainStensor", "nf", 3u);
      // data
      checkOptionsNames(
          d, {"fracture_stress", "fracture_stresses", "softening_slope",
              "softening_slopes", "fracture_energy", "fracture_energies",
              "handle_pressure_on_crack_surface"},
          this->getName());
      throw_if((d.count("fracture_stress")) && (d.count("fracture_stresses")),
               "can't specify 'fracture_stress' and 'fracture_stresses");
      throw_if((d.count("softening_slope")) && (d.count("softening_slopes")) &&
                   (d.count("fracture_energy")) &&
                   (d.count("fracture_energies")),
               "only of of 'softening_slope', 'softening_slopes', "
               "'fracture_energy' and 'fracture_energies' must be specified");
      if (d.count("fracture_stress")) {
        const auto& s = d.at("fracture_stress");
        if (tfel::utilities::is_convertible<double>(s)) {
          const auto v = tfel::utilities::convert<double>(s);
          throw_if(v < 0,
                   "invalid fracture stress (" + std::to_string(v) + ")");
          this->sr[0] = this->sr[1] = this->sr[2] = v;
          addParameter(this->bd,"sigr", "FractureStress", 3, v);
        } else if (s.is<std::string>()) {
          const auto& f = s.get<std::string>();
          addLocalVariable(this->bd,"stress", "sigr", 3);
          this->sr[0] = this->dsl.handleMaterialPropertyDescription(f);
        } else {
          throw_if(true, "invalid type for data 'fracture_stress'");
        }
      }
      if (d.count("fracture_stresses")) {
        const auto& s = d.at("fracture_stresses");
        if (tfel::utilities::is_convertible<std::vector<double>>(s)) {
          const auto v = tfel::utilities::convert<std::vector<double>>(s);
          throw_if(v.size() != 3u,
                   "invalid array size for "
                   "the `fracture_stresses` parameters");
          for (unsigned short i = 0; i != 3; ++i) {
            throw_if(v[i] < 0,
                     "invalid fracture stress (" + std::to_string(v[i]) + ")");
            this->sr[i] = v[i];
          }
          addParameter(this->bd,"sigr", "FractureStress", 3, v);
        } else if (tfel::utilities::is_convertible<std::vector<std::string>>(
                       s)) {
          const auto v = tfel::utilities::convert<std::vector<std::string>>(s);
          throw_if(v.size() != 3u,
                   "invalid array size for "
                   "the `fracture_stresses` parameters");
          addLocalVariable(this->bd,"stress", "sigr", 3);
          for (unsigned short i = 0; i != 3; ++i) {
            this->sr[i] = this->dsl.handleMaterialPropertyDescription(v[i]);
          }
        } else {
          throw_if(true,
                   "invalid type for data 'fracture_stresses', "
                   "expected a list of values, or a list of string");
        }
      }
      if (d.count("softening_slope")) {
        const auto& s = d.at("softening_slope");
        if (tfel::utilities::is_convertible<double>(s)) {
          const auto v = tfel::utilities::convert<double>(s);
          throw_if(v > 0,
                   "invalid softening slope (" + std::to_string(v) + ")");
          this->rp[0] = this->rp[1] = this->rp[2] = v;
          addParameter(this->bd,"Rp", "SofteningSlope", 3u, v);
        } else if (s.is<std::string>()) {
          const auto& f = s.get<std::string>();
          addLocalVariable(this->bd,"stress", "Rp", 3);
          this->rp[0] = this->dsl.handleMaterialPropertyDescription(f);
        } else {
          throw_if(true, "invalid type for data 'softening_slope'");
        }
      }
      if (d.count("softening_slopes")) {
        const auto& s = d.at("softening_slopes");
        if (tfel::utilities::is_convertible<std::vector<double>>(s)) {
          const auto v = tfel::utilities::convert<std::vector<double>>(s);
          throw_if(v.size() != 3u,
                   "invalid array size for "
                   "the `softening_slopes` parameters");
          for (unsigned short i = 0; i != 3; ++i) {
            throw_if(v[i] > 0,
                     "invalid softening slope (" + std::to_string(v[i]) + ")");
            this->rp[i] = v[i];
          }
          addParameter(this->bd,"Rp", "SofteningSlope", 3, v);
        } else if (tfel::utilities::is_convertible<std::vector<std::string>>(
                       s)) {
          const auto v = tfel::utilities::convert<std::vector<std::string>>(s);
          throw_if(v.size() != 3u,
                   "invalid array size for "
                   "the `softening_slopes` parameters");
          addLocalVariable(this->bd,"stress", "Rp", 3);
          for (unsigned short i = 0; i != 3; ++i) {
            this->rp[i] = this->dsl.handleMaterialPropertyDescription(v[i]);
          }
        } else {
          throw_if(true,
                   "invalid type for data 'softening_slopes', "
                   "expected a list of values, or a list of string");
        }
      }
      if (d.count("fracture_energy")) {
        addLocalVariable(this->bd,"stress", "Rp", 3);
        const auto& s = d.at("fracture_energy");
        if (tfel::utilities::is_convertible<double>(s)) {
          const auto v = tfel::utilities::convert<double>(s);
          throw_if(v < 0,
                   "invalid fracture energy (" + std::to_string(v) + ")");
          this->gc[0] = this->gc[1] = this->gc[2] = v;
          addParameter(this->bd,"Gc", "FractureEnergy", 3u, v);
        } else if (s.is<std::string>()) {
          const auto& f = s.get<std::string>();
          addLocalVariable(this->bd,"stress", "Gc", 3);
          this->gc[0] = this->dsl.handleMaterialPropertyDescription(f);
        } else {
          throw_if(true, "invalid type for data 'fracture_energy'");
        }
      }
      if (d.count("fracture_energies")) {
        addLocalVariable(this->bd,"stress", "Rp", 3);
        const auto& s = d.at("fracture_energies");
        if (tfel::utilities::is_convertible<std::vector<double>>(s)) {
          const auto v = tfel::utilities::convert<std::vector<double>>(s);
          throw_if(v.size() != 3u,
                   "invalid array size for "
                   "the `fracture_energies` parameters");
          for (unsigned short i = 0; i != 3; ++i) {
            throw_if(v[i] < 0,
                     "invalid fracture energy (" + std::to_string(v[i]) + ")");
            this->gc[i] = v[i];
          }
          addParameter(this->bd,"Gc", "FractureEnergy", 3, v);
        } else if (tfel::utilities::is_convertible<std::vector<std::string>>(
                       s)) {
          const auto v = tfel::utilities::convert<std::vector<std::string>>(s);
          throw_if(v.size() != 3u,
                   "invalid array size for "
                   "the `fracture_energies` parameters");
          addLocalVariable(this->bd,"stress", "Gc", 3);
          for (unsigned short i = 0; i != 3; ++i) {
            this->gc[i] = this->dsl.handleMaterialPropertyDescription(v[i]);
          }
        } else {
          throw_if(true,
                   "invalid type for data 'fracture_energies', "
                   "expected a list of values, or a list of string");
        }
      }
      if (d.count("handle_pressure_on_crack_surface")) {
        const auto& b = d.at("handle_pressure_on_crack_surface");
        throw_if(!b.is<bool>(),
                 "invalid type for data 'handle_pressure_on_crack_surface'");
        this->pr = b.get<bool>();
        if (this->pr) {
          addExternalStateVariable(this->bd, "stress", "pr",
                                   "PressureOnCrackSurface");
        }
      }
    }  // end of DDIF2StressPotential::DDIF2StressPotential

    std::string DDIF2StressPotential::getName() const { return "DDIF2"; }

    void DDIF2StressPotential::completeVariableDeclaration() const {
      using tfel::glossary::Glossary;
      using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(
            b, "DDIF2StressPotential::completeVariableDeclaration: " + m);
      };
      std::function<std::string(const MaterialPropertyInput&)> ets =
          [this, throw_if](const MaterialPropertyInput& i) -> std::string {
        if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
            (i.category ==
             MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
            (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
          return "this->" + i.name + "+this->d" + i.name;
        } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                   (i.category == MaterialPropertyInput::PARAMETER)) {
          return "this->" + i.name;
        } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
          return this->bd.getClassName() + "::" + i.name;
        }
        throw_if(true, "unsupported input type for variable '" + i.name + "'");
      };
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "DDIF2StressPotential::completeVariableDeclaration: begin\n";
      }
      HookeStressPotential::completeVariableDeclaration();
      std::string init_code;
      // fracture stresses
      if (this->sr[0].empty()) {
        addMaterialPropertyIfNotDefined(this->bd, "stress", "sigr",
                                        "FractureStress", 3u);
      } else if (this->sr[0]
                     .is<std::shared_ptr<MaterialPropertyDescription>>()) {
        if (!this->sr[1].empty()) {
          std::ostringstream ssigr;
          for (unsigned short i = 0; i != 3; ++i) {
            BehaviourDescription::ExternalMFrontMaterialProperty mp_sr;
            mp_sr.mpd =
                this->sr[i].get<std::shared_ptr<MaterialPropertyDescription>>();
            ssigr << "this->sigr[" << i << "] = ";
            this->dsl.writeMaterialPropertyEvaluation(ssigr, mp_sr, ets);
            ssigr << ";\n";
            init_code += ssigr.str();
          }
        } else {
          BehaviourDescription::ExternalMFrontMaterialProperty mp_sr;
          mp_sr.mpd =
              this->sr[0].get<std::shared_ptr<MaterialPropertyDescription>>();
          std::ostringstream ssigr;
          ssigr << "this->sigr[0] = ";
          this->dsl.writeMaterialPropertyEvaluation(ssigr, mp_sr, ets);
          ssigr << ";\n";
          ssigr << "this->sigr[2] = this->sigr[1] = this->sigr[0];\n";
          init_code += ssigr.str();
        }
      }
      // softening slopes
      if ((this->rp[0].empty()) && (this->gc[0].empty())) {
        addMaterialPropertyIfNotDefined(this->bd, "stress", "Rp",
                                        "SofteningSlope", 3u);
      }
      if (this->rp[0].is<std::shared_ptr<MaterialPropertyDescription>>()) {
        if (!this->rp[1].empty()) {
          std::ostringstream sRp;
          for (unsigned short i = 0; i != 3; ++i) {
            BehaviourDescription::ExternalMFrontMaterialProperty mp_rp;
            mp_rp.mpd =
                this->rp[i].get<std::shared_ptr<MaterialPropertyDescription>>();
            sRp << "this->Rp[" << i << "] = ";
            this->dsl.writeMaterialPropertyEvaluation(sRp, mp_rp, ets);
            sRp << ";\n";
            init_code += sRp.str();
          }
        } else {
          BehaviourDescription::ExternalMFrontMaterialProperty mp_rp;
          mp_rp.mpd = this->rp[0];
          std::ostringstream srp;
          srp << "this->Rp[0] = ";
          this->dsl.writeMaterialPropertyEvaluation(srp, mp_rp, ets);
          srp << ";\n";
          srp << "this->Rp[2] = this->Rp[1] = this->Rp[0];\n";
          init_code += srp.str();
        }
      }
      //  fracture energies
      if (this->gc[0].is<std::shared_ptr<MaterialPropertyDescription>>()) {
        if (!this->gc[1].empty()) {
          std::ostringstream sGc;
          for (unsigned short i = 0; i != 3; ++i) {
            BehaviourDescription::ExternalMFrontMaterialProperty mp_gc;
            mp_gc.mpd =
                this->gc[i].get<std::shared_ptr<MaterialPropertyDescription>>();
            sGc << "this->Gc[" << i << "] = ";
            this->dsl.writeMaterialPropertyEvaluation(sGc, mp_gc, ets);
            sGc << ";\n";
            init_code += sGc.str();
          }
        } else {
          BehaviourDescription::ExternalMFrontMaterialProperty mp_gc;
          mp_gc.mpd = this->gc[0];
          std::ostringstream sgc;
          sgc << "this->Gc[0] = ";
          this->dsl.writeMaterialPropertyEvaluation(sgc, mp_gc, ets);
          sgc << ";\n";
          sgc << "this->Gc[2] = this->Gc[1] = this->Gc[0];\n";
          init_code += sgc.str();
        }
      }
      if (!this->gc[0].empty()) {
        addMaterialPropertyIfNotDefined(this->bd, "length", "Lc", "ElementSize",
                                        3u);
        const std::string young = this->bd.areElasticMaterialPropertiesDefined()
                                      ? "this->young_tdt"
                                      : "this->young";
        init_code +=
            "this->Rp[0]=-((this->Lc[0])*(this->sigr[0])*(this->sigr[0]))/"
            "(2*(this->Gc[0]));\n";
        init_code +=
            "this->Rp[1]=-((this->Lc[1])*(this->sigr[1])*(this->sigr[1]))/"
            "(2*(this->Gc[1]));\n";
        init_code +=
            "this->Rp[2]=-((this->Lc[2])*(this->sigr[2])*(this->sigr[2]))/"
            "(2*(this->Gc[2]));\n";
      }
      LocalDataStructure d;
      d.name = "ddif2bdata";
      d.addVariable(uh, {"StressStensor", "sig"});
      this->bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
      // modelling hypotheses supported by the brick
      const auto smh = this->getSupportedModellingHypotheses();
      // modelling hypotheses supported by the behaviour
      const auto bmh = this->bd.getModellingHypotheses();
      for (const auto& h : bmh) {
        throw_if(std::find(smh.begin(), smh.end(), h) == smh.end(),
                 "unsupported hypothesis '" + ModellingHypothesis::toString(h) +
                     "'");
      }
      // init local variables
      init_code +=
          "for(unsigned short idx=0;idx!=3;++idx){\n"
          "this->nf[idx]      = Stensor(real(0));\n"
          "this->nf[idx][idx] = real(1);\n"
          "}\n";
      for (const auto h : this->bd.getModellingHypotheses()) {
        CodeBlock init;
        init.code = init_code;
        if (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
          addMaterialPropertyIfNotDefined(this->bd, "real", "angl",
                                          "AngularCoordinate");
          init.code +=
              "// change to cylindrical coordinates\n"
              "DDIF2Base::cart2cyl(this->deto,this->angl);\n";
        }
        this->bd.setCode(h, BehaviourData::BeforeInitializeLocalVariables, init,
                         BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "DDIF2StressPotential::completeVariableDeclaration: end\n";
      }
    }

    void DDIF2StressPotential::endTreatment() const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "DDIF2StressPotential::endTreatment: begin\n";
      }
      HookeStressPotential::endTreatment();
      // implicit equation associated with the crack strains
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
      CodeBlock integrator;
      integrator.code =
          "feel += "
          "((this->def)[0])*(this->nf[0])+\n"
          "((this->def)[1])*(this->nf[1])+\n"
          "((this->def)[2])*(this->nf[2]);\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        const std::string young = this->bd.areElasticMaterialPropertiesDefined()
                                      ? "this->young_tdt"
                                      : "this->young";
        const std::string lambda =
            this->bd.areElasticMaterialPropertiesDefined()
                ? "this->lambda_tdt"
                : "this->sebdata.lambda";
        const std::string mu = this->bd.areElasticMaterialPropertiesDefined()
                                   ? "this->mu_tdt"
                                   : "this->sebdata.mu";
        integrator.code += "this->ddif2bdata.sig=(" + lambda +
                           "*trace(this->eel+deel))*StrainStensor::Id()+\n"
                           "                     2*" +
                           mu +
                           "*(this->eel+this->deel);\n"
                           "this->dfeel_ddef(0)(0) = real(1);\n"
                           "this->dfeel_ddef(1)(1) = real(1);\n"
                           "this->dfeel_ddef(2)(2) = real(1);\n"
                           "for(unsigned short idx=0;idx!=3;++idx){\n"
                           "auto dfe_ddeel = dfef_ddeel(idx);\n";
        if (this->pr) {
          integrator.code +=
              "DDIF2Base::treatFracture(dfe_ddeel,dfef_ddef(idx,idx),fef(idx),"
              "\n"
              "		            "
              "this->efm(idx),this->ef(idx),(this->def)(idx),\n"
              "                         this->ddif2bdata.sig,this->nf(idx),\n"
              "		            this->sigr[idx],this->Rp[idx],\n"
              "		            " +
              young + "," + lambda + ',' + mu +
              ",\n"
              "                         this->pr+this->dpr);\n";
        } else {
          integrator.code +=
              "DDIF2Base::treatFracture(dfe_ddeel,dfef_ddef(idx,idx),fef(idx),"
              "\n"
              "		            "
              "this->efm(idx),this->ef(idx),(this->def)(idx),\n"
              "                         this->ddif2bdata.sig,this->nf(idx),\n"
              "		            this->sigr[idx],this->Rp[idx],\n"
              "		            " +
              young + "," + lambda + ',' + mu + ");\n";
        }
        integrator.code += "}\n";
      }
      /* fracture */
      this->bd.setCode(uh, BehaviourData::Integrator, integrator,
                       BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      /* update auxiliary state variables */
      for (const auto h : this->bd.getModellingHypotheses()) {
        CodeBlock uasv;
        uasv.code =
            "this->efm[0]=std::max(this->efm[0],this->ef[0]);\n"
            "this->efm[1]=std::max(this->efm[1],this->ef[1]);\n"
            "this->efm[2]=std::max(this->efm[2],this->ef[2]);\n";
        if (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
          uasv.code +=
              "// change to cartesian coordinates\n"
              "DDIF2Base::cyl2cart(this->sig,this->angl);\n";
        }
        this->bd.setCode(h, BehaviourData::UpdateAuxiliaryStateVariables, uasv,
                         BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
    }  // end of DDIF2StressPotential::endTreatment

    std::vector<tfel::material::ModellingHypothesis::Hypothesis>
    DDIF2StressPotential::getSupportedModellingHypotheses() const {
      const auto mh = this->dsl.getDefaultModellingHypotheses();
      return {mh.begin(), mh.end()};
    }  // end of DDIF2StressPotential::getSupportedModellingHypothesis

    DDIF2StressPotential::~DDIF2StressPotential() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
