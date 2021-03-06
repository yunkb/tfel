/*!
 * \file   mfront/include/MFront/DSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DSLBASE_HXX
#define LIB_MFRONT_DSLBASE_HXX

#include <set>
#include <map>
#include <tuple>
#include <memory>
#include <string>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/CodeBlock.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;

  /*!
   * \return if the given name is valid
   * \param[in] n: material name
   */
  MFRONT_VISIBILITY_EXPORT  bool isValidMaterialName(const std::string&);
  /*!
   * \return if the given name is valid
   * \param[in] n: library name
   */
  MFRONT_VISIBILITY_EXPORT  bool isValidLibraryName(const std::string&);

  /*!
   * \brief base structure for domain specific languages
   */
  struct MFRONT_VISIBILITY_EXPORT DSLBase
      : public virtual AbstractDSL,
        public tfel::utilities::CxxTokenizer {
    //! \return the file description associated with the treated file
    const FileDescription& getFileDescription() const override final;
    /*!
     * \return the target description
     * \note This method shall be called *after* the analyseFile method
     */
    const TargetsDescription& getTargetsDescription() const override;
    /*!
     * \brief open a file and add given external instructions at the
     * beginning
     * \param[in] f     : file
     * \param[in] ecmds : external commands
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    virtual void openFile(const std::string&,
                          const std::vector<std::string>&,
                          const std::map<std::string, std::string>&);

   protected:
    /*!
     * \brief An helper structure used by the
     * BehaviourDSLCommon class to modify the code provided
     * by the user.
     *
     * These modifiers are called when the parser encounters variables.
     */
    struct MFRONT_VISIBILITY_EXPORT VariableModifier {
      /*!
       * \param[in] v : the variable name
       * \param[in] b : true if "this" shall be added
       */
      virtual std::string exe(const std::string&, const bool) = 0;
      /*!
       * destructor
       */
      virtual ~VariableModifier();
    };  // end of struct VariableModifier
        /*!
         * \brief An helper structure used by the DSLBase class to modify the
         * code provided by the user.
         *
         * These modifiers are called when the parser encounters variables.
         */
    struct MFRONT_VISIBILITY_EXPORT WordAnalyser {
      /*!
       * \param[in] cb : code block being treated
       * \param[in] k : the current word
       */
      virtual void exe(CodeBlock&, const std::string&) = 0;
      /*!
       * destructor
       */
      virtual ~WordAnalyser();
    };  // end of struct WordAnalyser
    //! \brief options passed to the readNextBlock member
    struct CodeBlockParserOptions {
      //! default constructor
      CodeBlockParserOptions();
      //! move constructor
      CodeBlockParserOptions(CodeBlockParserOptions&&) = default;
      //! copy constructor
      CodeBlockParserOptions(const CodeBlockParserOptions&) = default;
      //! move assignement
      CodeBlockParserOptions& operator=(CodeBlockParserOptions&&) = default;
      //! standard assignement
      CodeBlockParserOptions& operator=(const CodeBlockParserOptions&) =
          default;
      //! destructor
      ~CodeBlockParserOptions() noexcept;
      /*!
       * \brief list of known symbols.
       * The key is the symbolic version of the variable.
       * The value is the mangled of the variable.
       */
      std::map<std::string, std::string> symbols;
      //! member names
      std::set<std::string> mn;
      //! static member names
      std::set<std::string> smn;
      //! modifier applied to variables
      std::shared_ptr<VariableModifier> modifier;
      //! analyser applied to variables
      std::shared_ptr<WordAnalyser> analyser;
      //! delimeter marking the beginning of the block to be read
      std::string delim1 = "{";
      //! delimeter marking the end of the block to be read
      std::string delim2 = "}";
      //! if this member is true, the class name will be added in
      //! front of variables names
      bool qualifyStaticVariables = false;
      //! if this member is true, the "this->" prefix will be added in
      //! front of variables names
      bool qualifyMemberVariables = false;
      //! if true, the semi-colon will be allowed
      bool allowSemiColon = true;
      //! if true, line number will be registred
      bool registerLine = true;
    };  // end of CodeBlockParserOptions
    //! \return a list of names that shall be reserved
    static std::vector<std::string> getDefaultReservedNames();
    /*!
     * constructor
     */
    DSLBase();
    /*!
     * \brief register a name.
     * \param[in] n : name
     */
    virtual void reserveName(const std::string&) = 0;
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    virtual bool isNameReserved(const std::string&) const = 0;
    /*!
     * \return a temporary name which has not been reserved. This
     * method shall be used to get a temporary name at a
     * code block scope.
     * \param[in,out] tmpnames: list of already used temporary names
     * in the treated code block.
     * \param[in]     p: prefix
     */
    virtual std::string getTemporaryVariableName(std::vector<std::string>&,
                                                 const std::string&) const;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void addStaticVariableDescription(
        const StaticVariableDescription&) = 0;
    /*!
     * \return the value of an integer constant
     * \param[in] n: variable name
     */
    virtual int getIntegerConstant(const std::string&) const = 0;
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName() const = 0;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    virtual void addMaterialLaw(const std::string&) = 0;
    /*!
     * \brief append the given code to the includes
     */
    virtual void appendToIncludes(const std::string&) = 0;
    /*!
     * \brief append the given code to the members
     */
    virtual void appendToMembers(const std::string&) = 0;
    /*!
     * \brief append the given code to the private code
     */
    virtual void appendToPrivateCode(const std::string&) = 0;
    /*!
     * \brief append the given code to the sources
     */
    virtual void appendToSources(const std::string&) = 0;
    /*!
     * \brief check that the end of file in not reached yet
     * \param[in] m: calling method name
     * \param[in] e: error message
     */
    void checkNotEndOfFile(const std::string&, const std::string& = "") const;
    /*!
     * \brief read a specified token and advance to the next token
     * \param[in] m: calling method name
     * \param[in] v: expected value
     */
    void readSpecifiedToken(const std::string&, const std::string&);
    /*!
     * \brief throw an std::runtime_error
     * \param[in] m: calling method name
     * \param[in] e: error message
     */
    TFEL_NORETURN void throwRuntimeError(const std::string&,
                                         const std::string&) const;
    /*!
     * \return concatenate all the tokens from the current position up
     * to the first ';' token
     */
    std::string readUntilEndOfInstruction();
    /*!
     * read an unsigned short in the file
     * \param[in] m : calling method
     */
    unsigned short readUnsignedShort(const std::string&);
    /*!
     * \brief parse the next code block twice.
     * \param[out] res1 : resulting code block with the first  option set.
     * \param[out] res2 : resulting code block with the second option set.
     * \param[in]  o1   : first  option set.
     * \param[in]  o2   : second option set.
     */
    void readNextBlock(CodeBlock&,
                       CodeBlock&,
                       const CodeBlockParserOptions&,
                       const CodeBlockParserOptions&);
    /*!
     * \return the code block read.
     * \param[in] o : options
     */
    CodeBlock readNextBlock(const CodeBlockParserOptions&);
    /*!
     * \brief read one token and increment "current"
     */
    std::string readOnlyOneToken();
    /*!
     *
     */
    template <typename T>
    std::pair<bool, T> readInitialisationValue(const std::string&, const bool);
    //! \brief read a C++ type
    std::pair<std::string, bool> readType();
    /*!
     * \return the size of an array of variables, or 1 if the variable are not
     * defined as an array.
     * \param[in] n: variable name
     * \param[in] b: allow arrays of variables to be defined
     */
    unsigned int readArrayOfVariablesSize(const std::string&, const bool);
    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] type : type of the variable
     * \param[in] allowArray : allow arrays of variables to be defined
     */
    void readVarList(VariableDescriptionContainer&,
                     const std::string&,
                     const bool);
    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] allowArray : allow arrays of variables to be defined
     */
    void readVarList(VariableDescriptionContainer&, const bool);
    /*!
     * extract a boolean value from the current token and go the next
     * token
     * \param[in] m : calling method name (used for error message)
     * \return the boolean value read
     */
    bool readBooleanValue(const std::string&);
    /*!
     * extract a string from the current token and go the next token
     * \param[in] m : calling method name (used for error message)
     * \return the extracted string
     */
    std::string readString(const std::string&);
    /*!
     * extract an array of string starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of strings
     */
    std::vector<std::string> readArrayOfString(const std::string&);
    /*!
     * extract an array of double starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of doubles
     */
    std::vector<double> readArrayOfDouble(const std::string&);
    /*!
     * extract an array of string starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of strings which contains only one
     * element if a string was read
     */
    std::vector<std::string> readStringOrArrayOfString(const std::string&);
    /*!
     * \brief extract a list of token
     * \param[out] l  : list read
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in] b   : if the current value is not the beginning
     * delimiter (or is past the end of the file), nothing is returned
     * if this parameter is true. An exception is thrown otherwise
     */
    void readList(std::vector<tfel::utilities::Token>&,
                  const std::string&,
                  const std::string&,
                  const std::string&,
                  const bool);
    /*!
     * \return a list of token
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in] b   : if the current value is not the beginning
     * delimiter (or is past the end of the file), nothing is returned
     * if this parameter is true. An exception is thrown otherwise
     */
    std::vector<tfel::utilities::Token> readList(const std::string&,
                                                 const std::string&,
                                                 const std::string&,
                                                 const bool);

    std::string readSpecifiedValue(const std::string&, const std::string&);

    std::vector<std::string> readSpecifiedValues(const std::string&,
                                                 const std::string&,
                                                 const std::string&);

    std::vector<std::string> readSpecifiedValues(
        const std::string&, const std::vector<std::string>&);
    /*!
     * \brief read the bounds associated to a variable
     * \return a pair giving the variable name and the bounds
     */
    std::pair<std::string, VariableBoundsDescription> readVariableBounds();
    /*!
     * call mfront in a subprocess
     * \param[in] interfaces : list of interfaces
     * \param[in] files      : list of files
     */
    virtual void callMFront(const std::vector<std::string>&,
                            const std::vector<std::string>&);
    /*!
     * \brief This function handles a material property treated as a
     * dependency of the current file.
     *
     * This function:
     * - analyse the given file using the MaterialPropertyDSL
     * - register the function name generated by the MFront interface
     * - add the function name to the list of material laws used by
     *   the current file
     * - declared the headers generated by the MFront interface for inclusion
     * - launch mfront in a sub-process to generate the source files
     *   associated with this material property and have them compiled
     *   when mandatory (done by the callMFront method).
     *
     * \param[in] f : file in which the material law is
     * implemented. This must be the full path.
     */
    virtual std::shared_ptr<MaterialPropertyDescription>
    handleMaterialPropertyDescription(const std::string&);
    //! handle the `@MFront` keyword
    virtual void treatMFront();
    //! handle the `@Import` keyword
    virtual void treatImport();
    //! handle the `@MaterialLaw` keyword
    virtual void treatMaterialLaw();
    //! handle the `@Link` keyword
    virtual void treatLink();
    //! handle the `@Author` keyword
    virtual void treatAuthor();
    //! handle the `@Data` keyword
    virtual void treatDate();
    //! handle the `@Includes` keyword
    virtual void treatIncludes();
    //! handle the `@Sources` keyword
    virtual void treatSources();
    //! handle the `@Private` keyword
    virtual void treatPrivate();
    //! handle the `@Members` keyword
    virtual void treatMembers();
    //! handle the `@Parser` keyword
    virtual void treatParser();
    //! handle the `@StaticVar` keyword
    virtual void treatStaticVar();
    //! handle the `@IntegerConstant` keyword
    virtual void treatIntegerConstant();
    //! handle the `@Description` keyword
    virtual void treatDescription();
    //! handle the `@LonelySeparator` keyword
    virtual void treatLonelySeparator();
    //!
    virtual void ignoreKeyWord(const std::string&);
    //! treat an unknown keyword
    virtual void treatUnknownKeyword();
    /*!
     * \brief this method must be called at the end of the treatment:
     * - to set each libraries defined the auxiliary targets as
     *   dependencies of the librairies defined in the main one.  -
     * - merge the auxiliary targets descriptions into the main one.
     * - clear the auxiliary targets descriptions
     */
    virtual void completeTargetsDescription();
    /*!
     * \brief register an integer constant.
     * \param[in] n: name of the constant
     * \param[in] l: line number
     * \param[in] v: value
     */
    virtual void registerIntegerConstant(const std::string&,
                                         const size_t,
                                         const int);
    /*!
     * \brief extract a double from the current token
     */
    double readDouble();
    //! destructor
    ~DSLBase() override;
    //! \brief description of the file treated
    FileDescription fd;
    /*!
     * \brief main targets description
     *
     * Targets description are generated by the processing of the
     * input file. However, it is convenient to distinguish during the
     * processing phase:
     * - the main targets which directly result from the processing of
     *   the current file. Those targets can only completly be defined
     *   at the end of the processing.
     * - the auxiliary targets descriptions for targets generated
     *   indirectly (for example by importing material law using the
     *   `@MaterialLaw` keyword).
     *
     * Once the input is completely processed, the auxiliary targets
     * descriptions are merged into the main one using the
     * `completeTargetsDescription` method.
     */
    TargetsDescription td;
    //! additional linker flags
    std::vector<std::string> ldflags;
    //! auxiliary target descriptions
    std::vector<TargetsDescription> atds;
    /*!
     * \brief external mfront files
     * - key: mfront file name (full path)
     * - value: list of interfaces to be used
     * This list of external mfront files will be used to generate the
     * associated sources.
     */
    std::map<std::string, std::vector<std::string>> externalMFrontFiles;
    //! current position in the input stream
    TokensContainer::const_iterator current;
    /*!
     * \brief current comment
     * This is the user comment associated with the current keyword,
     * if any.
     *
     * \note This variable is automatically set by parsers before
     * calling the callback associated with the keyword and
     * automatically unset once the callback returns.
     */
    std::string currentComment;

  };  // end of class DSLBase

}  // end of namespace mfront

#include "MFront/DSLBase.ixx"

#endif /* LIB_MFRONT_DSLBASE_HXX */
