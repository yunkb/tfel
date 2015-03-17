/*!
 * \file   mfront/src/CppMaterialPropertyInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CppMaterialPropertyInterface.hxx"

namespace mfront
{

  std::string
  CppMaterialPropertyInterface::getName(void)
  {
    return "c++";
  }

  CppMaterialPropertyInterface::CppMaterialPropertyInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppMaterialPropertyInterface::treatKeyword(const std::string&,
				      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				      const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  CppMaterialPropertyInterface::~CppMaterialPropertyInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  CppMaterialPropertyInterface::getGlobalDependencies(const std::string& library,
					       const std::string& material,
					       const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["libCpp"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of CppMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  CppMaterialPropertyInterface::getGlobalIncludes(const std::string&,
					   const std::string&,
					   const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of CppMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  CppMaterialPropertyInterface::getGeneratedSources(const std::string& lib,
					     const std::string& mat,
					     const std::string& name)
  {
    using namespace std;
    map<string,vector<string> > src;
    if(!mat.empty()){
      src["libCpp"+getMaterialLawLibraryNameBase(lib,mat)].push_back(mat+"_"+name+"-cxx.cxx");
    } else {
      src["libCpp"+getMaterialLawLibraryNameBase(lib,mat)].push_back(name+"-cxx.cxx");
    }
    return src;
  } // end of CppMaterialPropertyInterface::getGeneratedSources

  std::vector<std::string>
  CppMaterialPropertyInterface::getGeneratedIncludes(const std::string&,
					      const std::string&,
					      const std::string&)
  {
    using namespace std;
    vector<string> incs;
    incs.push_back(this->headerFileName);
    return incs;
  } // end of CppMaterialPropertyInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  CppMaterialPropertyInterface::getLibrariesDependencies(const std::string& lib,
						  const std::string& mat,
						  const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["libCpp"+getMaterialLawLibraryNameBase(lib,mat)].push_back("-lm");
    return libs;
  } // end of CppMaterialPropertyInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  CppMaterialPropertyInterface::getSpecificTargets(const std::string&,
					    const std::string&,
					    const std::string&,
					    const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of CppMaterialPropertyInterface::getSpecificTargets

  std::map<std::string,std::vector<std::string> >
  CppMaterialPropertyInterface::getGeneratedEntryPoints(const std::string& library,
							const std::string& material,
							const std::string& className)
  {
    using namespace std;
    const auto name = (!material.empty()) ? material+"_"+className : className;
    auto cn = string{};
#warning "HERE"
    // for(const auto& ns : this->namespaces){
    //   cc += ns + "::"
    // }
    cn += name;    
    auto r = map<string,vector<string>>{};
    const auto l = "libCpp"+getMaterialLawLibraryNameBase(library,material);
    r[l].push_back(cn);
    return r;
  }

  void
  CppMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						 const FileDescription& fd)
  {
    using namespace std;
    using namespace tfel::system;
    const auto& file=fd.fileName;
    const auto& author=fd.authorName;
    const auto& description=fd.description;
    const auto& date=fd.date;
    const auto& material=mpd.material;
    const auto& className=mpd.className;
    const auto& includes=mpd.includes;
    const auto& output=mpd.output;
    const auto& inputs=mpd.inputs;
    const auto& materialLaws=mpd.materialLaws;
    const auto& staticVars=mpd.staticVars;
    const auto& params=mpd.parameters;
    const auto& paramValues=mpd.parametersValues;
    const auto& function=mpd.f;
    const auto& bounds=mpd.boundsDescriptions;
    const auto& physicalBounds=mpd.physicalBoundsDescriptions;
    string dir;
    string name;
    if(!material.empty()){
      name = material+"_"+className;
    } else {
      name = className;
    }
    dir="include/";
    this->headerFileName  = "include/";
    this->headerFileName += name;
    this->headerFileName += "-cxx.hxx";
    this->srcFileName  = "src/" + name;
    this->srcFileName += "-cxx.cxx";
    this->headerFile.open(this->headerFileName.c_str());
    if(!this->headerFile){
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeHeaderFile(name,author,date,description,includes,inputs,
			  params,bounds,physicalBounds);
    this->writeSrcFile(file,name,author,date,output,
		       inputs,materialLaws,staticVars,params,paramValues,
		       function,bounds,physicalBounds);
  } // end of CppMaterialPropertyInterface::writeOutputFiles

  void
  CppMaterialPropertyInterface::writeHeaderFile(const std::string& name,
						const std::string& author,
						const std::string& date,
						const std::string& description,
						const std::string& includes,
						const VariableDescriptionContainer& inputs,
						const std::vector<std::string>& params,
						const std::vector<VariableBoundsDescription>& bounds,
						const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    vector<string>::const_iterator p3;
    VariableDescriptionContainer::const_iterator p4;
    vector<string>::const_reverse_iterator p5;
    vector<string>::const_reverse_iterator p5e;
    map<string,double>::const_iterator p6;

    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << name << " MaterialLaw.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!author.empty()){
      this->headerFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->headerFile << "* \\date   " << date << endl;
    }
    if(!description.empty()){
      this->headerFile << description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef LIB_MFRONT_" << makeUpperCase(name) << "_HXX\n";
    this->headerFile << "#define LIB_MFRONT_" << makeUpperCase(name) << "_HXX\n\n";
    this->headerFile << "#include<ostream>\n";
    this->headerFile << "#include<cmath>\n";
    this->headerFile << "#include<algorithm>\n";
    this->headerFile << "#include<stdexcept>\n\n";
    this->headerFile << "#include<functional>\n\n";
    if(!includes.empty()){
      this->headerFile << includes << endl;
    }
    writeExportDirectives(this->headerFile);    
    this->headerFile << "namespace mfront\n{\n\n";
    this->headerFile << "struct MFRONT_SHAREDOBJ " << name << endl;
    if(!inputs.empty()){
      if(inputs.size()==1){
	this->headerFile << ": std::unary_function<double,double>\n";
      } else if(inputs.size()==2){
	this->headerFile << ": std::binary_function<double,double,double>\n";
      }
    }
    this->headerFile << "{\n\n";
    if(inputs.empty()){
      this->headerFile << "//! nested typedef to make " << name << " model an adaptable generator (STL compliance)\n\n";
      this->headerFile << "typedef double result_type;\n\n";
    } 
    this->headerFile << "//! Default constructor\n";
    this->headerFile << name << "();\n\n";
    this->headerFile << "//! Copy constructor\n";
    this->headerFile << name << "(const "
		     << name<< "&);\n\n";
    this->headerFile << "//! Assignement operator\n";
    this->headerFile << name << "&\n";
    this->headerFile << "operator=(const " << name << "&);\n\n";
    this->headerFile << "static double\ndefaultCompute(";
    if(!inputs.empty()){
      for(p4=inputs.begin();p4!=inputs.end();){
	this->headerFile << "const double";
	if((++p4)!=inputs.end()){
	  this->headerFile << ",";
	}
      }
    } else {
      this->headerFile << "void";
    }
    this->headerFile << ");\n\n";
    this->headerFile << "double\ncompute(";
    for(p4=inputs.begin();p4!=inputs.end();){
      this->headerFile << "const double";
      if((++p4)!=inputs.end()){
	this->headerFile << ",";
      }
    }
    this->headerFile << ");\n\n";
    this->headerFile << "double\noperator()(";
    for(p4=inputs.begin();p4!=inputs.end();){
      this->headerFile << "const double";
      if((++p4)!=inputs.end()){
	this->headerFile << ",";
      }
    }
    this->headerFile << ");\n\n";
    if((!bounds.empty())||
       (!physicalBounds.empty())){
      this->headerFile << "static void\ncheckBounds(";
      for(p4=inputs.begin();p4!=inputs.end();){
	this->headerFile << "const double";
	if((++p4)!=inputs.end()){
	  this->headerFile << ",";
	}
      }
      this->headerFile << ")\nthrow(std::range_error);\n\n";
    }
    for(p3=params.begin();p3!=params.end();++p3){
      this->headerFile << "const double& get" << *p3 << "(void) const;\n";
    }
    for(p3=params.begin();p3!=params.end();++p3){
      this->headerFile << "double& get" << *p3 << "(void);\n";
    }
    for(p3=params.begin();p3!=params.end();++p3){
      this->headerFile << "void set" << *p3 << "(const double);\n";
    }
    if(!params.empty()){
      this->headerFile << "private:\n";
      for(p3=params.begin();p3!=params.end();++p3){
	this->headerFile << "double " << *p3 << ";\n";
      }
    }
    this->headerFile << "}; // end of class " << name << endl << endl;
    this->headerFile << "std::ostream&\n";
    this->headerFile << "operator<<(std::ostream&,const " << name;
    this->headerFile << "&);\n\n";
    this->headerFile << "} // end of namespace mfront\n\n";
    this->headerFile << "#endif /* LIB_MFRONT_" << makeUpperCase(name) << "_HXX */\n";
    this->headerFile.close();
  } // end of CppMaterialPropertyInterface::writeHeaderFile(void)

  void
  CppMaterialPropertyInterface::writeSrcFile(const std::string& file,
					     const std::string& name,
					     const std::string& author,
					     const std::string& date,
					     const std::string& output,
					     const VariableDescriptionContainer& inputs,
					     const std::vector<std::string>& materialLaws,
					     const StaticVariableDescriptionContainer& staticVars,
					     const std::vector<std::string>& params,
					     const std::map<std::string,double>& paramValues,
					     const LawFunction& function,
					     const std::vector<VariableBoundsDescription>& bounds,
					     const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    vector<string>::const_iterator p;
    VariableDescriptionContainer::const_iterator p3;
    map<string,string>::const_iterator p4;
    vector<string>::const_reverse_iterator p5;
    vector<string>::const_reverse_iterator p5e;
    vector<VariableBoundsDescription>::const_iterator p6;
    map<string,double>::const_iterator p7;

    this->srcFile << "/*!" << endl;

    this->srcFile << "* \\file   " << this->srcFileName  << endl;

    this->srcFile << "* \\brief  " << "this file implements the " 
		  << name << " MaterialLaw.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!author.empty()){
      this->srcFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->srcFile << "* \\date   " << date << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<cstdlib>\n";
    this->srcFile << "#include<sstream>\n";
    this->srcFile << "#include<cstring>\n";
    this->srcFile << "#include\"" << name << "-cxx.hxx\"\n\n";
    this->srcFile << "namespace mfront\n{\n\n";
    this->srcFile << name;
    this->srcFile << "::" << name << "()\n";
    if(!params.empty()){
      this->srcFile << ": ";
    }
    for(p=params.begin();p!=params.end();){
      p7 = paramValues.find(*p);
      if(p7==paramValues.end()){
	string msg("MaterialPropertyDSL::writeCppSrcFile : ");
	msg += "internal error (can't find value of parameter '" + *p + "')";
	throw(runtime_error(msg));
      }
      this->srcFile << *p << "(" << p7->second << ")";
      if(++p!=params.end()){
	this->srcFile << ",";
      }
      this->srcFile << "\n";
    }
    this->srcFile << "{} // end of " << name << "::" << name << "\n\n";
    this->srcFile << name;
    this->srcFile << "::" << name << "(const ";
    this->srcFile << name;
    this->srcFile << "&";
    if(!params.empty()){
      this->srcFile << " src)\n: ";
    } else {
      this->srcFile << ")\n";
    }
    for(p=params.begin();p!=params.end();){
      this->srcFile << *p << "(src." << *p << ")";
      if(++p!=params.end()){
	this->srcFile << ",";
      }
      this->srcFile << "\n";
    }
    this->srcFile << "{} // end of " << name << "::" << name << "\n\n";

    this->srcFile << name;
    this->srcFile << "&\n";
    this->srcFile << name;
    this->srcFile << "::operator=(const " << name;
    this->srcFile << "&";
    if(!params.empty()){
      this->srcFile << "src)\n{\n";
    } else {
      this->srcFile << ")\n{\n";
    }
    for(p=params.begin();p!=params.end();++p){
      this->srcFile << "" << *p << " = src." << *p << ";\n";
    }
    this->srcFile << "return *this;\n";
    this->srcFile << "} // end of " << name << "::operator=\n\n";
    for(p=params.begin();p!=params.end();++p){
      this->srcFile << "const double& ";
      this->srcFile << name;
      this->srcFile << "::get"
		    << *p << "(void) const{\nreturn " << *p 
		    << ";\n} // end of " << name << "::get\n\n";
    }
    for(p=params.begin();p!=params.end();++p){
      this->srcFile << "double& ";
      this->srcFile << name;
      this->srcFile << "::get"
		    << *p << "(void){\nreturn " << *p 
		    << ";\n} // end of " << name << "::get\n\n";
    }
    for(p=params.begin();p!=params.end();++p){
      this->srcFile << "void " << name;
      this->srcFile  << "::set" << *p;
      this->srcFile << "(const double value)";
      this->srcFile << "{\n" << "" << *p << " = value;\n"
		    << "} // end of " << name << "::set\n\n";
    }
    // Compute
    this->srcFile << "double\n";
    this->srcFile << name;
    this->srcFile << "::defaultCompute(";      
    if(!inputs.empty()){
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << "const double " << p3->name;
	if((++p3)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
    } else {
      this->srcFile << "void";
    }
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;" << endl;
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("MaterialPropertyDSL::writeCppSrcFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("MaterialPropertyDSL::writeCppSrcFile",
			 srcFile,staticVars,file);
    // parameters
    if(!params.empty()){
      for(p=params.begin();p!=params.end();++p){
	p7 = paramValues.find(*p);
	if(p7==paramValues.end()){
	  string msg("MaterialPropertyDSL::writeCppSrcFile : ");
	  msg += "internal error (can't find value of parameter '" + *p + "')";
	  throw(runtime_error(msg));
	}
	this->srcFile << "static constexpr double " << *p
		      << " = " << p7->second << ";\n";
      }
    }
    this->srcFile << "double " << output << ";\n";
    if((!bounds.empty())||
       (!physicalBounds.empty())){
      this->srcFile << "#ifndef NO_BOUNDS_CHECK\n";
      this->srcFile << name << "::checkBounds(";
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << p3->name;
	if((++p3)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
      this->srcFile << ");\n";
      this->srcFile << "#endif /* NO_BOUNDS_CHECK */\n";
    }
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
    this->srcFile << "} // end of " << name << "::defaultCompute\n\n";
    this->srcFile << "double\n";
    this->srcFile << name;
    this->srcFile << "::compute(";
    for(p3=inputs.begin();p3!=inputs.end();){
      this->srcFile << "const double " << p3->name;
      if(++p3!=inputs.end()){
	this->srcFile << ",";
      }
    }
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;" << endl;
    writeStaticVariables("MaterialPropertyDSL::writeSrcFile",
			 srcFile,staticVars,file);
    this->srcFile << "double " << output << ";\n";
    if((!bounds.empty())||
       (!physicalBounds.empty())){
      this->srcFile << "#ifndef NO_BOUNDS_CHECK\n";	
      this->srcFile << name << "::checkBounds(";
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << p3->name;
	if((++p3)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
      this->srcFile << ");\n";
      this->srcFile << "#endif /* NO_BOUNDS_CHECK */\n";	
    }
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
    this->srcFile << "} // end of " << name << "::compute\n\n";
    this->srcFile << "double\n";
    this->srcFile << name;
    this->srcFile << "::operator()(";
    for(p3=inputs.begin();p3!=inputs.end();){
      this->srcFile << "const double " << p3->name;
      if(++p3!=inputs.end()){
	this->srcFile << ",";
      }
    }
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;" << endl;
    writeStaticVariables("MaterialPropertyDSL::writeSrcFile",
			 srcFile,staticVars,file);
    this->srcFile << "double " << output << ";\n";
    if((!bounds.empty())||
       (!physicalBounds.empty())){
      this->srcFile << "#ifndef NO_BOUNDS_CHECK\n";	
      this->srcFile << name << "::checkBounds(";
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << p3->name;
	if((++p3)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
      this->srcFile << ");\n";
      this->srcFile << "#endif /* NO_BOUNDS_CHECK */\n";	
    }
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
    this->srcFile << "} // end of " << name << "::operator()\n\n";
    if((!bounds.empty())||
       (!physicalBounds.empty())){
      this->srcFile << "void\n";
      this->srcFile << name;
      this->srcFile << "::checkBounds(";
      if(!inputs.empty()){
	for(p3=inputs.begin();p3!=inputs.end();){
	  this->srcFile << "const double " << p3->name;
	  if((++p3)!=inputs.end()){
	    this->srcFile << ",";
	  }
	}
      } else {
	this->srcFile << "void";
      }
      this->srcFile << ")\n throw(std::range_error)\n{\n";
      if(!physicalBounds.empty()){
	this->srcFile << "using namespace std;\n";	
	this->srcFile << "// treating physical bounds\n";
	for(p6=physicalBounds.begin();
	    p6!=physicalBounds.end();++p6){
	  if(p6->boundsType==VariableBoundsDescription::Lower){
	    this->srcFile << "if(" << p6->varName<< " < "<< p6->lowerBound << "){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is below its physical lower bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" < " << p6->lowerBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "}\n";
	  } else if(p6->boundsType==VariableBoundsDescription::Upper){
	    this->srcFile << "if(" << p6->varName<< " > "<< p6->upperBound << "){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is beyond its physical upper bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" > " << p6->upperBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "}\n";
	  } else {
	    this->srcFile << "if(" << p6->varName<< " < "<< p6->lowerBound << "){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is below its physical lower bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" < " << p6->lowerBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "}\n";
	    this->srcFile << "if(" << p6->varName<< " > "<< p6->upperBound << "){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is beyond its physical upper bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" > " << p6->upperBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      if(!bounds.empty()){
	this->srcFile << "// treating standard bounds\n";
	for(p6=bounds.begin();
	    p6!=bounds.end();++p6){
	  if(p6->boundsType==VariableBoundsDescription::Lower){
	    this->srcFile << "if(" << p6->varName<< " < "<< p6->lowerBound << "){\n";
	    this->srcFile << "const char * const policy = "
			  << "::getenv(\"OUT_OF_BOUND_POLICY\");\n";
	    this->srcFile << "if(policy!=0){\n";
	    this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is below its lower bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" < " << p6->lowerBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "} else if(strcmp(policy,\"WARNING\")==0){\n";
	    this->srcFile << "cerr << \"" << p6->varName << " is below its lower bound\";\n";
	    this->srcFile << "cerr << \"(\" << " << p6->varName 
			  << " << \" < " << p6->lowerBound << " )\";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	  } else if(p6->boundsType==VariableBoundsDescription::Upper){
	    this->srcFile << "if(" << p6->varName<< " > "<< p6->upperBound << "){\n";
	    this->srcFile << "const char * const policy = "
			  << "::getenv(\"OUT_OF_BOUND_POLICY\");\n";
	    this->srcFile << "if(policy!=0){\n";
	    this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is beyond its upper bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" > " << p6->upperBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "} else if(strcmp(policy,\"WARNING\")==0){\n";
	    this->srcFile << "cerr << \"" << p6->varName << " is beyond its upper bound\";\n";
	    this->srcFile << "cerr << \"(\" << " << p6->varName 
			  << " << \" > " << p6->upperBound << " )\";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	  } else {
	    this->srcFile << "if(" << p6->varName<< " < "<< p6->lowerBound << "){\n";
	    this->srcFile << "const char * const policy = "
			  << "::getenv(\"OUT_OF_BOUND_POLICY\");\n";
	    this->srcFile << "if(policy!=0){\n";
	    this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is below its lower bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" < " << p6->lowerBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "} else if(strcmp(policy,\"WARNING\")==0){\n";
	    this->srcFile << "cerr << \"" << p6->varName << " is below its lower bound\";\n";
	    this->srcFile << "cerr << \"(\" << " << p6->varName 
			  << " << \" < " << p6->lowerBound << " )\";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "if(" << p6->varName<< " > "<< p6->upperBound << "){\n";
	    this->srcFile << "const char * const policy = "
			  << "::getenv(\"OUT_OF_BOUND_POLICY\");\n";
	    this->srcFile << "if(policy!=0){\n";
	    this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	    this->srcFile << "ostringstream msg;\n";
	    this->srcFile << "msg << \"" << name << " : "  << p6->varName << " is beyond its upper bound\";\n";
	    this->srcFile << "msg << \"(\" << " << p6->varName 
			  << " << \" > " << p6->upperBound << " )\";\n";
	    this->srcFile << "throw(range_error(msg.str()));\n";
	    this->srcFile << "} else if(strcmp(policy,\"WARNING\")==0){\n";
	    this->srcFile << "cerr << \"" << p6->varName << " is beyond its upper bound\";\n";
	    this->srcFile << "cerr << \"(\" << " << p6->varName 
			  << " << \" > " << p6->upperBound << " )\";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      this->srcFile << "} // end of " << name << "::checkBounds\n\n";
    }
    this->srcFile << "std::ostream&\n";
    this->srcFile << "operator<<(std::ostream& os,const "
		  << name;
    if(!params.empty()){
      this->srcFile << "& src){\n";
    } else {
      this->srcFile << "&){\n";
    }
    for(p=params.begin();p!=params.end();++p){
      this->srcFile << "os << \"" << *p << " : \" << src.get" << *p << "() << std::endl;\n";
    }
    this->srcFile << "return os;\n}// end of operator(std::ostream& os," 
		  << name << "\n\n";
    this->srcFile << "} // end of namespace mfront\n\n";
    this->srcFile.close();
  } // end of CppMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront