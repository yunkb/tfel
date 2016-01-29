/*! 
 * \file  mtest/include/MTest/PipeTestParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_PIPETESTPARSER_H_
#define LIB_MTEST_PIPETESTPARSER_H_ 

#include<map>
#include<string>
#include<vector>
#include<memory>

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/SingleStructureSchemeParser.hxx"

namespace mtest
{

  // forward declaration
  struct PipeTest;
  
  /*!
   * PipeTestParser is used to parse mtest file.
   */
  struct MTEST_VISIBILITY_EXPORT PipeTestParser
    : public SingleStructureSchemeParser
  {
    /*!
     * default constructor
     */
    PipeTestParser();
    /*!
     * execute mtest parser on a file
     * \param[out] t : structure to be filled
     * \param[in] f : file name
     */
    void execute(PipeTest&,
		 const std::string&);
    /*!
     * execute mtest parser on a string
     * \param[in] s : string
     */
    void parseString(PipeTest&,
		     const std::string&);
    //! \return the list of keywords
    virtual std::vector<std::string>
    getKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsList() const;
    /*!
     * display the list of keywords
     */
    virtual void
    displayKeyWordsHelp() const;
    /*!
     * display the description of a keyword
     */
    virtual void
    displayKeyWordDescription(const std::string&) const;
    /*!
     * destructor
     */
    virtual ~PipeTestParser();
  protected:
    /*!
     * \brief handle the @RadialLoading keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleRadialLoading(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @AxialLoading keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleAxialLoading(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @InnerRadius keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleInnerRadius(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @OuterRadius keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleOuterRadius(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @NumberOfElements keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleNumberOfElements(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ElementType keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleElementType(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @PerformSmallStrainAnalysis keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handlePerformSmallStrainAnalysis(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @InnerPressureEvolution keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleInnerPressureEvolution(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @OuterPressureEvolution keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleOuterPressureEvolution(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @OuterRadiusEvolution keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleOuterRadiusEvolution(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @AxialForceEvolution keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleAxialForceEvolution(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @AxialGrowthEvolution keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleAxialGrowthEvolution(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @FillingPressure keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleFillingPressure(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @FillingTemperature keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleFillingTemperature(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @DisplacementEpsilon keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleDisplacementEpsilon(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @ResidualEpsilon keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleResidualEpsilon(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @Profile keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleProfile(PipeTest&,TokensContainer::const_iterator&);
    /*!
     * \brief handle the @Test keyword
     * \param[out]    t: test
     * \param[in,out] p: position in the input file
     */
    virtual void
    handleTest(PipeTest&,TokensContainer::const_iterator&);
    //! a simple alias
    typedef void (PipeTestParser::* CallBack)(PipeTest&,
					      TokensContainer::const_iterator&);
    /*!
     * execute mtest parser after reading a file or parsing a string
     * \param[out] t: structure to be filled
     */
    void execute(PipeTest& t);
    /*!
     * register the call backs associated with each command
     */
    virtual void
    registerCallBacks(void) override;
    /*!
     * \brief treat the current keyword
     * \param[in,out] p: position in the input file
     */
    bool treatKeyword(PipeTest&,TokensContainer::const_iterator&);
  private:
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    /*!
     * register a call back
     * \param[in] k: key word
     * \param[in] p: pointer to a member function
     */
    void registerCallBack(const std::string&,const CallBack&);
  }; // end of struct PipeTestParser

} // end of namespace mtest

#endif /* LIB_MTEST_PIPETESTPARSER_H_ */

