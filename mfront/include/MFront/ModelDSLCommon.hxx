/*!
 * \file   mfront/include/MFront/ModelDSLCommon.hxx
 * \brief  This file declares the ModelDSLCommon class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTMODELDSLCOMMON_HXX
#define LIB_MFRONT_MFRONTMODELDSLCOMMON_HXX 

#include<set>
#include<map>
#include<vector>
#include<string>

#include"MFront/MFrontConfig.hxx"

#include"MFront/DSLBase.hxx"
#include"MFront/AbstractDSL.hxx"
#include"MFront/ModelDescription.hxx"

namespace mfront{

  // forward declaration
  struct AbstractModelInterface;

  /*!
   * \return if the given name is valid
   * \param[in] n: model name
   */
  MFRONT_VISIBILITY_EXPORT  bool isValidModelName(const std::string&);

  struct MFRONT_VISIBILITY_EXPORT ModelDSLCommon : public DSLBase {
    //! constructor
    ModelDSLCommon();
    DSLTarget getTargetType() const override final;
    void endsInputFileProcessing() override;
    void generateOutputFiles() override;
    void setInterfaces(const std::set<std::string>&) override;
    //! destructor
    ~ModelDSLCommon() override;

  protected:
    /*!
     * \brief register a name.
     * \param[in] n : name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     */
    void reserveName(const std::string&) override;
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    bool isNameReserved(const std::string&) const override;
    /*!
     * \return the name of the generated class
     */
    std::string getClassName() const override ;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    void addMaterialLaw(const std::string&) override;
    /*!
     * \brief append the given code to the includes
     */
    void appendToIncludes(const std::string&) override;
    /*!
     * \brief append the given code to the members
     */
    void appendToMembers(const std::string&) override;
    /*!
     * \brief append the given code to the private code
     */
    void appendToPrivateCode(const std::string&) override;
    /*!
     * \brief append the given code to the sources
     */
    void appendToSources(const std::string&) override;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    void addStaticVariableDescription(const StaticVariableDescription&) override;
    /*!
     * \return the value of an integer constant
     * \param[in] n: variable name
     */
    int getIntegerConstant(const std::string&) const override;
    //! treat the `@Material` keyword
    virtual void treatMaterial();
    //! treat the `@Library` keyword
    virtual void treatLibrary();

    virtual void treatModel();

    virtual void treatDomain();

    virtual void treatDomains();

    void treatUnknownKeyword() override;

    virtual void treatBounds();
    
    virtual void treatPhysicalBounds();

    virtual void treatConstantMaterialProperty();

    virtual void treatConstantMaterialPropertyMethod();

    virtual void treatFunction();

    virtual void treatOutput();

    virtual void treatOutputMethod();

    virtual void treatInput();

    virtual void treatInputMethod();

    virtual void treatParameter();

    virtual void treatLocalParameter();

    virtual void treatParameterMethod();

    virtual void readDefaultValue();

    virtual bool isOutputVariable(const std::string&) const;

    virtual bool isInputVariable(const std::string&) const;

    ModelDescription md;
    
    std::map<std::string,
	     std::shared_ptr<AbstractModelInterface>> interfaces;

    std::string currentVar;
  }; // end of class ModelDSLCommon

} // end of namespace mfront  

#endif /* LIB_MFRONT_MFRONTMODELDSLCOMMON_HXX */

